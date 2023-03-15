import sys
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
FLAG_NHEADER = 'z'

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
    ('x', 'z'),
    ('Z', 'z'),
    ('a', 'z'),
]


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
    println()
    printf(message)
    println()
    printf(
        "\x1b[1;31mError occurred\x1b[0m. Please pass \x1b[134m-?-\x1b[0m to show help\n"
    )
    exit(1)


def printHelp(exec_name: str, script_name: str) -> None:
    printf(
        "\x1b[1;30;42mHelp | Chubak#7400 (Discord) | @bidpaafx (Telegram) | Chubakbidpaa[at]gmail\x1b[0m\n"
    )
    println()
    printf("Examples \x1b[1m(flags go between two dashes!)\x1b[0m:\n")
    printf("%s %s -N82- myword1\n", exec_name, script_name)
    printf("%s %s -*+^- mywod to be joined\n", exec_name, script_name)
    printf("%s %s -Dhob- word1 word 2\n", exec_name, script_name)
    printf("%s %s -^^+- large seq  to join and  benchmark\n", exec_name,
           script_name)
    printf("wget -qO- www.example.com | xargs bash -c '%s %s -h+- $@'\n",
           exec_name, script_name)
    println()
    printf("\x1b[1;32mFlags:\x1b[0m\n")
    printf(
        "\033[1;35m\t`%c`\033[0m: Benchmark run (pass two to only show benchmark)\n",
        FLAG_BENCHMARK)
    printf("\033[1;35m\t`%c`\033[0m: Join arguments with space (byte 32)\n",
           FLAG_JOIN)
    printf("\033[1;35m\t`%c`\033[0m: Print every digest\n", FLAG_EVERTHING)
    printf("\033[1;35m\t`%c`\033[0m: Print every non-decimal digest\n",
           FLAG_ALL_NON_DEC)
    printf("\033[1;35m\t`%c`\033[0m: Print every decimal digest\n",
           FLAG_ALL_DECIMAL)
    printf(
        "\033[1;35m\t`%c`\033[0m: Print bytes digest (eight unsigned 8-bit integers)\n",
        FLAG_BYTES)
    printf(
        "\033[1;35m\t`%c`\033[0m: Print words digest (four unsigned 16-bit integers)\n",
        FLAG_WORDS)
    printf(
        "\033[1;35m\t`%c`\033[0m: Print doubles digest (two unsigned 32-bit integers)\n",
        FLAG_DOUBLES)
    printf(
        "\033[1;35m\t`%c`\033[0m: Print quad digest (one unsigned 64-bit integer)\n",
        FLAG_QUAD)
    printf("\033[1;35m\t`%c`\033[0m: Print sexagesimal digest (base sixty)\n",
           FLAG_SEX)
    printf("\033[1;35m\t`%c`\033[0m: Print vigesimal digest (base twenty)\n",
           FLAG_VIG)
    printf(
        "\033[1;35m\t`%c`\033[0m: Print hexadecimal digest (base sixteen)\n",
        FLAG_HEX)
    printf(
        "\033[1;35m\t`%c`\033[0m: Print tetradecimal digest (base fourteen)\n",
        FLAG_TET)
    printf("\033[1;35m\t`%c`\033[0m: Print duodecimal digest (base twelve)\n",
           FLAG_DUO)
    printf("\033[1;35m\t`%c`\033[0m: Print octal digest (base eight)\n",
           FLAG_OCT)
    printf("\033[1;35m\t`%c`\033[0m: Print senary digest (base six)\n",
           FLAG_SEN)
    printf("\033[1;35m\t`%c`\033[0m: Print binary digest (base two)\n",
           FLAG_BIN)
    printf("\033[1;35m\t`%c`\033[0m: Print Help\n\n", FLAG_HELP)
    exit(1)


def get_script_and_exec_name(path: str) -> str:
    return path.split("/")[-1]


def check_for_wrong_flags(flags: str) -> None:
    for flag in flags:
        for (wrong_flag, right_flag) in WRONG_FLAGS:
            if flag == wrong_flag:
                printf("No flag for `%c`, perhaps you meant `%c`?", flag,
                       right_flag)
                error_out("Flag erreror")


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

    if len_argv < MIN_ARG_NUM + 1:
        error_out("You must pass at least one argument to hash")

    all_flags_passed = arg_has_flag(flags_arg, FLAG_EVERTHING)
    all_flags_dec_passed = arg_has_flag(flags_arg, FLAG_ALL_DECIMAL)
    all_flags_non_dec_passed = arg_has_flag(flags_arg, FLAG_ALL_NON_DEC)

    for flag in flags_arg[1:-1]:
        if flag == FLAG_BENCHMARK:
            continue
        if flag == FLAG_JOIN:
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
        if flag == FLAG_NHEADER:
            continue
        else:
            error_out("Unknown flag detected!")


def get_time_in_us() -> int:
    return time_ns() // NS_TO_US


def all_are_false(bools: list[bool]) -> bool:
    for bl in bools:
        if bl:
            return False
    return True


def join_args(args: list[str]) -> str:
    return ' '.join(args)


def print_hashes(hashes: list[PoxDigest], flags: str, total_time: int) -> None:
    len_hashes = len(hashes)
    reoccurrance = search_for_flag_occurrances(flags[1:-1])

    if arg_has_flag(flags, FLAG_BENCHMARK):
        printf("Total time for hashing %d bytestring(s): %dus \n", len_hashes,
               total_time)

    if reoccurrance == FLAG_BENCHMARK:
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
    duo = arg_has_flag(flags, FLAG_TET)
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
        printf("----\n")
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
            printf("Quad: U64[%d]\n", hash.quad)
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


def main(exec_name: str, argv: list[str]) -> None:
    validate_flags(exec_name, argv)
    flags_arg = argv[0]

    if not arg_has_flag(flags_arg, FLAG_NHEADER):
        printf(
            "\x1b[1;30;47mPoxHashRunner   |    Python   |  March 2023 - Chubak Bidpaa  |  GPLv3  \x1b[0m\n"
        )

    len_hashes = len(argv) - 1
    hashes = [None] * len_hashes

    total_time = 0
    if arg_has_flag(flags_arg, FLAG_JOIN):
        args_joined = join_args(argv[1:])
        t1 = get_time_in_us()
        hashes[0] = pox_hash(args_joined.encode())
        t2 = get_time_in_us()
        print_hashes(hashes[:1], flags_arg, t2 - t1)
    else:
        for i, arg in enumerate(argv[1:]):
            t1 = get_time_in_us()
            hashes[i] = pox_hash(arg.encode())
            t2 = get_time_in_us()
            total_time += t2 - t1
        print_hashes(hashes, flags_arg, total_time)


if __name__ == "__main__":
    main(sys.argv[0], sys.argv[1:])