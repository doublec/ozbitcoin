# Mozart/Oz libraries for Bitcoin

This library contains utilities for interfacing with Bitcoin. It can be built with:

    ozmake

Installation:

    ozmake --install

Import of the library can be done in the OPI with:

    declare [Bitcoin] = {Module.link ['x-ozlib://doublec/bitcoin/Bitcoin.ozf']}

Currently the library exposes the following functions:

* `{Bitcoin.rpc url auth string}` - Given a JSON-RPC in string, call the given server and return the result.

        declare Input = json(version:"1.1" id:1 method: getinfo params: nil)
        declare Output = {Bitcoin.rpc "http://localhost:8332" "user:password" Input}
	{Browse Output}

You'll need the following libraries to build:

* [libcurl](http://curl.haxx.se/libcurl/)
* [ozjson](http://github.com/doublec/ozjson)

Recent versions of this library can be obtained from:

    http://github.com/doublec/ozbitcoin

The author can be reached at:

    Chris Double
    chris.double@double.co.nz
    http://www.bluishcoder.co.nz
