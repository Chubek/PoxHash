#####################################################################################
##                       Runner for libpoxh.nim (PoxHash Nim)                      ##
##                          March 2023 - Chubak Bidpaa                             ##
#####################################################################################
## MIT License                                                                     ##
##                                                                                 ##
## Copyright (c) 2023 Chubak Bidpaa                                                ##
##                                                                                 ##
## Permission is hereby granted, free of charge, to any person obtaining a copy    ##
## of this software and associated documentation files (the "Software"), to deal   ##
## in the Software without restriction, including without limitation the rights    ##
## to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       ##
## copies of the Software, and to permit persons to whom the Software is           ##
## furnished to do so, subject to the following conditions:                        ##
##                                                                                 ##
## The above copyright notice and this permission notice shall be included in all  ##
## copies or substantial portions of the Software.                                 ##
##                                                                                 ##
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      ##
## IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        ##
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     ##
## AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          ##
## LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   ##
## OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   ##
## SOFTWARE.                                                                       ##
#####################################################################################


import libpoxh
import os
import times
import sequtils

const
  MAX_FLAG_SIZE: int = 24
  MIN_FLAG_SIZE: int = 3
  MIN_ARG_NUM: int = 1
  NUM_ASCII: int = 128
  FORMAT_MARKER: char = '%'
  FORMAT_STR: char = 's'
  FORMAT_DIGIT: char = 'd'
  FORMAT_CHAR: char = 'c'
  BENCHMARK_BYTE_INDEX = 94

  FLAG_BENCHMARK = '^'
  FLAG_JOIN = '+'
  FLAG_EVERTHING = '*'
  FLAG_ALL_NON_DEC = 'N'
  FLAG_ALL_DECIMAL = 'D'
  FLAG_BYTES = '8'
  FLAG_WORDS = '4'
  FLAG_DOUBLES = '2'
  FLAG_QUAD = '1'
  FLAG_SEX = 'g'
  FLAG_VIG = 'v'
  FLAG_HEX = 'h'
  FLAG_TET = 't'
  FLAG_DUO = 'd'
  FLAG_OCT = 'o'
  FLAG_SEN = 's'
  FLAG_BIN = 'b'
  FLAG_HELP = '?'
  FLAG_DASH = '-'
  FLAG_NHEADER = 'z'
  FLAG_ECHO = 'e'

  FILE_PREFIX = "file="
  FILE_PREFIX_LEN = 5

  WRONG_FLAGS = @[
    ('G', 'g'),
    ('V', 'v'),
    ('O', 'o'),
    ('T', 't'),
    ('S', 's'),
    ('H', 'h'),
    ('n', 'N'),
    ('W', '4'),
    ('w', '4'),
    ('q', '1'),
    ('Q', '1'),
    ('3', '2'),
    ('5', '4'),
    ('6', '^'),
    ('7', '8'),
    ('9', '8'),
    ('0', '1'),
    ('/', '?'),
    ('=', '+'),
    ('B', 'b'),
    ('E', '*'),
    ('A', '*'),
    ('>', '?'),
    ('&', '*'),
    ('r', 't'),
    ('y', 't'),
    ('f', 'g'),
    ('x', 'h'),
    ('Z', 'z'),
    ('a', 'z'),
    ('E', 'e'),
    ('w', 'e'),
    ('r', 'e'),
    ('i', 'e'),
  ]


iterator `...`(a, b: int): int =
  var i = a
  while i < b:
    yield i
    inc i

iterator `<..`(a, b: int): int =
  var i = b - 1
  while i >= a:
    yield i
    dec i


iterator `**`(str: string): char =
  var i = 0
  while i < str.len():
    yield str[i]
    inc i

iterator `**`[T](sequence: seq[T]): T =
  var i = 0
  while i < sequence.len():
    yield sequence[i]
    inc i

iterator enumerate[T](sequence: seq[T]): (int, T) =
  for i in 0..(sequence.len() - 1):
    yield (i, sequence[i])

proc `+`(str1, str2: string): string =
  var
    len1 = str1.len()
    len2 = str2.len()
    j = 0
  result = newString(len1 + len2)
  for i in 0...len1:
    result[i] = str1[i]
  for k in len1...(len1 + len2):
    result[k] = str2[j]
    inc j

proc `+=`(str1: var string, str2: string) = str1 = str1 + str2
proc `*`(c: char): string =
  result = newString(1)
  result[0] = c
