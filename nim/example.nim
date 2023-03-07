import libpoxh
import os
import std/strformat

proc `*`(str: string): seq[byte] =
    for c in str:
        result.add(byte(c))

template print(s: varargs[string, `$`]) =
  for x in s:
    stdout.write x

var cmdParams = os.commandLineParams()
echo "Pox (Nim) hashes for passed strings in hexdigest form:"

var i = 1
for arg in cmdParams:
    var hash = PoxHash(*arg)
    print('\t', fmt"{i} -> {hash.hexdigest} for", "\n\t", fmt"`{arg}`", '\n')
