#####################################################################################
##                       Runner for libpoxh.py (PoxHash Python)                    ##
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

import sys
from array import array
from pathlib import Path
from time import time_ns

from libpoxh import PoxDigest, pox_hash

MAX_FLAG_SIZE = 24
MIN_FLAG_SIZE = 3
MIN_ARG_NUM = 1
NUM_ASCII = 128
FORMAT_MARKER = '%'
FORMAT_STR = 's'
FORMAT_DIGIT = 'd'
FORMAT_CHAR = 'c'
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
FLAG_NS = '9'
FLAG_US = '6'
FLAG_MS = '3'
FLAG_SS = '5'
FLAG_MM = '0'
FLAG_HELP = '?'
FLAG_DASH = '-'
FLAG_NHEADER = 'z'
FLAG_ECHO = 'e'

SKIPPER_FLAGS = [FLAG_BENCHMARK, FLAG_JOIN, FLAG_NHEADER, FLAG_ECHO]
TIMESTAMP_FLAGS = [FLAG_NS, FLAG_US, FLAG_MS, FLAG_SS, FLAG_MM]

FILE_PREFIX = "file="
FILE_PREFIX_LEN = 5

INT_PREFIX = "int="
INT_PREFIX_LEN = 4

MAX_HEX = 2
MAX_OCT = 3
MAX_BIN = 8

MAX_U8 = 255

HEX_PREFIX = "0x"
BIN_PREFIX = "0b"
OCT_PREFIX = "0o"
BASE_PREFIX_NUM = 2

NS_TO_NS = 100
NS_TO_US = 1000
NS_TO_MS = 1000000
NS_TO_SS = 1000000000
NS_TO_MM = 60000000000

E_NOT_TRUNC_LEN = 4

WRONG_FLAGS = [
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
    ('x', 'z'),
    ('Z', 'z'),
    ('a', 'z'),
    ('E', 'e'),
    ('w', 'e'),
    ('r', 'e'),
    ('i', 'e'),
]


def to_e_notation(num_in: float, places: int) -> str:
    num = abs(num_in)
    if num > 1.0:
        num_str = str(num)
        index_of_period = num_str.index(".")
        e = index_of_period - 1
        first_digit = num_str[0]
        truncs = ""
        for c in num_str[1:places + 1]:
            if c == '.':
                continue
            truncs = f"{truncs}{c}"

        if len(truncs) < places:
            pad = "0" * (places - len(truncs))
            truncs = f"{truncs}{pad}"

        e_str = f"{e}" if e > 9 else f"0{e}"
        return f"{first_digit}.{truncs}e+{e_str}"
    elif 0.0 < num < 1.0:
        num_str = str(num)
        first_non_zero_index = 0
        truncs = ""
        first_digit = '\0'
        for i, c in enumerate(num_str):
            if c != '0' and c != '.' and first_non_zero_index == 0:
                first_non_zero_index = i
                first_digit = c
                continue

            if first_non_zero_index != 0 and len(truncs) < places:
                truncs = f"{truncs}{c}"

        if len(truncs) < places:
            pad = "0" * (places - len(truncs))
            truncs = f"{truncs}{pad}"

        e = first_non_zero_index - 1
        e_str = f"{e}" if e > 9 else f"0{e}"
        return f"{first_digit}.{truncs}e-{e_str}"
    else:
        return f"{num}"


def printf(*argc, **_) -> None:
    message = argc[0]
    len_message = len(message)
    finalMessage = ""
    currChar = ''
    peekChar = ''
    index = 0
    cursor = 1

    while index < len_message - 1:
        currChar = message[index]
        peekChar = message[index + 1]
        if currChar == FORMAT_MARKER:
            if peekChar == FORMAT_CHAR or peekChar == FORMAT_DIGIT or peekChar == FORMAT_STR:
                finalMessage += str(argc[cursor])
                cursor += 1
                index += 2
                continue
        finalMessage += currChar
        index += 1
    finalMessage += message[-1]
    sys.stdout.write(finalMessage)


