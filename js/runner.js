/////////////////////////////////////////////////////////////////////////////////////
//                   Runner for libpoxh.js (PoxHash JavaScript)                    //
//                          March 2023 - Chubak Bidpaa                             //
/////////////////////////////////////////////////////////////////////////////////////
// MIT License                                                                     //
//                                                                                 //
// Copyright (c) 2023 Chubak Bidpaa                                                //
//                                                                                 //
// Permission is hereby granted, free of charge, to any person obtaining a copy    //
// of this software and associated documentation files (the "Software"), to deal   //
// in the Software without restriction, including without limitation the rights    //
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       //
// copies of the Software, and to permit persons to whom the Software is           //
// furnished to do so, subject to the following conditions:                        //
//                                                                                 //
// The above copyright notice and this permission notice shall be included in all  //
// copies or substantial portions of the Software.                                 //
//                                                                                 //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     //
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   //
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   //
// SOFTWARE.                                                                       //
/////////////////////////////////////////////////////////////////////////////////////

const libpoxh = require("./libpoxh.js");
const fs = require("fs");

const MAX_FLAG_SIZE = 24;
const MIN_FLAG_SIZE = 3;
const MIN_ARG_NUM = 2;
const NUM_ASCII = 128;
const LEN_WRONG_FLAGS = 28;
const FORMAT_MARKER = "%";
const FORMAT_STR = "s";
const FORMAT_DIGIT = "d";
const FORMAT_CHAR = "c";
const BENCHMARK_BYTE_INDEX = 94;

const FLAG_BENCHMARK = "^";
const FLAG_JOIN = "+";
const FLAG_EVERTHING = "*";
const FLAG_ALL_NON_DEC = "N";
const FLAG_ALL_DECIMAL = "D";
const FLAG_BYTES = "8";
const FLAG_WORDS = "4";
const FLAG_DOUBLES = "2";
const FLAG_QUAD = "1";
const FLAG_SEX = "g";
const FLAG_VIG = "v";
const FLAG_HEX = "h";
const FLAG_TET = "t";
const FLAG_DUO = "d";
const FLAG_OCT = "o";
const FLAG_SEN = "s";
const FLAG_BIN = "b";
const FLAG_NS = "9";
const FLAG_US = "6";
const FLAG_MS = "3";
const FLAG_SS = "5";
const FLAG_MM = "0";
const FLAG_HELP = "?";
const FLAG_DASH = "-";
const FLAG_NHEADER = "z";
const FLAG_ECHO = "e";

const MAX_HEX = 4;
const MAX_OCT = 5;
const MAX_BIN = 8;

const MAX_U8 = 255;

const FILE_PREFIX = "file=";
const FILE_PREFIX_LEN = 5;

const INT_PREFIX = "int=";
const INT_PREFIX_LEN = 4;

const HEX_PREFIX = "0x";
const BIN_PREFIX = "0b";
const OCT_PREFIX = "0o";
const BASE_PREFIX_NUM = 2;

const NS_TO_NS = 100;
const NS_TO_US = 1000;
const NS_TO_MS = 1000000;
const NS_TO_SS = 1000000000;
const NS_TO_MM = 60000000000;

const E_NOT_TRUNC_LEN = 4;

const WRONG_FLAGS = [
  ["G", "g"],
  ["V", "v"],
  ["O", "o"],
  ["T", "t"],
  ["S", "s"],
  ["H", "h"],
  ["n", "N"],
  ["W", "4"],
  ["w", "4"],
  ["q", "1"],
  ["Q", "1"],
  ["/", "?"],
  ["=", "+"],
  ["B", "b"],
  ["E", "*"],
  ["A", "*"],
  [">", "?"],
  ["&", "*"],
  ["r", "t"],
  ["y", "t"],
  ["f", "g"],
  ["x", "z"],
  ["Z", "z"],
  ["a", "z"],
  ["E", "e"],
  ["w", "e"],
  ["r", "e"],
  ["i", "e"],
];

