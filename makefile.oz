makefile(
   lib : [
	  'Example.exe'
	  'Bitcoin.ozf'
	  'bitcoinrpc.so'
	 ]
   rules:o('bitcoinrpc.so':ld('bitcoinrpc.o' [library('curl') library('ssl') library('rt')]))
   uri: 'x-ozlib://doublec/bitcoin'
   mogul: 'mogul:/doublec/bitcoin'
   blurb: "Wrapper for Bitcoin RPC and utilities"
)