def println() -> None:
    sys.stdout.write("\n")


def error_out(message: str) -> None:
    sys.stderr.write("\n")
    sys.stderr.write(message)
    sys.stderr.write("\n")
    sys.stderr.write(
        "\033[1;31mError occurred\033[0m. Please pass \033[134m-?-\033[0m to show help\n"
    )
    exit(1)


def printHelp(exec_name: str, script_name: str) -> None:
    printf(
        "\033[1;30;42mHelp | Chubak#7400 (Discord) | @bidpaafx (Telegram) | Chubakbidpaa[at]gmail[dot]com\033[0m\n"
    )
    println()
    printf("Examples \033[1m(flags go between two dashes!)\033[0m:\n")
    printf("%s %s -N82- myword1\n", exec_name, script_name)
    printf("%s %s -*+^- mywod to be joined\n", exec_name, script_name)
    printf("%s %s -Dhob- word1 word 2\n", exec_name, script_name)
    printf("%s %s -^^+- large seq  to join and  benchmark\n", exec_name,
           script_name)
    printf("wget -qO- www.example.com | xargs bash -c '%s %s -h+- $@'\n",
           exec_name, script_name)
    printf(
        "If an argument stats with `%s`, it will lead to file read attempt, unless `%c` is passed\n",
        FILE_PREFIX, FLAG_JOIN)
    printf(
        "If an argument stats with `%s`, it will parse the int, values 0-255, prefixes `0b`, `0o` and `0x` for bin, oct and hex and none for decimal apply\n",
        INT_PREFIX)
    println()
    printf("\033[1;32mFlags:\033[0m\n")
    printf("\033[1;33m\t`%c`\033[0m: Echo argument\n", FLAG_ECHO)
    printf("\033[1;33m\t`%c`\033[0m: Don't print header message\n",
           FLAG_NHEADER)
    printf(
        "\033[1;33m\t`%c`\033[0m: Benchmark run (pass two to only show benchmark with all timestamps)\n",
        FLAG_BENCHMARK)
    printf("\033[1;33m\t`%c`\033[0m: Join arguments with space (byte 32)\n",
           FLAG_JOIN)
    printf("\033[1;33m\t`%c`\033[0m: Print every digest\n", FLAG_EVERTHING)
    printf("\033[1;33m\t`%c`\033[0m: Print every non-decimal digest\n",
           FLAG_ALL_NON_DEC)
    printf("\033[1;33m\t`%c`\033[0m: Print every decimal digest\n",
           FLAG_ALL_DECIMAL)
    printf(
        "\033[1;33m\t`%c`\033[0m: Print bytes digest (eight unsigned 8-bit integers)\n",
        FLAG_BYTES)
    printf(
        "\033[1;33m\t`%c`\033[0m: Print words digest (four unsigned 16-bit integers)\n",
        FLAG_WORDS)
    printf(
        "\033[1;33m\t`%c`\033[0m: Print doubles digest (two unsigned 32-bit integers)\n",
        FLAG_DOUBLES)
    printf(
        "\033[1;33m\t`%c`\033[0m: Print quad digest (one unsigned 64-bit integer)\n",
        FLAG_QUAD)
    printf("\033[1;33m\t`%c`\033[0m: Print sexagesimal digest (base sixty)\n",
           FLAG_SEX)
    printf("\033[1;33m\t`%c`\033[0m: Print vigesimal digest (base twenty)\n",
           FLAG_VIG)
    printf(
        "\033[1;33m\t`%c`\033[0m: Print hexadecimal digest (base sixteen)\n",
        FLAG_HEX)
    printf(
        "\033[1;33m\t`%c`\033[0m: Print tetradecimal digest (base fourteen)\n",
        FLAG_TET)
    printf("\033[1;33m\t`%c`\033[0m: Print duodecimal digest (base twelve)\n",
           FLAG_DUO)
    printf("\033[1;33m\t`%c`\033[0m: Print octal digest (base eight)\n",
           FLAG_OCT)
    printf("\033[1;33m\t`%c`\033[0m: Print senary digest (base six)\n",
           FLAG_SEN)
    printf("\033[1;33m\t`%c`\033[0m: Print binary digest (base two)\n",
           FLAG_BIN)
    printf(
        "\033[1;33m\t`%c`\033[0m: Print total timestamp delta in nanoseconds\n",
        FLAG_NS)
    printf(
        "\033[1;33m\t`%c`\033[0m: Print total timestamp delta in mictoseconds\n",
        FLAG_US)
    printf(
        "\033[1;33m\t`%c`\033[0m: Print total timestamp delta in milliseconds\n",
        FLAG_MS)
    printf("\033[1;33m\t`%c`\033[0m: Print total timestamp delta in seconds\n",
           FLAG_SS)
    printf("\033[1;33m\t`%c`\033[0m: Print total timestamp delta in minutes\n",
           FLAG_MM)
    printf("\033[1;33m\t`%c`\033[0m: Print Help\n\n", FLAG_HELP)
    exit(1)