function printf() {
  let message = arguments[0];
  if (arguments.length == 1) {
    process.stdout.write(message);
    return;
  }
  let finalMessage = "";
  let argsCursor = 1;
  let lenMessage = message.length;
  let currIndex = 0;
  let currChar, peekChar;
  while (currIndex <= lenMessage - 2) {
    currChar = message[currIndex];
    peekChar = message[currIndex + 1];

    if (currChar == FORMAT_MARKER) {
      if (
        peekChar == FORMAT_STR ||
        peekChar == FORMAT_DIGIT ||
        peekChar == FORMAT_CHAR
      ) {
        finalMessage += arguments[argsCursor];
        argsCursor += 1;
        currIndex += 2;
        continue;
      }
    }
    finalMessage += currChar;
    currIndex += 1;
  }
  finalMessage += message[lenMessage - 1];
  process.stdout.write(finalMessage);
}

const toENotation = (numIn, places) => {
  const num = Math.abs(numIn);
  if (num > 1.0) {
    const numStr = `${num}`;
    const indexOfPeriod = numStr.indexOf(".");
    const e = indexOfPeriod - 1;
    const firstDigit = numStr.charAt(0);
    let truncs = "";
    numStr
      .substring(1, places + 1)
      .split("")
      .forEach((char_at) => {
        if (char_at != ".") {
          truncs = `${truncs}${char_at}`;
        }
      });

    if (truncs.length < places) {
      const pad = "0".repeat(places - truncs.length);
      truncs = `${truncs}${pad}`;
    }

    const eStr = e > 9 ? `${e}` : `0${e}`;
    return `${firstDigit}.${truncs}e+${eStr}`;
  } else if (num > 0.0 && num < 1.0) {
    const numStr = `${num}`;
    let firstNonZeroIndex = 0;
    let truncs = "";
    let firstDigit = "\0";
    numStr.split("").forEach((c, i) => {
      if (c != "0" && c != "." && firstNonZeroIndex == 0) {
        firstNonZeroIndex = i;
        firstDigit = c;
        return;
      }
      if (firstNonZeroIndex != 0 && truncs.length < places) {
        truncs = `${truncs}${c}`;
      }
    });

    if (truncs.length < places) {
      const pad = "0".repeat(places - truncs.length);
      truncs = `${truncs}${pad}`;
    }

    const e = firstNonZeroIndex - 1;
    const eStr = e > 9 ? `${e}` : `0${e}`;
    return `${firstDigit}.${truncs}e-${eStr}`;
  } else {
    return `${num}`;
  }
};

const println = () => {
  process.stdout.write("\n");
};

const errorOut = (message) => {
  process.stderr.write("\n");
  process.stderr.write(message);
  process.stderr.write("\n");
  process.stderr.write(
    "\033[1;31mError occurred\033[0m. Please pass \033[1;34m-?-\033[0m to show help\n"
  );
  process.exit(1);
};