proc `^^`(c: char): uint8 = cast[uint8](c)
proc `^^`(str: string): seq[uint8] =
  map(str, proc(x: char): uint8 = ^^x)

proc printf(input: varargs[string, `$`]) =
  var
    message = input[0]
    lenMessage = message.len()
    finalMessage = ""
    currChar: char
    peekChar: char
    index = 0
    cursor = 1
  while index < lenMessage - 1:
    currChar = message[index]
    peekChar = message[index + 1]
    if currChar == FORMAT_MARKER:
      if peekChar == FORMAT_CHAR or peekChar == FORMAT_DIGIT or peekChar == FORMAT_STR:
        finalMessage += input[cursor]
        inc cursor
        index += 2
        continue
    finalMessage += *currChar
    inc index
  finalMessage += *message[lenMessage - 1]
  stdout.write finalMessage


proc println() = stdout.write "\n"

template errorOut(message: untyped): untyped =
  println()
  printf(message)
  println()
  printf(
    "\x1b[1;31mError occurred\x1b[0m. Please pass \x1b[134m-?-\x1b[0m to show help\n"
  )
  quit(1)

proc printHelp(execName: string) =
  printf("\x1b[1;30;42mHelp | Chubak#7400 (Discord) | @bidpaafx (Telegram) | Chubakbidpaa[at]gmail[dot]com\x1b[0m\n")
  println()
  printf("Examples \x1b[1m(flags go between two dashes!)\x1b[0m:\n")
  printf("%s -N82- myword1\n", execName)
  printf("%s -*+^- mywod to be joined\n", execName)
  printf("%s -Dhob- word1 word 2\n", execName)
  printf("%s -^^+- large seq  to join and  benchmark\n", execName)
  printf("wget -qO- www.example.com | xargs bash -c '%s -h+- $@'\n", execName)
  printf("If an argument stats with `%s`, it will lead to file read attempt, unles `%c` is passed\n",
      FILE_PREFIX, FLAG_JOIN)
  println()
  printf("\x1b[1;32mFlags:\x1b[0m\n")
  printf("\x1b[1;33m\t`%c`\x1b[0m: Echo argument\n", FLAG_ECHO)
  printf("\x1b[1;33m\t`%c`\x1b[0m: Don't print header message\n", FLAG_NHEADER)
  printf("\x1b[1;33m\t`%c`\x1b[0m: Benchmark run (pass two to only show benchmark)\n", FLAG_BENCHMARK)
  printf("\x1b[1;33m\t`%c`\x1b[0m: Join arguments with space (byte 32)\n", FLAG_JOIN)
  printf("\x1b[1;33m\t`%c`\x1b[0m: Print every digest\n", FLAG_EVERTHING)
  printf("\x1b[1;33m\t`%c`\x1b[0m: Print every non-decimal digest\n", FLAG_ALL_NON_DEC)
  printf("\x1b[1;33m\t`%c`\x1b[0m: Print every decimal digest\n", FLAG_ALL_DECIMAL)
  printf("\x1b[1;33m\t`%c`\x1b[0m: Print bytes digest (eight unsigned 8-bit integers)\n", FLAG_BYTES)
  printf("\x1b[1;33m\t`%c`\x1b[0m: Print words digest (four unsigned 16-bit integers)\n", FLAG_WORDS)
  printf("\x1b[1;33m\t`%c`\x1b[0m: Print doubles digest (two unsigned 32-bit integers)\n", FLAG_DOUBLES)
  printf("\x1b[1;33m\t`%c`\x1b[0m: Print quad digest (one unsigned 64-bit integer)\n", FLAG_QUAD)
  printf("\x1b[1;33m\t`%c`\x1b[0m: Print sexagesimal digest (base sixty)\n", FLAG_SEX)
  printf("\x1b[1;33m\t`%c`\x1b[0m: Print vigesimal digest (base twenty)\n", FLAG_VIG)
  printf("\x1b[1;33m\t`%c`\x1b[0m: Print hexadecimal digest (base sixteen)\n", FLAG_HEX)
  printf("\x1b[1;33m\t`%c`\x1b[0m: Print tetradecimal digest (base fourteen)\n", FLAG_TET)
  printf("\x1b[1;33m\t`%c`\x1b[0m: Print duodecimal digest (base twelve)\n", FLAG_DUO)
  printf("\x1b[1;33m\t`%c`\x1b[0m: Print octal digest (base eight)\n", FLAG_OCT)
  printf("\x1b[1;33m\t`%c`\x1b[0m: Print senary digest (base six)\n", FLAG_SEN)
  printf("\x1b[1;33m\t`%c`\x1b[0m: Print binary digest (base two)\n", FLAG_BIN)
  printf("\x1b[1;33m\t`%c`\x1b[0m: Print Help\n\n", FLAG_HELP)
  quit(1)