def get_script_and_exec_name(path: str) -> str:
    return path.split("/")[-1]


def check_for_wrong_flags(flags: str) -> None:
    for flag in flags:
        for (wrong_flag, right_flag) in WRONG_FLAGS:
            if flag == wrong_flag:
                printf("No flag for `%c`, perhaps you meant `%c`?", flag,
                       right_flag)
                error_out("Flag errror")


def arg_has_flag(flags: str, must_have: str) -> bool:
    for flag in flags:
        if flag == must_have:
            return True
    return False


def search_for_flag_occurrances(flags: str) -> str:
    count_bm = flags.count(FLAG_BENCHMARK)
    if count_bm == 2:
        return FLAG_BENCHMARK
    if count_bm > 2:
        error_out("`^` can appear at most twice")

    for flg in flags:
        if flags.count(flg) > 1:
            return flg

    return '\0'


def validate_flags(exec: str, argv: list[str]) -> None:
    len_argv = len(argv)
    flags_arg = argv[0]
    len_flags = len(flags_arg)
    script_name = get_script_and_exec_name(exec)

    if len_argv < MIN_ARG_NUM:
        error_out("No flags passed")

    if len_flags < MIN_FLAG_SIZE or len_flags > MAX_FLAG_SIZE:
        error_out(
            "Length of the first argument must at least be 3 and at most 24")

    if not (flags_arg.startswith(FLAG_DASH) and flags_arg.endswith(FLAG_DASH)):
        error_out("The flag argument must begin and end with `-`")

    check_for_wrong_flags(flags_arg)
    exec_name = get_script_and_exec_name(sys.executable)
    if flags_arg == "-?-":
        printHelp(exec_name, script_name)

    helpPassed = arg_has_flag(flags_arg, FLAG_HELP)
    if helpPassed and len_flags > MIN_FLAG_SIZE:
        error_out("You may not pass the `?` flag along with other flags")

    reoccurrance = search_for_flag_occurrances(flags_arg[1:-1])
    if reoccurrance != '\0' and reoccurrance != FLAG_BENCHMARK:
        printf("Flag `%c` appears twice", reoccurrance)
        error_out("Only `^` can appear twice")
    double_benchmark = reoccurrance == FLAG_BENCHMARK

    if len_argv < MIN_ARG_NUM + 1:
        error_out("You must pass at least one argument to hash")

    all_flags_passed = arg_has_flag(flags_arg, FLAG_EVERTHING)
    all_flags_dec_passed = arg_has_flag(flags_arg, FLAG_ALL_DECIMAL)
    all_flags_non_dec_passed = arg_has_flag(flags_arg, FLAG_ALL_NON_DEC)
    benchmark_has_passed = arg_has_flag(flags_arg, FLAG_BENCHMARK)

    for flag in flags_arg[1:-1]:
        if any([flag == f for f in SKIPPER_FLAGS]):
            continue
        if any([flag == f for f in TIMESTAMP_FLAGS]):
            if not benchmark_has_passed:
                error_out(
                    "When a timestamp flag has passed, `^` must be passed as well"
                )
            elif double_benchmark:
                error_out(
                    "When double benchmark (`^^`) is passed, you may not pass a timestamp flag"
                )
            continue
        if flag == FLAG_EVERTHING:
            if all_flags_dec_passed or all_flags_non_dec_passed:
                error_out(
                    "You may not pass `*` when you have passed `N` or `D`")
            continue
        if flag == FLAG_ALL_NON_DEC:
            if all_flags_passed:
                error_out("You may not pass `N` when `*` is passed")
            continue
        if flag == FLAG_ALL_DECIMAL:
            if all_flags_passed:
                error_out("You may not pass `D` when `*` is passed")
            continue
        if flag == FLAG_BYTES:
            if all_flags_dec_passed or all_flags_passed:
                error_out(
                    "You may not pass a decimal digest flag when `*` or `D` is passed"
                )
            continue
        if flag == FLAG_WORDS:
            if all_flags_dec_passed or all_flags_passed:
                error_out(
                    "You may not pass a decimal digest flag when `*` or `D` is passed"
                )
            continue
        if flag == FLAG_DOUBLES:
            if all_flags_dec_passed or all_flags_passed:
                error_out(
                    "You may not pass a decimal digest flag when `*` or `D` is passed"
                )
            continue
        if flag == FLAG_QUAD:
            if all_flags_dec_passed or all_flags_passed:
                error_out(
                    "You may not pass a decimal digest flag when `*` or `D` is passed"
                )
            continue
        if flag == FLAG_SEX:
            if all_flags_non_dec_passed or all_flags_passed:
                error_out(
                    "You may not pass a non-decimal digest flag when `*` or `N` is passed"
                )
            continue
        if flag == FLAG_VIG:
            if all_flags_non_dec_passed or all_flags_passed:
                error_out(
                    "You may not pass a non-decimal digest flag when `*` or `N` is passed"
                )
            continue
        if flag == FLAG_HEX:
            if all_flags_non_dec_passed or all_flags_passed:
                error_out(
                    "You may not pass a non-decimal digest flag when `*` or `N` is passed"
                )
            continue
        if flag == FLAG_TET:
            if all_flags_non_dec_passed or all_flags_passed:
                error_out(
                    "You may not pass a non-decimal digest flag when `*` or `N` is passed"
                )
            continue
        if flag == FLAG_DUO:
            if all_flags_non_dec_passed or all_flags_passed:
                error_out(
                    "You may not pass a non-decimal digest flag when `*` or `N` is passed"
                )
            continue
        if flag == FLAG_OCT:
            if all_flags_non_dec_passed or all_flags_passed:
                error_out(
                    "You may not pass a non-decimal digest flag when `*` or `N` is passed"
                )
            continue
        if flag == FLAG_SEN:
            if all_flags_non_dec_passed or all_flags_passed:
                error_out(
                    "You may not pass a non-decimal digest flag when `*` or `N` is passed"
                )
            continue
        if flag == FLAG_BIN:
            if all_flags_non_dec_passed or all_flags_passed:
                error_out(
                    "You may not pass a non-decimal digest flag when `*` or `N` is passed"
                )
            continue
        if flag == FLAG_HELP:
            if len_flags > MIN_FLAG_SIZE:
                error_out(
                    "You may not pass the `?` flag along with other flags")
        if flag == FLAG_DASH:
            error_out(
                "You may not use `-` in the first argument other than in the first, and the last letter"
            )
        else:
            error_out("Unknown flag detected!")


