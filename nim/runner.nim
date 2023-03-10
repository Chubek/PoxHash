import libpoxh
import os
import times

const
  MAX_FLAG_SIZE: int = 24
  MIN_FLAG_SIZE: int = 3
  MIN_ARG_NUM: int = 1
  NUM_ASCII: int = 128
  FORMAT_MARKER: char = '%'
  FORMAT_STR: char = 's'
  FORMAT_DIGIT: char = 'd'
  FORMAT_CHAR: char = 'c'
  NS_TO_US = 1000
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
  for c in **str:
    result.add(^^c)

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
  printf("\x1b[1;30;42mHelp | Chubak#7400 (Discord) | @bidpaafx (Telegram) | Chubakbidpaa[at]gmail\x1b[0m\n")
  println()
  printf("Examples \x1b[1m(flags go between two dashes!)\x1b[0m:\n")
  printf("%s -N82- myword1\n", execName)
  printf("%s -*+^- mywod to be joined\n", execName)
  printf("%s -Dhob- word1 word 2\n", execName)
  printf("%s -^^+- large seq  to join and  benchmark\n", execName)
  printf("wget -qO- www.example.com | xargs bash -c '%s -h+- $@'\n", execName)
  println()
  printf("\x1b[1;32mFlags:\x1b[0m\n")
  printf("\x1b[1;35m\t`%c`\x1b[0m: Benchmark run (pass two to only show benchmark)\n", FLAG_BENCHMARK)
  printf("\x1b[1;35m\t`%c`\x1b[0m: Join arguments with space (byte 32)\n", FLAG_JOIN)
  printf("\x1b[1;35m\t`%c`\x1b[0m: Print every digest\n", FLAG_EVERTHING)
  printf("\x1b[1;35m\t`%c`\x1b[0m: Print every non-decimal digest\n", FLAG_ALL_NON_DEC)
  printf("\x1b[1;35m\t`%c`\x1b[0m: Print every decimal digest\n", FLAG_ALL_DECIMAL)
  printf("\x1b[1;35m\t`%c`\x1b[0m: Print bytes digest (eight unsigned 8-bit integers)\n", FLAG_BYTES)
  printf("\x1b[1;35m\t`%c`\x1b[0m: Print words digest (four unsigned 16-bit integers)\n", FLAG_WORDS)
  printf("\x1b[1;35m\t`%c`\x1b[0m: Print doubles digest (two unsigned 32-bit integers)\n", FLAG_DOUBLES)
  printf("\x1b[1;35m\t`%c`\x1b[0m: Print quad digest (one unsigned 64-bit integer)\n", FLAG_QUAD)
  printf("\x1b[1;35m\t`%c`\x1b[0m: Print sexagesimal digest (base sixty)\n", FLAG_SEX)
  printf("\x1b[1;35m\t`%c`\x1b[0m: Print vigesimal digest (base twenty)\n", FLAG_VIG)
  printf("\x1b[1;35m\t`%c`\x1b[0m: Print hexadecimal digest (base sixteen)\n", FLAG_HEX)
  printf("\x1b[1;35m\t`%c`\x1b[0m: Print tetradecimal digest (base fourteen)\n", FLAG_TET)
  printf("\x1b[1;35m\t`%c`\x1b[0m: Print duodecimal digest (base twelve)\n", FLAG_DUO)
  printf("\x1b[1;35m\t`%c`\x1b[0m: Print octal digest (base eight)\n", FLAG_OCT)
  printf("\x1b[1;35m\t`%c`\x1b[0m: Print senary digest (base six)\n", FLAG_SEN)
  printf("\x1b[1;35m\t`%c`\x1b[0m: Print binary digest (base two)\n", FLAG_BIN)
  printf("\x1b[1;35m\t`%c`\x1b[0m: Print Help\n\n", FLAG_HELP)
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
      of FLAG_BENCHMARK:
        continue
      of FLAG_JOIN:
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

proc getTimeInUS(): uint64 = cast[uint64](getTime().nanosecond() div NS_TO_US)

proc allAreFalse(bools: seq[bool]): bool =
  for bl in **bools:
    if bl:
      return false
  return true

proc joinArgs(args: seq[string]): string =
  for arg in **args:
    result += " "
    result += arg
  result = result[1..result.len() - 1]

proc printHashes(hashes: seq[PoxDigest], flags: string, totalTime: uint64,
    joined: string) =
  var
    lenHashes = hashes.len()
    reoccurrance = searchForFlagReoccurrances(flags)

  if argHasFlag(flags, FLAG_BENCHMARK):
    printf(
      "Total time for hashing %d bytestring(s): %dus \n",
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
    duo = argHasFlag(flags, FLAG_TET)
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

  for (i, hash) in enumerate(hashes):
    printf("----\n")
    printf(
      "Requested digests for bytestring #%d%s\n",
      i + 1,
      joined
    )
    if everything or allFlagsDecimal or by:
      printf(
        "\tBytes: U8[%d, %d, %d, %d, %d, %d, %d, %d]\n",
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
        "\tWords: U16[%d, %d, %d, %d]\n",
        hash.words[0],
        hash.words[1],
        hash.words[2],
        hash.words[3]
      )

    if everything or allFlagsDecimal or dub:
      printf(
        "\tdoubles: U32[%d, %d]\n",
        hash.doubles[0],
        hash.doubles[1]
      )

    if everything or allFlagsDecimal or quad:
      printf("\tQuad: U64[%d]\n", hash.quad)

    if everything or allFlagsNonDecimal or sex:
      printf("\tSexdigest: %s\n", hash.sexdigest)

    if everything or allFlagsNonDecimal or vig:
      printf("\tVigdigest: %s\n", hash.vigdigest)

    if everything or allFlagsNonDecimal or hex:
      printf("\tHexdigest: %s\n", hash.hexdigest)

    if everything or allFlagsNonDecimal or tet:
      printf("\tTetdigest: %s\n", hash.tetdigest)

    if everything or allFlagsNonDecimal or duo:
      printf("\tDuodigest: %s\n", hash.duodigest)

    if everything or allFlagsNonDecimal or oct:
      printf("\tOctdigest: %s\n", hash.octdigest)

    if everything or allFlagsNonDecimal or sen:
      printf("\tSendgiest: %s\n", hash.sendigest)

    if everything or allFlagsNonDecimal or bin:
      printf("\tBindigest: %s\n", hash.bindigest)


proc main(exec: string, argv: seq[string]) =
  printf("\x1b[1;30;47mPoxHashRunner   |   Nim    |  March 2023 - Chubak Bidpaa  |  GPLv3  \x1b[0m\n")
  validateFlags(exec, argv)
  var
    flagsArg = argv[0]
    lenHashes = argv.len() - 1
    hashes = newSeq[PoxDigest](lenHashes)
    totalTime, t1, t2: uint64

  totalTime = 0
  if argHasFlag(flagsArg, FLAG_JOIN):
    var argsJoined = joinArgs(argv[1..lenHashes])
    t1 = getTimeInUS()
    hashes[0] = PoxHash(^^argsJoined)
    t2 = getTimeInUS()
    printHashes(hashes[0..0], flagsArg, t2 - t1, " (joined arguments):")
  else:
    for (i, arg) in enumerate(argv[1..lenHashes]):
      t1 = getTimeInUS()
      hashes[i] = PoxHash(^^arg)
      t2 = getTimeInUS()
      totalTime += t2 - t1
    printHashes(hashes, flagsArg, totalTime, ":")



var
  cmdParams = os.commandLineParams()
  exec = os.getAppFilename()

main(exec, cmdParams)