proc getExecName(path: string): string =
  var
    slashIndex = 0
    pathLen = path.len() - 1
  for i in 0<..pathLen:
    if path[i] == '/':
      break
    slashIndex = i
  result = path[slashIndex..pathLen]

proc checkForWrongFlags(flags: string) =
  for flag in **flags:
    for wrongFlagTuplet in **WRONG_FLAGS:
      var (wrongFlag, rightFlag) = wrongFlagTuplet
      if flag == wrongFlag:
        printf(
          "No flag for `%c`, perhaps you meant `%c`?",
          flag,
          rightFlag
        )
        errorOut("Flag erreror")

proc argHasFlag(flags: string, mustHave: char): bool =
  for flag in **flags:
    if flag == mustHave:
      return true
  return false

proc searchForFlagReoccurrances(flags: string): char =
  var
    occurranceArray: array[NUM_ASCII, uint8]

  for c in **flags:
    occurranceArray[^^c] += 1

  if occurranceArray[BENCHMARK_BYTE_INDEX] == 2:
    result = FLAG_BENCHMARK

  if occurranceArray[BENCHMARK_BYTE_INDEX] > 2:
    errorOut("`^` can appear at most twice")

  for i in 0...NUM_ASCII:
    if i == 45 or i == BENCHMARK_BYTE_INDEX:
      continue
    if occurranceArray[i] > 1:
      return cast[char](i)

proc validateFlags(exec: string, argv: seq[string]) =
  var
    lenArgv = argv.len()
    flagsArg = argv[0]
    lenFlags = flagsArg.len()
    helpPassed = false
    reoccurrance = '\0'
    allFlagsPassed = false
    allFlagsNonDecPassed = false
    allFlagsDecPassed = false
    execName = getExecName(exec)

  if lenArgv < MIN_ARG_NUM:
    errorOut("No flags passed")

  if lenFlags < MIN_FLAG_SIZE or lenFlags > MAX_FLAG_SIZE:
    errorOut("Length of the first argument must at least be 3 and at most 24")

  if flagsArg[0] != FLAG_DASH or flagsArg[lenFlags - 1] != FLAG_DASH:
    errorOut("The flag argument must begin and end with `-`")

  checkForWrongFlags(flagsArg)

  if flagsArg == "-?-":
    printHelp(execName)

  helpPassed = argHasFlag(flagsArg, FLAG_HELP)
  if helpPassed and lenFlags > MIN_FLAG_SIZE:
    errorOut("You may not pass the `?` flag along with other flags")

  reoccurrance = searchForFlagReoccurrances(flagsArg)
  if reoccurrance != '\0' and reoccurrance != FLAG_BENCHMARK:
    printf("Flag `%c` appears twice", reoccurrance)
    errorOut("Only `^` can appear twice")

  if lenArgv < MIN_ARG_NUM + 1:
    errorOut("You must pass at least one argument to hash")

  allFlagsPassed = argHasFlag(flagsArg, FLAG_EVERTHING)
  allFlagsDecPassed = argHasFlag(flagsArg, FLAG_ALL_DECIMAL)
  allFlagsNonDecPassed = argHasFlag(flagsArg, FLAG_ALL_NON_DEC)

  for flag in **flagsArg[1..lenFlags - 2]:
    case flag:
      of FLAG_BENCHMARK, FLAG_JOIN, FLAG_NHEADER, FLAG_ECHO:
        continue
      of FLAG_EVERTHING:
        if allFlagsDecPassed or allFlagsNonDecPassed:
          errorOut("You may not pass `*` when you have passed `N` or `D`")
        continue
      of FLAG_ALL_NON_DEC:
        if allFlagsPassed:
          errorOut("You may not pass `N` when `*` is passed")
        continue
      of FLAG_ALL_DECIMAL:
        if allFlagsPassed:
          errorOut("You may not pass `D` when `*` is passed")
        continue
      of FLAG_BYTES:
        if allFlagsDecPassed or allFlagsPassed:
          errorOut(
            "You may not pass a decimal digest flag when `*` or `D` is passed"
          )
        continue
      of FLAG_WORDS:
        if allFlagsDecPassed or allFlagsPassed:
          errorOut(
            "You may not pass a decimal digest flag when `*` or `D` is passed"
          )
        continue
      of FLAG_DOUBLES:
        if allFlagsDecPassed or allFlagsPassed:
          errorOut(
            "You may not pass a decimal digest flag when `*` or `D` is passed"
          )
        continue
      of FLAG_QUAD:
        if allFlagsDecPassed or allFlagsPassed:
          errorOut(
            "You may not pass a decimal digest flag when `*` or `D` is passed"
          )
        continue
      of FLAG_SEX:
        if allFlagsNonDecPassed or allFlagsPassed:
          errorOut(
            "You may not pass a non-decimal digest flag when `*` or `N` is passed"
          )
        continue
      of FLAG_VIG:
        if allFlagsNonDecPassed or allFlagsPassed:
          errorOut(
            "You may not pass a non-decimal digest flag when `*` or `N` is passed"
          )
        continue
      of FLAG_HEX:
        if allFlagsNonDecPassed or allFlagsPassed:
          errorOut(
            "You may not pass a non-decimal digest flag when `*` or `N` is passed"
          )
        continue
      of FLAG_TET:
        if allFlagsNonDecPassed or allFlagsPassed:
          errorOut(
            "You may not pass a non-decimal digest flag when `*` or `N` is passed"
          )
        continue
      of FLAG_DUO:
        if allFlagsNonDecPassed or allFlagsPassed:
          errorOut(
            "You may not pass a non-decimal digest flag when `*` or `N` is passed"
          )
        continue
      of FLAG_OCT:
        if allFlagsNonDecPassed or allFlagsPassed:
          errorOut(
            "You may not pass a non-decimal digest flag when `*` or `N` is passed"
          )
        continue
      of FLAG_SEN:
        if allFlagsNonDecPassed or allFlagsPassed:
          errorOut(
            "You may not pass a non-decimal digest flag when `*` or `N` is passed"
          )
        continue
      of FLAG_BIN:
        if allFlagsNonDecPassed or allFlagsPassed:
          errorOut(
            "You may not pass a non-decimal digest flag when `*` or `N` is passed"
          )
        continue
      of FLAG_HELP:
        if lenFlags > MIN_FLAG_SIZE:
          errorOut("You may not pass the `?` flag along with other flags")
      of FLAG_DASH:
        errorOut(
          "You may not use `-` in the first argument other than in the first, and the last letter"
        )
      else:
        errorOut("Unknown flag detected!")