const printHelp = (execApp, execScript) => {
  printf(
    "\033[1;30;42mHelp | Chubak#7400 (Discord) | @bidpaafx (Telegram) | Chubakbidpaa[at]gmail[dot]com\033[0m\n"
  );
  println();
  printf("Examples \033[1m(flags go between two dashes!)\033[0m:\n");
  printf("%s %s -N82- myword1\n", execApp, execScript);
  printf("%s %s -*+^- mywod to be joined\n", execApp, execScript);
  printf("%s %s -Dhob- word1 word 2\n", execApp, execScript);
  printf(
    "%s %s -^^+- large seq  to join and  benchmark\n",
    execApp,
    execScript
  );
  printf(
    "wget -qO- www.example.com | xargs bash -c '%s %s -h+- $@'\n",
    execApp,
    execScript
  );
  printf(
    "If an argument stats with `%s`, it will lead to file read attempt, unless `%c` is passed\n",
    FILE_PREFIX,
    FLAG_JOIN
  );
  println();
  printf("\033[1;32mFlags:\033[0m\n");
  printf("\033[1;33m\t`%c`\033[0m: Echo argument\n", FLAG_ECHO);
  printf("\033[1;33m\t`%c`\033[0m: Don't print header message\n", FLAG_NHEADER);
  printf(
    "\033[1;33m\t`%c`\033[0m: Benchmark run (pass two to only show benchmark with all timestamps)\n",
    FLAG_BENCHMARK
  );
  printf(
    "\033[1;33m\t`%c`\033[0m: Join arguments with space (byte 32)\n",
    FLAG_JOIN
  );
  printf("\033[1;33m\t`%c`\033[0m: Print every digest\n", FLAG_EVERTHING);
  printf(
    "\033[1;33m\t`%c`\033[0m: Print every non-decimal digest\n",
    FLAG_ALL_NON_DEC
  );
  printf(
    "\033[1;33m\t`%c`\033[0m: Print every decimal digest\n",
    FLAG_ALL_DECIMAL
  );
  printf(
    "\033[1;33m\t`%c`\033[0m: Print bytes digest (eight unsigned 8-bit integers)\n",
    FLAG_BYTES
  );
  printf(
    "\033[1;33m\t`%c`\033[0m: Print words digest (four unsigned 16-bit integers)\n",
    FLAG_WORDS
  );
  printf(
    "\033[1;33m\t`%c`\033[0m: Print doubles digest (two unsigned 32-bit integers)\n",
    FLAG_DOUBLES
  );
  printf(
    "\033[1;33m\t`%c`\033[0m: Print quad digest (one unsigned 64-bit integer)\n",
    FLAG_QUAD
  );
  printf(
    "\033[1;33m\t`%c`\033[0m: Print sexagesimal digest (base sixty)\n",
    FLAG_SEX
  );
  printf(
    "\033[1;33m\t`%c`\033[0m: Print vigesimal digest (base twenty)\n",
    FLAG_VIG
  );
  printf(
    "\033[1;33m\t`%c`\033[0m: Print hexadecimal digest (base sixteen)\n",
    FLAG_HEX
  );
  printf(
    "\033[1;33m\t`%c`\033[0m: Print tetradecimal digest (base fourteen)\n",
    FLAG_TET
  );
  printf(
    "\033[1;33m\t`%c`\033[0m: Print duodecimal digest (base twelve)\n",
    FLAG_DUO
  );
  printf(
    "\033[1;33m\t`%c`\033[0m: Print octal digest (base eight)\n",
    FLAG_OCT
  );
  printf("\033[1;33m\t`%c`\033[0m: Print senary digest (base six)\n", FLAG_SEN);
  printf("\033[1;33m\t`%c`\033[0m: Print binary digest (base two)\n", FLAG_BIN);
  printf(
    "\033[1;33m\t`%c`\033[0m: Print total timestamp delta in nanoseconds\n",
    FLAG_NS
  );
  printf(
    "\033[1;33m\t`%c`\033[0m: Print total timestamp delta in mictoseconds\n",
    FLAG_US
  );
  printf(
    "\033[1;33m\t`%c`\033[0m: Print total timestamp delta in milliseconds\n",
    FLAG_MS
  );
  printf(
    "\033[1;33m\t`%c`\033[0m: Print total timestamp delta in seconds\n",
    FLAG_SS
  );
  printf(
    "\033[1;33m\t`%c`\033[0m: Print total timestamp delta in minutes\n",
    FLAG_MM
  );
  printf("\033[1;33m\t`%c`\033[0m: Print Help\n\n", FLAG_HELP);
  process.exit(1);
};

const checkForWrongFlags = (flags) => {
  let flag, wrong_flag, right_flag;
  const lenFlags = flags.length;
  for (let i = 0; i < lenFlags; i++) {
    flag = flags[i];
    for (let j = 0; j < LEN_WRONG_FLAGS; j++) {
      wrong_flag = WRONG_FLAGS[j][0];
      right_flag = WRONG_FLAGS[j][1];
      if (flag == wrong_flag) {
        printf("No flag for `%c`, perhaps you meant `%c`?", flag, right_flag);
        errorOut("Flag errror");
      }
    }
  }
};

const getExecOrScriptName = (argv0) => {
  let slashIndex = 0;
  let size = argv0.length;
  for (let i = size - 1; i > 0; i--) {
    if (argv0[i] == "/") {
      break;
    }
    slashIndex = i;
  }

  return argv0.substring(slashIndex, size);
};

const argHasFlag = (flagsArg, mustHave) => {
  let lenFlags = flagsArg.length;
  for (let i = 1; i < lenFlags - 1; i++) {
    if (flagsArg[i] == mustHave) {
      return true;
    }
  }
  return false;
};

