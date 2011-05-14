functor
import
   Bitcoin at 'x-ozlib://doublec/bitcoin/Bitcoin.ozf'
   Application
   System
define
   {System.show {Bitcoin.rpc 'http://localhost:9332' "chris:password" json(method:'getinfo' version:'1.1'	id:1 params: nil)}}
in
   {Application.exit 0}
end