def get_time_in_ns() -> int:
    return time_ns()


def convert_time(ns: int, div: int) -> str:
    return to_e_notation(ns / div, E_NOT_TRUNC_LEN)


def all_are_false(bools: list[bool]) -> bool:
    for bl in bools:
        if bl:
            return False
    return True


def print_hashes(hashes: list[PoxDigest], flags: str, total_time: int) -> None:
    len_hashes = len(hashes)
    reoccurrance = search_for_flag_occurrances(flags[1:-1])
    double_benchmark = reoccurrance == FLAG_BENCHMARK

    if arg_has_flag(flags, FLAG_BENCHMARK):
        printf("| %d Message(s) ||", len(hashes))
        has_printed = False
        if arg_has_flag(flags, FLAG_NS) or double_benchmark:
            printf(" %dns |", convert_time(total_time, NS_TO_NS))
            has_printed = True
        if arg_has_flag(flags, FLAG_US) or double_benchmark:
            printf(" %dus |", convert_time(total_time, NS_TO_US))
            has_printed = True
        if arg_has_flag(flags, FLAG_MS) or double_benchmark:
            printf(" %dms |", convert_time(total_time, NS_TO_MS))
            has_printed = True
        if arg_has_flag(flags, FLAG_SS) or double_benchmark:
            printf(" %ds |", convert_time(total_time, NS_TO_SS))
            has_printed = True
        if arg_has_flag(flags, FLAG_MM) or double_benchmark:
            printf(" %dm |", convert_time(total_time, NS_TO_MM))
            has_printed = True
        if not has_printed:
            printf(" %dus |", convert_time(total_time, NS_TO_US))
        println()

    if double_benchmark:
        println()
        exit(0)

    everything = arg_has_flag(flags, FLAG_EVERTHING)
    all_flags_decimal = arg_has_flag(flags, FLAG_ALL_DECIMAL)
    all_flags_non_decimal = arg_has_flag(flags, FLAG_ALL_NON_DEC)
    by = arg_has_flag(flags, FLAG_BYTES)
    word = arg_has_flag(flags, FLAG_WORDS)
    dub = arg_has_flag(flags, FLAG_DOUBLES)
    quad = arg_has_flag(flags, FLAG_QUAD)
    sex = arg_has_flag(flags, FLAG_SEX)
    vig = arg_has_flag(flags, FLAG_VIG)
    hex = arg_has_flag(flags, FLAG_HEX)
    tet = arg_has_flag(flags, FLAG_TET)
    duo = arg_has_flag(flags, FLAG_DUO)
    oct = arg_has_flag(flags, FLAG_OCT)
    sen = arg_has_flag(flags, FLAG_SEN)
    bin = arg_has_flag(flags, FLAG_BIN)

    all_false = all_are_false([
        everything,
        all_flags_decimal,
        all_flags_non_decimal,
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

    if all_false:
        printf("You had not specfied any digests to be printed\n")
        exit(0)

    for hash in hashes:
        if everything or all_flags_decimal or by:
            printf("Bytes: U8[%d, %d, %d, %d, %d, %d, %d, %d]\n",
                   hash.bytes[0], hash.bytes[1], hash.bytes[2], hash.bytes[3],
                   hash.bytes[4], hash.bytes[5], hash.bytes[6], hash.bytes[7])
        if everything or all_flags_decimal or word:
            printf("Words: U16[%d, %d, %d, %d]\n", hash.words[0],
                   hash.words[1], hash.words[2], hash.words[3])
        if everything or all_flags_decimal or dub:
            printf("Doubles: U32[%d, %d]\n", hash.doubles[0], hash.doubles[1])
        if everything or all_flags_decimal or quad:
            printf("Quad: U64[%d]\n", hash.quad[0])
        if everything or all_flags_non_decimal or sex:
            printf("Sexdigest: %s\n", hash.sexdigest)
        if everything or all_flags_non_decimal or vig:
            printf("Vigdigest: %s\n", hash.vigdigest)
        if everything or all_flags_non_decimal or hex:
            printf("Hexdigest: %s\n", hash.hexdigest)
        if everything or all_flags_non_decimal or tet:
            printf("Tetdigest: %s\n", hash.tetdigest)
        if everything or all_flags_non_decimal or duo:
            printf("Duodigest: %s\n", hash.duodigest)
        if everything or all_flags_non_decimal or oct:
            printf("Octdigest: %s\n", hash.octdigest)
        if everything or all_flags_non_decimal or sen:
            printf("Sendgiest: %s\n", hash.sendigest)
        if everything or all_flags_non_decimal or bin:
            printf("Bindigest: %s\n", hash.bindigest)
        printf("\033[2m----\033[0m\n")


def assert_file(arg: str) -> bool:
    return len(arg) > FILE_PREFIX_LEN and arg.startswith(FILE_PREFIX)


def assert_int(arg: str) -> bool:
    return len(arg) > INT_PREFIX_LEN and arg.startswith(INT_PREFIX)


def to_int(numbers: str) -> array:
    result = []

    split_up = numbers.split(",")
    for num in split_up:
        if num.startswith(BIN_PREFIX):
            if len(num) - BASE_PREFIX_NUM > MAX_BIN:
                error_out("Size of binary number should not exceed 8")
            result.append(int(num[BASE_PREFIX_NUM:], 2))
        elif num.startswith(OCT_PREFIX):
            if len(num) - BASE_PREFIX_NUM > MAX_OCT:
                error_out("Size of octal number should not exceed 5")
            result.append(int(num[BASE_PREFIX_NUM:], 8))
        elif num.startswith(HEX_PREFIX):
            if len(num) - BASE_PREFIX_NUM > MAX_HEX:
                error_out("Size of hexadecimal number should not exceed 2")
            result.append(int(num[BASE_PREFIX_NUM:], 16))
        else:
            if num.isdigit():
                integer = int(num)
                if array('H', [integer])[0] > MAX_U8:
                    error_out("Given integer must be byte-sized (0-255)")
                result.append(integer)
            else:
                error_out(
                    "With 'int=' prefix you must pass byte-sized integers in base 16, 8, 10 and 2"
                )
    return array('B', result)


def join_args(args: list[str]) -> str:
    joined = args[0]
    for arg in args[1:]:
        joined = f"{joined} {arg}"
    return joined


def is_regular_file(fpath: str) -> Path:
    path = Path(fpath)
    if not path.is_file() or not path.exists():
        error_out(
            "Specfied file does not exist or is a directory. Pass `+` with only one argument to ignore"
        )

    return path


def read_given_file(fpath: str) -> str:
    return is_regular_file(fpath).read_text()


def to_ubyte_array(arg: str) -> any:
    from array import array
    return array('B', arg)


def process_arg(arg: str) -> any:
    if not assert_file(arg) and not assert_int(arg):
        return to_ubyte_array(arg.encode())
    elif assert_int(arg):
        return to_int(arg[INT_PREFIX_LEN:])
    return to_ubyte_array(read_given_file(arg[FILE_PREFIX_LEN:]).encode())


def main(exec_name: str, argv: list[str]) -> None:
    validate_flags(exec_name, argv)
    flags_arg = argv[0]

    if not arg_has_flag(flags_arg, FLAG_NHEADER):
        printf(
            "\033[1;30;47m   PoxHashRunner   |    Python   |  March 2023 - Chubak Bidpaa  |  MIT  \033[0m\n"
        )

    echo_arg = arg_has_flag(flags_arg, FLAG_ECHO)
    len_hashes = len(argv) - 1
    hashes = [None] * len_hashes

    total_time = 0
    if arg_has_flag(flags_arg, FLAG_JOIN):
        args_joined = join_args(argv[1:])
        if echo_arg:
            printf("Joined Args: \n`%s`\n", args_joined)
        u8_arg = to_ubyte_array(args_joined.encode())
        t1 = get_time_in_ns()
        hashes[0] = pox_hash(u8_arg)
        t2 = get_time_in_ns()
        print_hashes(hashes[:1], flags_arg, t2 - t1)
    else:
        for i, arg in enumerate(argv[1:]):
            if echo_arg:
                printf("Arg %d: %s\n", i + 1, arg)
            processed_arg = process_arg(arg)
            t1 = get_time_in_ns()
            hashes[i] = pox_hash(to_ubyte_array(processed_arg))
            t2 = get_time_in_ns()
            total_time += t2 - t1
        print_hashes(hashes, flags_arg, total_time)


if __name__ == "__main__":
    main(sys.argv[0], sys.argv[1:])