const searchForFlagReoccurrances = (flagsArg) => {
  let occurranceArray = new Uint8Array(NUM_ASCII);
  let utf8Encode = new TextEncoder();
  let byteArray = utf8Encode.encode(flagsArg);
  const lenFlags = flagsArg.length;
  for (let i = 0; i < lenFlags; i++) {
    occurranceArray[byteArray[i]] += 1;
  }

  let ret = "\0";

  if (occurranceArray[BENCHMARK_BYTE_INDEX] == 2) {
    ret = "^";
  }

  if (occurranceArray[BENCHMARK_BYTE_INDEX] > 2) {
    errorOut("`^` can appear at most twice");
  }

  for (let i = 0; i < NUM_ASCII; i++) {
    if (i == BENCHMARK_BYTE_INDEX || i == 45) {
      continue;
    }
    if (occurranceArray[i] > 1) {
      ret = String.fromCharCode(i);
      return ret;
    }
  }

  return ret;
};

const validateFlags = (argv0, argv1, argv) => {
  const lenArgv = argv.length;

  if (lenArgv < MIN_ARG_NUM - 1) {
    errorOut("No flags passed");
  }
  const flagsArg = argv[0];
  const lenFlags = flagsArg.length;
  if (lenFlags < MIN_FLAG_SIZE || lenFlags > MAX_FLAG_SIZE) {
    errorOut("Length of the first argument must at least be 3 and at most 24");
  }

  if (flagsArg[0] != FLAG_DASH || flagsArg[lenFlags - 1] != FLAG_DASH) {
    errorOut("The flag argument must begin and end with `-`");
  }

  checkForWrongFlags(flagsArg, lenFlags);

  const execApp = getExecOrScriptName(argv0);
  const execScript = getExecOrScriptName(argv1);
  if (flagsArg == "-?-") {
    printHelp(execApp, execScript);
  }
  const helpPassed = argHasFlag(flagsArg, FLAG_HELP);
  if (helpPassed && lenFlags > MIN_FLAG_SIZE) {
    errorOut("You may not pass the `?` flag along with other flags");
  }

  const reoccurrance = searchForFlagReoccurrances(flagsArg);
  if (reoccurrance != "\0" && reoccurrance != FLAG_BENCHMARK) {
    printf("Flag `%c` appears twice", reoccurrance);
    errorOut("Only `^` can appear twice");
  }
  const doubleBenchmark = reoccurrance == FLAG_BENCHMARK;

  if (lenArgv < MIN_ARG_NUM) {
    errorOut("You must pass at least one argument to hash");
  }

  const allFlagsPassed = argHasFlag(flagsArg, FLAG_EVERTHING);
  const allFlagsDecPassed = argHasFlag(flagsArg, FLAG_ALL_DECIMAL);
  const allFlagsNonDecPassed = argHasFlag(flagsArg, FLAG_ALL_NON_DEC);
  const benchmarkHasPassed = argHasFlag(flagsArg, FLAG_BENCHMARK);

  for (let i = 1; i < lenFlags - 1; i++) {
    switch (flagsArg[i]) {
      case FLAG_BENCHMARK:
      case FLAG_JOIN:
      case FLAG_NHEADER:
      case FLAG_ECHO:
        continue;
      case FLAG_NS:
      case FLAG_US:
      case FLAG_MS:
      case FLAG_SS:
      case FLAG_MM:
        if (!benchmarkHasPassed) {
          errorOut(
            "When a timestamp flag has passed, `^` must be passed as well"
          );
        } else if (doubleBenchmark) {
          errorOut(
            "When double benchmark (`^^`) is passed, you may not pass a timestamp flag"
          );
        }
        continue;
      case FLAG_EVERTHING:
        if (allFlagsDecPassed || allFlagsNonDecPassed) {
          errorOut("You may not pass `*` when you have passed `N` or `D`");
        }
        continue;
      case FLAG_ALL_NON_DEC:
        if (allFlagsPassed) {
          errorOut("You may not pass `N` when `*` is passed");
        }
        continue;
      case FLAG_ALL_DECIMAL:
        if (allFlagsPassed) {
          errorOut("You may not pass `D` when `*` is passed");
        }
        continue;
      case FLAG_BYTES:
        if (allFlagsDecPassed || allFlagsPassed) {
          errorOut(
            "You may not pass a decimal digest flag when `*` or `D` is passed"
          );
        }
        continue;
      case FLAG_WORDS:
        if (allFlagsDecPassed || allFlagsPassed) {
          errorOut(
            "You may not pass a decimal digest flag when `*` or `D` is passed"
          );
        }
        continue;
      case FLAG_DOUBLES:
        if (allFlagsDecPassed || allFlagsPassed) {
          errorOut(
            "You may not pass a decimal digest flag when `*` or `D` is passed"
          );
        }
        continue;
      case FLAG_QUAD:
        if (allFlagsDecPassed || allFlagsPassed) {
          errorOut(
            "You may not pass a decimal digest flag when `*` or `D` is passed"
          );
        }
        continue;
      case FLAG_SEX:
        if (allFlagsNonDecPassed || allFlagsPassed) {
          errorOut(
            "You may not pass a non-decimal digest flag when `*` or `N` is passed"
          );
        }
        continue;
      case FLAG_VIG:
        if (allFlagsNonDecPassed || allFlagsPassed) {
          errorOut(
            "You may not pass a non-decimal digest flag when `*` or `N` is passed"
          );
        }
        continue;
      case FLAG_HEX:
        if (allFlagsNonDecPassed || allFlagsPassed) {
          errorOut(
            "You may not pass a non-decimal digest flag when `*` or `N` is passed"
          );
        }
        continue;
      case FLAG_TET:
        if (allFlagsNonDecPassed || allFlagsPassed) {
          errorOut(
            "You may not pass a non-decimal digest flag when `*` or `N` is passed"
          );
        }
        continue;
      case FLAG_DUO:
        if (allFlagsNonDecPassed || allFlagsPassed) {
          errorOut(
            "You may not pass a non-decimal digest flag when `*` or `N` is passed"
          );
        }
        continue;
      case FLAG_OCT:
        if (allFlagsNonDecPassed || allFlagsPassed) {
          errorOut(
            "You may not pass a non-decimal digest flag when `*` or `N` is passed"
          );
        }
        continue;
      case FLAG_SEN:
        if (allFlagsNonDecPassed || allFlagsPassed) {
          errorOut(
            "You may not pass a non-decimal digest flag when `*` or `N` is passed"
          );
        }
        continue;
      case FLAG_BIN:
        if (allFlagsNonDecPassed || allFlagsPassed) {
          errorOut(
            "You may not pass a non-decimal digest flag when `*` or `N` is passed"
          );
        }
        continue;
      case FLAG_HELP:
        if (lenFlags > MIN_FLAG_SIZE) {
          errorOut("You may not pass the `?` flag along with other flags");
        }
      case FLAG_DASH:
        errorOut(
          "You may not use `-` in the first argument other than in the first, and the last letter"
        );
      default:
        errorOut("Unknown flag detected!");
    }
  }
};

