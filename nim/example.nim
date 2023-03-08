import libpoxh
import os

proc `*`(str: string): seq[byte] =
    for c in str:
        result.add(byte(c))

template print(s: varargs[string, `$`]) =
  for x in s:
    stdout.write x

var cmdParams = os.commandLineParams()
echo "Pox (Nim) hashes for passed strings in various forms:"

var i = 1
for arg in cmdParams:
  var hash = PoxHash(*arg)
  var hexdigest = hash.hexdigest
  var bytes = hash.bytes
  var words = hash.words
  var doubles = hash.doubles
  var quad = hash.quad
  print("\n")
  print("\tArg #", i, " as follows\n")
  print("\t\thexdigest: ", hexdigest, "\n")
  print("\t\tbytes: uint8(", bytes[0], ", ", bytes[1], ", ", bytes[2], ", ", bytes[3], ", ", bytes[4], ", ", bytes[5], ", ", bytes[6], ", ", bytes[7], ")\n")
  print("\t\twords: uint16(", words[0], ", ", words[1], ", ", words[2], ", ", words[3], ")\n")
  print("\t\tdoubles: uint32(", doubles[0], ", ", doubles[1], ")\n")
  print("\t\tquad: uint64(", quad, ")\n")
  print("\n")
