const libpoxh = require("./libpoxh.js");

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
const FLAG_HELP = "?";
const FLAG_DASH = "-";

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
  ["3", "2"],
  ["5", "4"],
  ["6", "^"],
  ["7", "8"],
  ["9", "8"],
  ["0", "1"],
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
  ["x", "h"],
];

function printFormatted() {
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

const printLn = () => {
  process.stdout.write("\n");
};

const errorOut = (message) => {
  printLn();
  printFormatted(message);
  printLn();
  printFormatted(
    "\033[1;31mError occurred\033[0m. Please pass \033[1;34m-?-\033[0m to show help\n"
  );
  process.exit(1);
};

const printHelp = (execApp, execScript) => {
  printFormatted(
    "\033[1;42mHelp | Chubak#7400 (Discord) | @bidpaafx (Telegram) | Chubakbidpaa[at]gmail\033[0m\n"
  );
  printLn();
  printFormatted("Examples \033[1m(flags go between two dashes!)\033[0m:\n");
  printFormatted("%s %s -N82- myword1\n", execApp, execScript);
  printFormatted("%s %s -*+^- mywod to be joined\n", execApp, execScript);
  printFormatted("%s %s -Dhob- word1 word 2\n", execApp, execScript);
  printFormatted(
    "%s %s -^^+- large seq  to join and  benchmark\n",
    execApp,
    execScript
  );
  printFormatted(
    "wget -qO- www.example.com | xargs bash -c '%s %s -h+- $@'\n",
    execApp,
    execScript
  );
  printLn();
  printFormatted("\033[1;32mFlags:\033[0m\n");
  printFormatted(
    "\033[1;35m\t`^`\033[0m: Benchmark run (pass two to only show benchmark)\n"
  );
  printFormatted(
    "\033[1;35m\t`+`\033[0m: Join arguments with space (byte 32)\n"
  );
  printFormatted("\033[1;35m\t`*`\033[0m: Print every digest\n");
  printFormatted("\033[1;35m\t`N`\033[0m: Print every non-decimal digest\n");
  printFormatted("\033[1;35m\t`D`\033[0m: Print every decimal digest\n");
  printFormatted(
    "\033[1;35m\t`8`\033[0m: Print bytes digest (eight unsigned 8-bit integers)\n"
  );
  printFormatted(
    "\033[1;35m\t`4`\033[0m: Print words digest (four unsigned 16-bit integers)\n"
  );
  printFormatted(
    "\033[1;35m\t`2`\033[0m: Print doubles digest (two unsigned 32-bit integers)\n"
  );
  printFormatted(
    "\033[1;35m\t`1`\033[0m: Print quad digest (one unsigned 64-bit integer)\n"
  );
  printFormatted(
    "\033[1;35m\t`g`\033[0m: Print sexagesimal digest (base sixty)\n"
  );
  printFormatted(
    "\033[1;35m\t`v`\033[0m: Print vigesimal digest (base twenty)\n"
  );
  printFormatted(
    "\033[1;35m\t`h`\033[0m: Print hexadecimal digest (base sixteen)\n"
  );
  printFormatted(
    "\033[1;35m\t`t`\033[0m: Print tetradecimal digest (base fourteen)\n"
  );
  printFormatted(
    "\033[1;35m\t`d`\033[0m: Print duodecimal digest (base twelve)\n"
  );
  printFormatted("\033[1;35m\t`o`\033[0m: Print octal digest (base eight)\n");
  printFormatted("\033[1;35m\t`s`\033[0m: Print senary digest (base six)\n");
  printFormatted("\033[1;35m\t`b`\033[0m: Print binary digest (base two)\n");
  printFormatted("\033[1;35m\t`?`\033[0m: Print Help\n\n");
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
        printFormatted(
          "No flag for `%c`, perhaps you meant `%c`?",
          flag,
          right_flag
        );
        errorOut("Flag erreror");
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
    printFormatted("Flag `%c` appears twice", reoccurrance);
    errorOut("Only `^` can appear twice");
  }

  if (lenArgv < MIN_ARG_NUM) {
    errorOut("You must pass at least one argument to hash");
  }

  const allFlagsPassed = argHasFlag(flagsArg, FLAG_EVERTHING);
  const allFlagsDecPassed = argHasFlag(flagsArg, FLAG_ALL_DECIMAL);
  const allFlagsNonDecPassed = argHasFlag(flagsArg, FLAG_ALL_NON_DEC);

  for (let i = 1; i < lenFlags - 1; i++) {
    switch (flagsArg[i]) {
      case FLAG_BENCHMARK:
        continue;
      case FLAG_JOIN:
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
        s;
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

const getTimeInUS = () => {
  return Number(Date.now() + String(process.hrtime()[1]).slice(3, 6));
};

const allAreFalse = (arr) => {
  for (let i = 0; i < arr.length; i++) {
    if (arr[i] == true) {
      return false;
    }
  }
  return true;
};

const printHashes = (hashes, flags, totalTime, joined) => {
  const lenFlags = flags.length;
  const lenHashes = hashes.length;
  if (argHasFlag(flags, FLAG_BENCHMARK))
    printFormatted(
      "Total time for hashing %d bytestring(s): %dus \n",
      lenHashes,
      totalTime
    );

  const reoccurrance = searchForFlagReoccurrances(flags, lenFlags);
  if (reoccurrance == FLAG_BENCHMARK) {
    printLn();
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
  const duo = argHasFlag(flags, FLAG_TET);
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
    printFormatted("You had not specfied any digests to be printed\n");
    exit(0);
  }

  for (let i = 0; i < lenHashes; i++) {
    printFormatted("----\n");
    printFormatted("Requested digests for bytestring #%d%s\n", i + 1, joined);
    if (everything || allFlagsDecimal || byte) {
      printFormatted(
        "\tBytes: U8[%d, %d, %d, %d, %d, %d, %d, %d]\n",
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
      printFormatted(
        "\tWords: U16[%d, %d, %d, %d]\n",
        hashes[i].words[0],
        hashes[i].words[1],
        hashes[i].words[2],
        hashes[i].words[3]
      );
    }
    if (everything || allFlagsDecimal || dub) {
      printFormatted(
        "\tdoubles: U32[%d, %d]\n",
        hashes[i].doubles[0],
        hashes[i].doubles[1]
      );
    }
    if (everything || allFlagsDecimal || quad) {
      printFormatted("\tQuad: U64[%d]\n", hashes[i].quad);
    }
    if (everything || allFlagsNonDecimal || sex) {
      printFormatted("\tSexdigest: %s\n", hashes[i].sexdigest);
    }
    if (everything || allFlagsNonDecimal || vig) {
      printFormatted("\tVigdigest: %s\n", hashes[i].vigdigest);
    }
    if (everything || allFlagsNonDecimal || hex) {
      printFormatted("\tHexdigest: %s\n", hashes[i].hexdigest);
    }
    if (everything || allFlagsNonDecimal || tet) {
      printFormatted("\tTetdigest: %s\n", hashes[i].tetdigest);
    }
    if (everything || allFlagsNonDecimal || duo) {
      printFormatted("\tDuodigest: %s\n", hashes[i].duodigest);
    }
    if (everything || allFlagsNonDecimal || oct) {
      printFormatted("\tOctdigest: %s\n", hashes[i].octdigest);
    }
    if (everything || allFlagsNonDecimal || sen) {
      printFormatted("\tSendgiest: %s\n", hashes[i].sendigest);
    }
    if (everything || allFlagsNonDecimal || bin) {
      printFormatted("\tBindigest: %s\n", hashes[i].bindigest);
    }
  }
  printFormatted(
    "\nFinished run for PoxHash example code (JS implementation)\n"
  );
};

const newNullArray = (size) => {
  let ret = [];
  for (let i = 0; i < size; i++) {
    ret.push(null);
  }
  return ret;
};

const joinArgs = (args) => {
  return args.join(" ");
};

const main = (argv0, argv1, argv) => {
  printFormatted(
    "\033[1;47mPoxHash   |  JavaScript   |  March 2023 - Chubak Bidpaa  |  GPLv3  \033[0m\n"
  );
  validateFlags(argv0, argv1, argv);
  const flagsArg = argv[0];
  const lenHashes = argv.length - 1;

  let hashes = newNullArray(lenHashes);
  let totalTime, t1, t2;
  totalTime = 0;

  if (argHasFlag(flagsArg, FLAG_JOIN)) {
    const argsJoined = joinArgs(argv.slice(1));
    t1 = getTimeInUS();
    hashes[0] = libpoxh.poxHash(argsJoined);
    t2 = getTimeInUS();
    printHashes(hashes.slice(0, 1), flagsArg, t2 - t1, " (joined arguments):");
  } else {
    let cursor = 0;
    for (let i = 1; i <= lenHashes; i++) {
      t1 = getTimeInUS();
      hashes[cursor] = libpoxh.poxHash(argv[i]);
      t2 = getTimeInUS();
      cursor += 1;
      totalTime += t2 - t1;
    }
    printHashes(hashes, flagsArg, totalTime, ":");
  }
};

const argv0 = process.argv[0];
const argv1 = process.argv[1];
const argv = process.argv.slice(2);
main(argv0, argv1, argv);