const getTimeInNS = () => {
  return Number(Date.now() + String(process.hrtime()[1]).slice(3));
};

const convertTime = (time, divisor) => {
  return toENotation(time / divisor, E_NOT_TRUNC_LEN);
};

const allAreFalse = (arr) => {
  for (let i = 0; i < arr.length; i++) {
    if (arr[i] == true) {
      return false;
    }
  }
  return true;
};

const printHashes = (hashes, flags, totalTime) => {
  const lenFlags = flags.length;
  const lenHashes = hashes.length;
  const reoccurrance = searchForFlagReoccurrances(flags, lenFlags);
  const doubleBenchmark = reoccurrance == FLAG_BENCHMARK;

  if (argHasFlag(flags, FLAG_BENCHMARK)) {
    printf("| %d Message(s) ||", lenHashes);
    let hasPrinted = false;
    if (argHasFlag(flags, FLAG_NS) || doubleBenchmark) {
      printf(" %sns |", convertTime(totalTime, NS_TO_NS));
      hasPrinted = true;
    }
    if (argHasFlag(flags, FLAG_US) || doubleBenchmark) {
      printf(" %sus |", convertTime(totalTime, NS_TO_US));
      hasPrinted = true;
    }
    if (argHasFlag(flags, FLAG_MS) || doubleBenchmark) {
      printf(" %sms |", convertTime(totalTime, NS_TO_MS));
      hasPrinted = true;
    }
    if (argHasFlag(flags, FLAG_SS) || doubleBenchmark) {
      printf(" %ss |", convertTime(totalTime, NS_TO_SS));
      hasPrinted = true;
    }
    if (argHasFlag(flags, FLAG_MM) || doubleBenchmark) {
      printf(" %sm |", convertTime(totalTime, NS_TO_MM));
      hasPrinted = true;
    }
    if (!hasPrinted) {
      printf(" %sus |", convertTime(totalTime, NS_TO_US));
    }
    println();
  }

  if (doubleBenchmark) {
    println();
    process.exit(0);
  }

  const everything = argHasFlag(flags, FLAG_EVERTHING);
  const allFlagsDecimal = argHasFlag(flags, FLAG_ALL_DECIMAL);
  const allFlagsNonDecimal = argHasFlag(flags, FLAG_ALL_NON_DEC);
  const byte = argHasFlag(flags, FLAG_BYTES);
  const word = argHasFlag(flags, FLAG_WORDS);
  const dub = argHasFlag(flags, FLAG_DOUBLES);
  const quad = argHasFlag(flags, FLAG_QUAD);
  const sex = argHasFlag(flags, FLAG_SEX);
  const vig = argHasFlag(flags, FLAG_VIG);
  const hex = argHasFlag(flags, FLAG_HEX);
  const tet = argHasFlag(flags, FLAG_TET);
  const duo = argHasFlag(flags, FLAG_DUO);
  const oct = argHasFlag(flags, FLAG_OCT);
  const sen = argHasFlag(flags, FLAG_SEN);
  const bin = argHasFlag(flags, FLAG_BIN);

  const allFalse = allAreFalse([
    everything,
    allFlagsDecimal,
    allFlagsNonDecimal,
    byte,
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
  ]);

  if (allFalse) {
    printf("You had not specfied any digests to be printed\n");
    exit(0);
  }

  for (let i = 0; i < lenHashes; i++) {
    if (everything || allFlagsDecimal || byte) {
      printf(
        "Bytes: U8[%d, %d, %d, %d, %d, %d, %d, %d]\n",
        hashes[i].bytes[0],
        hashes[i].bytes[1],
        hashes[i].bytes[2],
        hashes[i].bytes[3],
        hashes[i].bytes[4],
        hashes[i].bytes[5],
        hashes[i].bytes[6],
        hashes[i].bytes[7]
      );
    }
    if (everything || allFlagsDecimal || word) {
      printf(
        "Words: U16[%d, %d, %d, %d]\n",
        hashes[i].words[0],
        hashes[i].words[1],
        hashes[i].words[2],
        hashes[i].words[3]
      );
    }
    if (everything || allFlagsDecimal || dub) {
      printf(
        "Doubles: U32[%d, %d]\n",
        hashes[i].doubles[0],
        hashes[i].doubles[1]
      );
    }
    if (everything || allFlagsDecimal || quad) {
      printf("Quad: U64[%d]\n", hashes[i].quad);
    }
    if (everything || allFlagsNonDecimal || sex) {
      printf("Sexdigest: %s\n", hashes[i].sexdigest);
    }
    if (everything || allFlagsNonDecimal || vig) {
      printf("Vigdigest: %s\n", hashes[i].vigdigest);
    }
    if (everything || allFlagsNonDecimal || hex) {
      printf("Hexdigest: %s\n", hashes[i].hexdigest);
    }
    if (everything || allFlagsNonDecimal || tet) {
      printf("Tetdigest: %s\n", hashes[i].tetdigest);
    }
    if (everything || allFlagsNonDecimal || duo) {
      printf("Duodigest: %s\n", hashes[i].duodigest);
    }
    if (everything || allFlagsNonDecimal || oct) {
      printf("Octdigest: %s\n", hashes[i].octdigest);
    }
    if (everything || allFlagsNonDecimal || sen) {
      printf("Sendgiest: %s\n", hashes[i].sendigest);
    }
    if (everything || allFlagsNonDecimal || bin) {
      printf("Bindigest: %s\n", hashes[i].bindigest);
    }
    printf("\033[2m----\033[0m\n");
  }
};