proc getTimeInUS(): Duration = initDuration(nanoseconds = getTime().nanosecond())

proc allAreFalse(bools: seq[bool]): bool =
  for bl in **bools:
    if bl:
      return false
  return true

proc printHashes(hashes: seq[PoxDigest], flags: string, totalTime: int64) =
  var
    lenHashes = hashes.len()
    reoccurrance = searchForFlagReoccurrances(flags)

  if argHasFlag(flags, FLAG_BENCHMARK):
    printf(
      "Total time for hashing %d unsigned bytearrays(s): %dus \n",
      lenHashes,
      totalTime
    )

  if reoccurrance == FLAG_BENCHMARK:
    println()
    quit(0)

  var
    everything = argHasFlag(flags, FLAG_EVERTHING)
    allFlagsDecimal = argHasFlag(flags, FLAG_ALL_DECIMAL)
    allFlagsNonDecimal = argHasFlag(flags, FLAG_ALL_NON_DEC)
    by = argHasFlag(flags, FLAG_BYTES)
    word = argHasFlag(flags, FLAG_WORDS)
    dub = argHasFlag(flags, FLAG_DOUBLES)
    quad = argHasFlag(flags, FLAG_QUAD)
    sex = argHasFlag(flags, FLAG_SEX)
    vig = argHasFlag(flags, FLAG_VIG)
    hex = argHasFlag(flags, FLAG_HEX)
    tet = argHasFlag(flags, FLAG_TET)
    duo = argHasFlag(flags, FLAG_DUO)
    oct = argHasFlag(flags, FLAG_OCT)
    sen = argHasFlag(flags, FLAG_SEN)
    bin = argHasFlag(flags, FLAG_BIN)

  var allFalse = allAreFalse(@[
    everything,
    allFlagsDecimal,
    allFlagsNonDecimal,
    by,
    word,
    dub,
    quad,
    sex,
    vig,
    hex,
    tet,
    duo,
    oct,
    sen,
    bin,
  ])

  if allFalse:
    printf("You had not specfied any digests to be printed\n")
    quit(0)

  for hash in hashes:
    if everything or allFlagsDecimal or by:
      printf(
        "Bytes: U8[%d, %d, %d, %d, %d, %d, %d, %d]\n",
        hash.bytes[0],
        hash.bytes[1],
        hash.bytes[2],
        hash.bytes[3],
        hash.bytes[4],
        hash.bytes[5],
        hash.bytes[6],
        hash.bytes[7]
      )

    if everything or allFlagsDecimal or word:
      printf(
        "Words: U16[%d, %d, %d, %d]\n",
        hash.words[0],
        hash.words[1],
        hash.words[2],
        hash.words[3]
      )

    if everything or allFlagsDecimal or dub:
      printf(
        "Doubles: U32[%d, %d]\n",
        hash.doubles[0],
        hash.doubles[1]
      )

    if everything or allFlagsDecimal or quad:
      printf("Quad: U64[%d]\n", hash.quad)

    if everything or allFlagsNonDecimal or sex:
      printf("Sexdigest: %s\n", hash.sexdigest)

    if everything or allFlagsNonDecimal or vig:
      printf("Vigdigest: %s\n", hash.vigdigest)

    if everything or allFlagsNonDecimal or hex:
      printf("Hexdigest: %s\n", hash.hexdigest)

    if everything or allFlagsNonDecimal or tet:
      printf("Tetdigest: %s\n", hash.tetdigest)

    if everything or allFlagsNonDecimal or duo:
      printf("Duodigest: %s\n", hash.duodigest)

    if everything or allFlagsNonDecimal or oct:
      printf("Octdigest: %s\n", hash.octdigest)

    if everything or allFlagsNonDecimal or sen:
      printf("Sendgiest: %s\n", hash.sendigest)

    if everything or allFlagsNonDecimal or bin:
      printf("Bindigest: %s\n", hash.bindigest)
    printf("----\n")

