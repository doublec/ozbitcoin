// Copyright (C) 2011 Chris Double.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// DEVELOPERS AND CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
#include "mozart.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <curl/curl.h>

#define BUFFER_SIZE  (256 * 1024)

struct write_result
{
    char *data;
    int pos;
};

static size_t write_response(void *ptr, size_t size, size_t nmemb, void *stream)
{
    struct write_result *result = (struct write_result *)stream;

    if(result->pos + size * nmemb >= BUFFER_SIZE - 1)
    {
        fprintf(stderr, "error: too small buffer\n");
        return 0;
    }

    memcpy(result->data + result->pos, ptr, size * nmemb);
    result->pos += size * nmemb;

    return size * nmemb;
}

static char *request(const char *url, const char* auth, const char* json)
{
  long code;
  struct curl_slist* slist = NULL;

  CURL* curl = curl_easy_init();
  char* data = new char[BUFFER_SIZE];
  if(!curl || !data)
    return NULL;

  struct write_result write_result = {
    data, 0
  };

  slist = curl_slist_append(slist, "content-type: application/json");

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &write_result);
  curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
  curl_easy_setopt(curl, CURLOPT_USERPWD, auth);
  curl_easy_setopt(curl, CURLOPT_POST, 1);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(json));

  CURLcode status = curl_easy_perform(curl);
  curl_slist_free_all(slist);
  if(status != 0)
    {
      fprintf(stderr, "error: unable to request data from %s:\n", url);
      fprintf(stderr, "%s\n", curl_easy_strerror(status));
      return NULL;
    }

  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
  if(code != 200)
    {
      fprintf(stderr, "error: server responded with code %ld\n", code);
      return NULL;
    }

  curl_easy_cleanup(curl);
  curl_global_cleanup();

  /* zero-terminate the result */
  data[write_result.pos] = '\0';
  return data;
}

OZ_BI_define(BitcoinRPC_rpc,3,1)
{
  OZ_declareVirtualString(0, Url);
  char* url = strdup(Url);
  OZ_declareVirtualString(1, Auth);
  char* auth = strdup(Auth);
  OZ_declareVirtualString(2, Json);
  char* r = request(url, auth, Json);

  OZ_Term s = OZ_string(r);
  delete[] r;

  free(url);
  free(auth);
  OZ_RETURN(s);
}
OZ_BI_end

static OZ_C_proc_interface oz_interface[] = {
  { "rpc", 3, 1, BitcoinRPC_rpc },
  { 0,0,0,0}
};

OZ_C_proc_interface *oz_init_module() {
  return oz_interface;
}