const newNullArray = (size) => {
  let ret = [];
  for (let i = 0; i < size; i++) {
    ret.push(null);
  }
  return ret;
};

const assertFile = (arg) => {
  return arg.length > FILE_PREFIX_LEN && arg.startsWith(FILE_PREFIX);
};

const assertInt = (arg) => {
  return arg.length > INT_PREFIX_LEN && arg.startsWith(INT_PREFIX);
};

const toInt = (arg) => {
  const split = arg.split(",");
  let result = new Uint8Array(split.length);
  split.forEach((num, i) => {
    const base = num.substring(0, BASE_PREFIX_NUM);
    const sansBase = num.substring(BASE_PREFIX_NUM);
    switch (base) {
      case BIN_PREFIX:
        if (sansBase.length > MAX_BIN) {
          errorOut("Size of binary number should not exceed 8");
        }
        result[i] = parseInt(sansBase, 2);
        break;
      case OCT_PREFIX:
        if (sansBase.length > MAX_OCT) {
          errorOut("Size of octal number should not exceed 5");
        }
        result[i] = parseInt(sansBase, 8);
        break;
      case HEX_PREFIX:
        if (sansBase.length > MAX_HEX) {
          errorOut("Size of hexadecimal number should not exceed 2");
        }
        result[i] = parseInt(sansBase, 16);
        break;
      default:
        if (parseInt(num) == NaN) {
          errorOut(
            "With 'int=' prefix you must pass byte-sized integers in base 16, 8, 10 and 2"
          );
        }
        const dec = parseInt(num);
        const u16Container = new Uint16Array([dec]);
        if (u16Container[0] > MAX_U8) {
          errorOut("Given integer must be byte-sized (0-255)");
        }
        result[i] = dec;
        break;
    }
  });
  return result;
};