proc assertFile(arg: string): bool =
  result = arg.len() > FILE_PREFIX_LEN and arg[0..FILE_PREFIX_LEN - 1] == FILE_PREFIX

proc isRegularFile(fpath: string) =
  if not fileExists(fpath):
    errorOut("Specfied file does not exist or is a directory. Pass `+` with only one argument to ignore")

proc readGivenFile(fpath: string): string =
  isRegularFile(fpath)
  result = readFile(fpath)


proc joinArgs(args: seq[string]): string =
  var warned = false
  for arg in **args:
    if assertFile(arg) and not warned:
      printf("\x1b[1;33mWarning:\x1b[0m: The `filepath=` prefix is ignored in join mode\n")
      warned = true
    result += arg
    result += " "
  result = result[0..result.len() - 2]

proc processArg(arg: string): string =
  if not assertFile(arg):
    return arg
  result = readGivenFile(arg[FILE_PREFIX_LEN..arg.len() - 1])

proc main(exec: string, argv: seq[string]) =
  validateFlags(exec, argv)
  var
    flagsArg = argv[0]
    lenHashes = argv.len() - 1
    hashes = newSeq[PoxDigest](lenHashes)
    t1, t2: Duration
    totalTime = 0i64
    processedArg: seq[uint8]
    echoArg = false

  if not argHasFlag(flagsArg, FLAG_NHEADER):
    printf("\x1b[1;30;47mPoxHashRunner   |   Nim    |  March 2023 - Chubak Bidpaa  |  GPLv3  \x1b[0m\n")

  echoArg = argHasFlag(flagsArg, FLAG_ECHO)
  if argHasFlag(flagsArg, FLAG_JOIN):
    var argsJoined = joinArgs(argv[1..lenHashes])
    if echoArg: printf("Joined Args: \n`%s`\n", args_joined)
    t1 = getTimeInUS()
    hashes[0] = PoxHash(^^argsJoined)
    t2 = getTimeInUS()
    totalTime = (t2 - t1).inMicroseconds()
    printHashes(hashes[0..0], flagsArg, totalTime)
  else:
    for (i, arg) in enumerate(argv[1..lenHashes]):
      if echoArg: printf("Arg %d: %s\n", i + 1, arg)
      processedArg = ^^processArg(arg)
      t1 = getTimeInUS()
      hashes[i] = PoxHash(processedArg)
      t2 = getTimeInUS()
      totalTime = totalTime + (t2 - t1).inMicroseconds()
    printHashes(hashes, flagsArg, totalTime)

var
  cmdParams = os.commandLineParams()
  exec = os.getAppFilename()

main(exec, cmdParams)