const isRegularFile = async (fpath) => {
  try {
    const stat = fs.lstatSync(fpath);
    if (stat.isDirectory()) {
      return false;
    }
  } catch (e) {
    if (e.code == "ENOENT") {
      return false;
    }
    errorOut("Unknown error with file input");
  }
  return true;
};

const readGivenFile = async (arg) => {
  const isRegular = await isRegularFile(arg);
  if (isRegular) {
    return fs.readFileSync(arg, "utf8");
  } else {
    errorOut(
      "Specfied file does not exist or is a directory. Pass `+` with only one argument to ignore"
    );
  }
};

const joinArgs = (args) => {
  let joined = args[0];
  args.slice(1).forEach((arg) => {
      joined = `${joined} ${arg}`;
    });
  return joined;
};

const stringToU8Array = (str) => {
  let utf8Encode = new TextEncoder();
  byteArray = utf8Encode.encode(str);
  return byteArray;
};

const processArg = async (arg) => {
  if (!assertFile(arg) && !assertInt(arg)) {
    return stringToU8Array(arg);
  } else if (assertInt(arg)) {
    return toInt(arg.substring(INT_PREFIX_LEN));
  }
  return stringToU8Array(await readGivenFile(arg.substring(FILE_PREFIX_LEN)));
};

const main = async (argv0, argv1, argv) => {
  validateFlags(argv0, argv1, argv);
  const flagsArg = argv[0];

  if (!argHasFlag(flagsArg, FLAG_NHEADER)) {
    printf(
      "\033[1;30;47m   PoxHashRunner   |  JavaScript   |  March 2023 - Chubak Bidpaa  |  MIT  \033[0m\n"
    );
  }

  const echoArg = argHasFlag(flagsArg, FLAG_ECHO);
  const lenHashes = argv.length - 1;

  let hashes = newNullArray(lenHashes);
  let totalTime, t1, t2;
  totalTime = 0;

  if (argHasFlag(flagsArg, FLAG_JOIN)) {
    const argsJoined = joinArgs(argv.slice(1));
    if (echoArg) {
      printf("Joined Args: \n`%s`\n", argsJoined);
    }
    const u8Array = stringToU8Array(argsJoined);
    t1 = getTimeInNS();
    hashes[0] = libpoxh.poxHash(u8Array);
    t2 = getTimeInNS();
    printHashes(hashes.slice(0, 1), flagsArg, t2 - t1);
  } else {
    let cursor = 0;
    let processedArg = "";
    for (let i = 1; i <= lenHashes; i++) {
      if (echoArg) {
        printf("Arg %d: %s\n", i - 1, argv[i]);
      }
      processedArg = await processArg(argv[i]);
      t1 = getTimeInNS();
      hashes[cursor] = libpoxh.poxHash(processedArg);
      t2 = getTimeInNS();
      cursor += 1;
      totalTime += t2 - t1;
    }
    printHashes(hashes, flagsArg, totalTime);
  }
};

const argv0 = process.argv[0];
const argv1 = process.argv[1];
const argv = process.argv.slice(2);
(async () => {
  await main(argv0, argv1, argv);
})();
