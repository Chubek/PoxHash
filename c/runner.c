/////////////////////////////////////////////////////////////////////////////////////
//                       Runner for libpoxh.h (PoxHash C)                          //
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

#ifndef __RUNNER_C
#define __RUNNER_C

#ifndef _POXIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif

#ifndef __INCLUDE_STDIO
#define __INCLUDE_STDIO
#define _GNU_SOURCE
#include <stdio.h>
#endif
#ifndef __INCLUDE_STDLIB
#define __INCLUDE_STDLIB
#include <stdlib.h>
#endif
#ifndef __INDLUCE_TIME
#define __INCLUDE_TIME
#include <time.h>
#endif
#ifndef __INCLUDE_LIBPOX
#define __INCLUDE_LIBPOX
#include "libpoxh.h"
#endif
#ifndef __INCLUDE_UNISTD
#define __INCLUDE_UNISTD
#include <unistd.h>
#endif
#ifndef __INCLUDE_TYPES
#define __INCLUDE_TYPES
#include <sys/types.h>
#endif
#ifndef __INCLUDE_STAT
#define __INCLUDE_STAT
#include <sys/stat.h>
#endif
#ifndef __INCLUDE_STR
#define __INCLUDE_STR
#include <string.h>
#endif
#ifndef __INCLUDE_MATH
#define __INCLUDE_MATH
#include <math.h>
#endif

#define MAX_FLAG_SIZE 24
#define MIN_FLAG_SIZE 3
#define HELP_FLAG_SIZE 3
#define SPACE 32
#define MIN_ARG_NUM 3
#define NUM_ASCII 128
#define LEN_WRONG_FLAGS 28
#define BENCHMARK_BYTE_INDEX 94
#define FILE_PREFIX_LEN 5
#define FILE_PREFIX "file="
#define INT_PREFIX_LEN 5
#define INT_PREFIX "int="
#define NUM_ALL_DIGITS 10

#define MAX_HEX 4
#define MAX_OCT 5
#define MAX_BIN 8

#define PREFIX_HEX "0x"
#define PREFIX_OCT "0o"
#define PREFIX_BIN "0b"

#define BASE_PREFIX_NUM 2

#define NS_TO_NS 100
#define NS_TO_US 1000
#define NS_TO_MS 1000000
#define NS_TO_SS 1000000000
#define NS_TO_MM 60000000000

#define E_NOT_TRUNC_LEN 4

#define COMPARE_STR(a, b, n) (strncmp(a, b, n) == 0)
#define IS_STRLEN_BIGGER(a, len) (strlen(a) > len)

#define REALLOC_ARR(size, arr)                                    \
    do                                                            \
    {                                                             \
        void *nnptr = realloc(arr, size++ * sizeof(uint8_t));     \
        if (!nnptr)                                               \
        {                                                         \
            ERR_OUT("Problem reallocating integer result array"); \
        }                                                         \
        arr = nnptr;                                              \
    } while (0)

#define ERR_OUT(message)                                                                                 \
    fprintf(stderr, "\n");                                                                               \
    fprintf(stderr, message);                                                                            \
    fprintf(stderr, "\n");                                                                               \
    fprintf(stderr, "\033[1;31mError occurred\033[0m. Please pass \033[1;34m-?-\033[0m to show help\n"); \
    exit(1)

#define SASPRINTF(write_to, ...)                  \
    {                                             \
        char *tmp_string_for_extend = (write_to); \
        asprintf(&(write_to), __VA_ARGS__);       \
        free(tmp_string_for_extend);              \
    }

#define INDEX_OF(str, of, res)            \
    for (int i = 0; i < strlen(str); i++) \
    {                                     \
        if (str[i] == of)                 \
        {                                 \
            res = i;                      \
        }                                 \
    }

typedef enum FLAGS
{
    FLAG_BENCHMARK = '^',
    FLAG_JOIN = '+',
    FLAG_EVERTHING = '*',
    FLAG_ALL_NON_DEC = 'N',
    FLAG_ALL_DECIMAL = 'D',
    FLAG_BYTES = '8',
    FLAG_WORDS = '4',
    FLAG_DOUBLES = '2',
    FLAG_QUAD = '1',
    FLAG_SEX = 'g',
    FLAG_VIG = 'v',
    FLAG_HEX = 'h',
    FLAG_TET = 't',
    FLAG_DUO = 'd',
    FLAG_OCT = 'o',
    FLAG_SEN = 's',
    FLAG_BIN = 'b',
    FLAG_NS = '9',
    FLAG_US = '6',
    FLAG_MS = '3',
    FLAG_SS = '5',
    FLAG_MM = '0',
    FLAG_HELP = '?',
    FLAG_DASH = '-',
    FLAG_NHEADER = 'z',
    FLAG_ECHO = 'e',
} flag_t;

const char cFILE_DENOTE_PREFIX[FILE_PREFIX_LEN] = {
    'f',
    'i',
    'l',
    'e',
    '=',
};

const char cWRONG_FLAGS[LEN_WRONG_FLAGS][2] = {
    {'G', 'g'},
    {'V', 'v'},
    {'O', 'o'},
    {'T', 't'},
    {'S', 's'},
    {'H', 'h'},
    {'n', 'N'},
    {'W', '4'},
    {'w', '4'},
    {'q', '1'},
    {'Q', '1'},
    {'/', '?'},
    {'=', '+'},
    {'B', 'b'},
    {'E', '*'},
    {'A', '*'},
    {'>', '?'},
    {'&', '*'},
    {'r', 't'},
    {'y', 't'},
    {'f', 'g'},
    {'x', 'h'},
    {'Z', 'z'},
    {'a', 'z'},
    {'E', 'e'},
    {'w', 'e'},
    {'r', 'e'},
    {'i', 'e'},
};
const char cALL_DIGITS[10] = {
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
};

char *to_e_notation(double num, size_t places)
{
    num = fabs(num);
    if (num > 1.0)
    {
        char *num_str, first_digit, *truncs, *e_str, *ret;
        size_t index_of_period, e;

        num_str = NULL;
        SASPRINTF(num_str, "%f", num);
        INDEX_OF(num_str, '.', index_of_period);
        e = index_of_period - 1;

        truncs = NULL;
        first_digit = num_str[0];
        for (int i = 1; i < places + 1; i++)
        {
            if (num_str[i] == '.')
            {
                continue;
            }
            truncs == NULL ? (SASPRINTF(truncs, "%c", num_str[i])) : (SASPRINTF(truncs, "%s%c", truncs, num_str[i]));
        }

        e_str = NULL;
        if (e > 9)
        {
            SASPRINTF(e_str, "%lu", e);
        }
        else
        {
            SASPRINTF(e_str, "0%lu", e);
        }
        ret = NULL;
        SASPRINTF(ret, "%c.%se+%s", first_digit, truncs, e_str);
        return ret;
    }
    else if (num > 0.0 && num < 1.0)
    {
        char *num_str, first_digit, *truncs, *e_str, *ret, c;
        size_t first_non_zero_index, e, len_truncs;

        num_str = NULL;
        SASPRINTF(num_str, "%f", num);

        first_non_zero_index = 0;
        len_truncs = 0;
        truncs = NULL;
        for (int i = 0; i < strlen(num_str); i++)
        {
            c = num_str[i];
            if (c != '0' && c != '.' && first_non_zero_index == 0)
            {
                first_non_zero_index = i;
                first_digit = c;
                continue;
            }
            if (first_non_zero_index != 0 && len_truncs < places)
            {
                truncs == NULL ? (SASPRINTF(truncs, "%c", c)) : (SASPRINTF(truncs, "%s%c", truncs, c));
                ++len_truncs;
            }
        }
        e = first_non_zero_index - 1;
        e_str = NULL;
        if (e > 9)
        {
            SASPRINTF(e_str, "%lu", e);
        }
        else
        {
            SASPRINTF(e_str, "0%lu", e);
        }
        ret = NULL;
        SASPRINTF(ret, "%c.%se-%s", first_digit, truncs, e_str);
        return ret;
    }
    else
    {
        char *num_str = NULL;
        SASPRINTF(num_str, "%f", num);
        return num_str;
    }
}

void print_help(char *exec)
{
    printf("\033[1;30;42mHelp | Chubak#7400 (Discord) | @bidpaafx (Telegram) | Chubakbidpaa[at]gmail[dot]com\033[0m\n");
    printf("\n");
    printf("Examples \033[1m(flags go between two dashes!)\033[0m:\n");
    printf("%s -N82- myword1\n", exec);
    printf("%s -*+^- mywod to be joined\n", exec);
    printf("%s -Dhob- word1 word 2\n", exec);
    printf("%s -^^+- large seq  to join and  benchmark\n", exec);
    printf("wget -qO- www.example.com | xargs bash -c '%s -h+- $@'\n", exec);
    printf("If an argument stats with `%s`, it will lead to file read attempt, unless `%c` is passed\n", FILE_PREFIX, FLAG_JOIN);
    printf("If an argument stats with `%s`, it will parse the int, prefixes 0b, 0o and 0x for bin, oct and hex and none for decimal apply\n", INT_PREFIX);
    printf("\n");
    printf("\033[1;32mFlags:\033[0m\n");
    printf("\033[1;33m\t`%c`\033[0m: Echo argument\n", FLAG_ECHO);
    printf("\033[1;33m\t`%c`\033[0m: Don't print header message\n", FLAG_NHEADER);
    printf("\033[1;33m\t`%c`\033[0m: Benchmark run (pass two to only show benchmark)\n", FLAG_BENCHMARK);
    printf("\033[1;33m\t`%c`\033[0m: Join arguments with space (byte 32)\n", FLAG_JOIN);
    printf("\033[1;33m\t`%c`\033[0m: Print every digest\n", FLAG_EVERTHING);
    printf("\033[1;33m\t`%c`\033[0m: Print every non-decimal digest\n", FLAG_ALL_NON_DEC);
    printf("\033[1;33m\t`%c`\033[0m: Print every decimal digest\n", FLAG_ALL_DECIMAL);
    printf("\033[1;33m\t`%c`\033[0m: Print bytes digest (eight unsigned 8-bit integers)\n", FLAG_BYTES);
    printf("\033[1;33m\t`%c`\033[0m: Print words digest (four unsigned 16-bit integers)\n", FLAG_WORDS);
    printf("\033[1;33m\t`%c`\033[0m: Print doubles digest (two unsigned 32-bit integers)\n", FLAG_DOUBLES);
    printf("\033[1;33m\t`%c`\033[0m: Print quad digest (one unsigned 64-bit integer)\n", FLAG_QUAD);
    printf("\033[1;33m\t`%c`\033[0m: Print sexagesimal digest (base sixty)\n", FLAG_SEX);
    printf("\033[1;33m\t`%c`\033[0m: Print vigesimal digest (base twenty)\n", FLAG_VIG);
    printf("\033[1;33m\t`%c`\033[0m: Print hexadecimal digest (base sixteen)\n", FLAG_HEX);
    printf("\033[1;33m\t`%c`\033[0m: Print tetradecimal digest (base fourteen)\n", FLAG_TET);
    printf("\033[1;33m\t`%c`\033[0m: Print duodecimal digest (base twelve)\n", FLAG_DUO);
    printf("\033[1;33m\t`%c`\033[0m: Print octal digest (base eight)\n", FLAG_OCT);
    printf("\033[1;33m\t`%c`\033[0m: Print senary digest (base six)\n", FLAG_SEN);
    printf("\033[1;33m\t`%c`\033[0m: Print binary digest (base two)\n", FLAG_BIN);
    printf("\033[1;33m\t`%c`\033[0m: Print total time in nanoseconds\n", FLAG_NS);
    printf("\033[1;33m\t`%c`\033[0m: Print total time in mictoseconds\n", FLAG_US);
    printf("\033[1;33m\t`%c`\033[0m: Print total time in milliseconds\n", FLAG_MS);
    printf("\033[1;33m\t`%c`\033[0m: Print total time in seconds\n", FLAG_SS);
    printf("\033[1;33m\t`%c`\033[0m: Print total time in minutes\n", FLAG_MM);
    printf("\033[1;33m\t`%c`\033[0m: Print Help\n\n", FLAG_HELP);
    free(exec);
    exit(1);
}

void check_for_wrong_flags(char *flags, int len_flags)
{
    char flag, wrong_flag, right_flag;
    for (int i = 0; i < len_flags; i++)
    {
        flag = flags[i];
        for (int j = 0; j < LEN_WRONG_FLAGS; j++)
        {
            wrong_flag = cWRONG_FLAGS[j][0];
            right_flag = cWRONG_FLAGS[j][1];
            if (flag == wrong_flag)
            {
                printf("No flag for `%c`, perhaps you meant `%c`?", flag, right_flag);
                ERR_OUT("Flag errror");
            }
        }
    }
}

char *get_exec_name(char *argv0)
{
    int size_before_slash = 0;
    int slash_index = 0;
    int size = strlen(argv0);
    for (int i = size - 1; i > 0; i--)
    {
        if (argv0[i] == '/')
            break;
        size_before_slash++;
        slash_index = i;
    }

    char *exec_name = calloc(0, size_before_slash + 1);
    memcpy(exec_name, &argv0[slash_index], size_before_slash);
    return exec_name;
}

int arg_has_flag(char *flag_arg, int len_flags, flag_t must_have)
{
    for (int i = 1; i < len_flags - 1; i++)
    {
        if (flag_arg[i] == must_have)
        {
            return 1;
        }
    }
    return 0;
}

char search_for_flag_reocurrance(char *flag_arg, int len_flags)
{
    char occurance_array[NUM_ASCII];
    memset(occurance_array, 0, NUM_ASCII);
    for (int i = 0; i < len_flags; i++)
    {
        occurance_array[flag_arg[i]] += 1;
    }

    char ret = 0;

    if (occurance_array[94] == 2)
        ret = '^';

    if (occurance_array[94] > 2)
    {
        ERR_OUT("`^` can appear at most twice");
    }

    for (int i = 0; i < NUM_ASCII; i++)
    {
        if (i == 94 || i == 45)
            continue;
        if (occurance_array[i] > 1)
        {
            ret = (char)i;
            return ret;
        }
    }

    return ret;
}

int validate_flags(int argc, char **argv)
{
    if (argc < MIN_ARG_NUM - 1)
    {
        ERR_OUT("No flags passed");
    }

    int len_flags = strlen(argv[1]);
    if (len_flags < MIN_FLAG_SIZE || len_flags > MAX_FLAG_SIZE)
    {
        ERR_OUT("Length of the first argument must at least be 3 and at most 24");
    }

    if (argv[1][0] != FLAG_DASH || argv[1][len_flags - 1] != FLAG_DASH)
    {
        ERR_OUT("The flag argument must begin and end with `-`");
    }

    check_for_wrong_flags(argv[1], len_flags);

    char *exec_name = get_exec_name(argv[0]);
    if (!(strncmp(argv[1], "-?-", HELP_FLAG_SIZE)))
        print_help(exec_name);

    int help_passed = arg_has_flag(argv[1], len_flags, FLAG_HELP);
    if (help_passed && len_flags > MIN_FLAG_SIZE)
    {
        ERR_OUT("You may not pass the `?` flag along with other flags");
    }

    char reoccurrance = search_for_flag_reocurrance(argv[1], len_flags);
    if (reoccurrance != '\0' && reoccurrance != FLAG_BENCHMARK)
    {
        printf("Flag `%c` appears twice", reoccurrance);
        ERR_OUT("Only `^` can appear twice");
    }

    if (argc < MIN_ARG_NUM)
    {
        ERR_OUT("You must pass at least one argument to hash");
    }

    int all_flags_passed = arg_has_flag(argv[1], len_flags, FLAG_EVERTHING);
    int all_flags_dec_passed = arg_has_flag(argv[1], len_flags, FLAG_ALL_DECIMAL);
    int all_flags_nondec_passed = arg_has_flag(argv[1], len_flags, FLAG_ALL_NON_DEC);
    int benchmark_has_passed = arg_has_flag(argv[1], len_flags, FLAG_BENCHMARK);

    for (int i = 1; i < len_flags - 1; i++)
    {
        switch (argv[1][i])
        {
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
            if (!benchmark_has_passed)
            {
                ERR_OUT("When a timestamp flag has passed, `^` must be passed as well");
            }
            continue;
        case FLAG_EVERTHING:
            if (all_flags_dec_passed || all_flags_nondec_passed)
            {
                ERR_OUT("You may not pass `*` when you have passed `N` or `D`");
            }
            continue;
        case FLAG_ALL_NON_DEC:
            if (all_flags_passed)
            {
                ERR_OUT("You may not pass `N` when `*` is passed");
            }
            continue;
        case FLAG_ALL_DECIMAL:
            if (all_flags_passed)
            {
                ERR_OUT("You may not pass `D` when `*` is passed");
            }
            continue;
        case FLAG_BYTES:
            if (all_flags_dec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a decimal digest flag when `*` or `D` is passed");
            }
            continue;
        case FLAG_WORDS:
            if (all_flags_dec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a decimal digest flag when `*` or `D` is passed");
            }
            continue;
        case FLAG_DOUBLES:
            if (all_flags_dec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a decimal digest flag when `*` or `D` is passed");
            }
            continue;
        case FLAG_QUAD:
            if (all_flags_dec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a decimal digest flag when `*` or `D` is passed");
            }
            continue;
        case FLAG_SEX:
            if (all_flags_nondec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a non-decimal digest flag when `*` or `N` is passed");
            }
            continue;
        case FLAG_VIG:
            if (all_flags_nondec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a non-decimal digest flag when `*` or `N` is passed");
            }
            continue;
        case FLAG_HEX:
            if (all_flags_nondec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a non-decimal digest flag when `*` or `N` is passed");
            }
            continue;
        case FLAG_TET:
            if (all_flags_nondec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a non-decimal digest flag when `*` or `N` is passed");
            }
            continue;
        case FLAG_DUO:
            if (all_flags_nondec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a non-decimal digest flag when `*` or `N` is passed");
            }
            continue;
        case FLAG_OCT:
            if (all_flags_nondec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a non-decimal digest flag when `*` or `N` is passed");
            }
            continue;
        case FLAG_SEN:
            if (all_flags_nondec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a non-decimal digest flag when `*` or `N` is passed");
            }
            continue;
        case FLAG_BIN:
            if (all_flags_nondec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a non-decimal digest flag when `*` or `N` is passed");
            }
            continue;
        case FLAG_HELP:
            if (len_flags > MIN_FLAG_SIZE)
            {
                ERR_OUT("You may not pass the `?` flag along with other flags");
            }
        case FLAG_DASH:
            ERR_OUT("You may not use `-` in the first argument other than in the first, and the last letter");
        default:
            ERR_OUT("Unknown flag detected!");
        }
    }

    return len_flags;
}

uint64_t get_time_in_ns()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_nsec;
}

char *convert_time(uint64_t time, uint64_t divisor)
{
    return to_e_notation(((double)time) / ((double)divisor), E_NOT_TRUNC_LEN);
}

int all_are_false(int *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == 1)
        {
            return 0;
        }
    }
    return 1;
}

void print_hashes(poxdigest_t *hashes, int len_hashes, char *flags, int len_flags, uint64_t total_time)
{
    if (arg_has_flag(flags, len_flags, FLAG_BENCHMARK))
    {
        printf("| %d Messages |", len_hashes);
        int has_printed = 0;
        if (arg_has_flag(flags, len_flags, FLAG_NS))
        {
            char *ns_float_notation = convert_time(total_time, NS_TO_NS);
            printf(" %sns |", ns_float_notation);
            free(ns_float_notation);
            has_printed = 1;
        }
        if (arg_has_flag(flags, len_flags, FLAG_US))
        {
            char *us_float_notation = convert_time(total_time, NS_TO_US);
            printf(" %sus |", us_float_notation);
            free(us_float_notation);
            has_printed = 1;
        }
        if (arg_has_flag(flags, len_flags, FLAG_MS))
        {
            char *ms_float_notation = convert_time(total_time, NS_TO_MS);
            printf(" %sms |", ms_float_notation);
            free(ms_float_notation);
            has_printed = 1;
        }
        if (arg_has_flag(flags, len_flags, FLAG_SS))
        {
            char *ss_float_notation = convert_time(total_time, NS_TO_SS);
            printf(" %ss |", ss_float_notation);
            free(ss_float_notation);
            has_printed = 1;
        }
        if (arg_has_flag(flags, len_flags, FLAG_MM))
        {
            char *mm_float_notation = convert_time(total_time, NS_TO_MM);
            printf(" %sm |", mm_float_notation);
            free(mm_float_notation);
            has_printed = 1;
        }
        if (!has_printed)
        {
            char *us_float_notation = convert_time(total_time, NS_TO_US);
            printf(" %sus |", us_float_notation);
            free(us_float_notation);
        }
        printf("\n");
    }

    char reoccurrance = search_for_flag_reocurrance(flags, len_flags);
    if (reoccurrance == FLAG_BENCHMARK)
    {
        printf("\n");
        exit(0);
    }

    int everything = arg_has_flag(flags, len_flags, FLAG_EVERTHING);
    int all_flags_decimal = arg_has_flag(flags, len_flags, FLAG_ALL_DECIMAL);
    int all_flags_non_decimal = arg_has_flag(flags, len_flags, FLAG_ALL_NON_DEC);
    int byte = arg_has_flag(flags, len_flags, FLAG_BYTES);
    int word = arg_has_flag(flags, len_flags, FLAG_WORDS);
    int dub = arg_has_flag(flags, len_flags, FLAG_DOUBLES);
    int quad = arg_has_flag(flags, len_flags, FLAG_QUAD);
    int sex = arg_has_flag(flags, len_flags, FLAG_SEX);
    int vig = arg_has_flag(flags, len_flags, FLAG_VIG);
    int hex = arg_has_flag(flags, len_flags, FLAG_HEX);
    int tet = arg_has_flag(flags, len_flags, FLAG_TET);
    int duo = arg_has_flag(flags, len_flags, FLAG_DUO);
    int oct = arg_has_flag(flags, len_flags, FLAG_OCT);
    int sen = arg_has_flag(flags, len_flags, FLAG_SEN);
    int bin = arg_has_flag(flags, len_flags, FLAG_BIN);

    int all_false = all_are_false(
        (int[]){
            everything,
            all_flags_decimal,
            all_flags_non_decimal,
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
        },
        15);

    if (all_false)
    {

        printf("You had not specfied any digests to be printed\n");
        exit(0);
    }

    for (int i = 0; i < len_hashes; i++)
    {
        if (everything || all_flags_decimal || byte)
            printf("Bytes: U8[%hu, %u, %u, %u, %u, %u, %u, %u]\n", hashes[i].bytes[0], hashes[i].bytes[1], hashes[i].bytes[2], hashes[i].bytes[3], hashes[i].bytes[4], hashes[i].bytes[5], hashes[i].bytes[6], hashes[i].bytes[7]);
        if (everything || all_flags_decimal || word)
            printf("Words: U16[%hu, %hu, %hu, %hu]\n", hashes[i].words[0], hashes[i].words[1], hashes[i].words[2], hashes[i].words[3]);
        if (everything || all_flags_decimal || dub)
            printf("Doubles: U32[%u, %u]\n", hashes[i].doubles[0], hashes[i].doubles[1]);
        if (everything || all_flags_decimal || quad)
            printf("Quad: U64[%lu]\n", hashes[i].quad);
        if (everything || all_flags_non_decimal || sex)
            printf("Sexdigest: %s\n", hashes[i].sexdigest);
        if (everything || all_flags_non_decimal || vig)
            printf("Vigdigest: %s\n", hashes[i].vigdigest);
        if (everything || all_flags_non_decimal || hex)
            printf("Hexdigest: %s\n", hashes[i].hexdigest);
        if (everything || all_flags_non_decimal || tet)
            printf("Tetdigest: %s\n", hashes[i].tetdigest);
        if (everything || all_flags_non_decimal || duo)
            printf("Duodigest: %s\n", hashes[i].duodigest);
        if (everything || all_flags_non_decimal || oct)
            printf("Octdigest: %s\n", hashes[i].octdigest);
        if (everything || all_flags_non_decimal || sen)
            printf("Sendgiest: %s\n", hashes[i].sendigest);
        if (everything || all_flags_non_decimal || bin)
            printf("Bindigest: %s\n", hashes[i].bindigest);
        printf("----\n");
    }
}

int string_starts_with(char *str, char *prefix)
{
    return strncmp(prefix, str, strlen(prefix)) == 0;
}

char *substring(char *str, size_t start, size_t end)
{
    char *sub = calloc((end - start) + 1, sizeof(char));
    memcpy(sub, &str[start], end - start);
    return sub;
}

char *append_str(char *str, char append_char)
{
    size_t len = strlen(str);
    char *append = calloc(len + 2, sizeof(char));
    memcpy(append, str, len * sizeof(char));
    append[len] = append_char;
    return append;
}

int char_is_in_array(const char *array, char is_in, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (array[i] == is_in)
        {
            return 1;
        }
    }
    return 0;
}

int string_is_numeric(char *str)
{
    size_t len_str = strlen(str);
    for (int i = 0; i < len_str; i++)
    {
        if (!char_is_in_array(cALL_DIGITS, str[i], NUM_ALL_DIGITS))
        {
            return 0;
        }
    }
    return 1;
}

uint8_t *char_to_uint8(char *carr)
{
    int size = strlen(carr);
    uint8_t *ret = calloc(size, 1);
    for (int i = 0; i < size; i++)
    {
        ret[i] = (uint8_t)carr[i];
    }
    return ret;
}

int assert_file(char *message_arg)
{
    return strlen(message_arg) > FILE_PREFIX_LEN && string_starts_with(message_arg, FILE_PREFIX);
}

int assert_int(char *message_arg)
{
    return strlen(message_arg) > INT_PREFIX_LEN && string_starts_with(message_arg, INT_PREFIX);
}

uint8_t *to_int(char *arg)
{
    uint8_t *result = NULL;
    size_t size = 0;
    char *num = strtok(arg, ",");
    while (num != NULL)
    {
        char *base = substring(num, 0, BASE_PREFIX_NUM);
        if COMPARE_STR (base, PREFIX_BIN, 2)
        {
            if IS_STRLEN_BIGGER (num, MAX_BIN + 2)
            {
                ERR_OUT("Size of binary number should not exceed 8");
            }
            char *sans_base = substring(num, BASE_PREFIX_NUM, MAX_BIN + BASE_PREFIX_NUM);
            REALLOC_ARR(size, result);
            result[size - 1] = (uint8_t)strtol(sans_base, NULL, 2);
            free(base);
            free(sans_base);
        }
        else if COMPARE_STR (base, PREFIX_OCT, 2)
        {
            if IS_STRLEN_BIGGER (num, MAX_OCT + 2)
            {
                ERR_OUT("Size of octal number should not exceed 5");
            }
            char *sans_base = substring(num, BASE_PREFIX_NUM, MAX_OCT + BASE_PREFIX_NUM);
            REALLOC_ARR(size, result);
            result[size - 1] = (uint8_t)strtol(sans_base, NULL, 8);
            free(base);
            free(sans_base);
        }
        else if COMPARE_STR (num, PREFIX_HEX, 2)
        {
            if IS_STRLEN_BIGGER (num, MAX_HEX + 2)
            {
                ERR_OUT("Size of hexadecimal number should not exceed 2");
            }
            char *sans_base = substring(num, BASE_PREFIX_NUM, MAX_HEX + BASE_PREFIX_NUM);
            REALLOC_ARR(size, result);
            result[size - 1] = (uint8_t)strtol(sans_base, NULL, 16);
            free(base);
            free(sans_base);
        }
        else
        {
            if (!string_is_numeric(num))
            {
                ERR_OUT("With 'int=' prefix you must pass byte-sized integers in base 16, 8, 10 and 2");
            }
            unsigned long convt = (unsigned long)strtol(num, NULL, 10);
            if (convt > UINT8_MAX)
            {
                ERR_OUT("Given integer must be byte-sized (0-255)");
            }
            REALLOC_ARR(size, result);
            result[size - 1] = (uint8_t)convt;
            free(base);
        }
        //   printf("%u\n", (uint8_t)result[size - 1]);
        num = strtok(NULL, ",");
    }
    free(arg);
    return result;
}

char *join_args(int argc, char **argv)
{
    int final_len = 0;
    for (int i = 2; i < argc; i++)
    {
        final_len += strlen(argv[i]);
        final_len += 1;
    }

    char *ret = (char *)malloc(final_len);
    memset(ret, SPACE, final_len - 1);
    ret[final_len - 1] = 0;
    int curr_len = 0;
    int ret_cursor = 0;
    int warned = 0;
    for (int i = 2; i < argc; i++)
    {
        if (assert_file(argv[i]) && !warned)
        {
            printf("\033[1;33mWarning:\033[0m: The `filepath=` prefix is ignored in join mode\n");
            warned = 1;
        }
        curr_len = strlen(argv[i]);
        for (int j = 0; j < curr_len; j++)
        {
            ret[ret_cursor++] = argv[i][j];
        }
        ++ret_cursor;
    }

    return ret;
}

int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

uint8_t *read_given_file(char *fpath)
{
    FILE *fptr;
    long barray_len;
    uint8_t *bytearray;

    if (!is_regular_file((const char *)fpath))
    {
        ERR_OUT("Specfied file does not exist, or a directory. Pass `+` with only one argument to ignore");
    }

    fptr = fopen(fpath, "rb");

    fseek(fptr, 0, SEEK_END);
    barray_len = ftell(fptr);
    rewind(fptr);

    bytearray = calloc(barray_len + 1, sizeof(uint8_t));
    fread(bytearray, 1, barray_len, fptr);
    bytearray[barray_len] = '\0';

    fclose(fptr);

    return bytearray;
}

char *truncate_denotation(char *arg)
{
    size_t size = (strlen(arg) - FILE_PREFIX_LEN) + 1;
    char *ret = calloc(size, sizeof(char));
    memcpy(ret, &arg[FILE_PREFIX_LEN], size);
    return ret;
}

uint8_t *process_arg(char *arg)
{
    if (!assert_file(arg) && !assert_int(arg))
    {
        return char_to_uint8(arg);
    }
    else if (assert_int(arg))
    {
        return to_int(substring(arg, INT_PREFIX_LEN - 1, strlen(arg)));
    }

    char *fpath = truncate_denotation(arg);
    uint8_t *contents = read_given_file(fpath);
    free(fpath);
    return contents;
}

int main(int argc, char **argv)
{
    int len_flags = validate_flags(argc, argv);

    if (!arg_has_flag(argv[1], len_flags, FLAG_NHEADER))
    {
        printf("\033[1;30;47m   PoxHashRunner   |   Header-Only C   |  March 2023 - Chubak Bidpaa  |  MIT  \033[0m\n");
    }

    int echo_arg = arg_has_flag(argv[1], len_flags, FLAG_ECHO);
    poxdigest_t hashes[argc - 2];
    uint64_t total_time, t1, t2;
    total_time = 0;
    memset(hashes, 0, (argc - 2) * sizeof(poxdigest_t));
    if (arg_has_flag(argv[1], len_flags, FLAG_JOIN))
    {
        char *args_joined = join_args(argc, argv);
        if (echo_arg)
        {
            printf("Joined Args: \n`%s`\n", args_joined);
        }
        uint8_t *args_joined_uint8 = char_to_uint8(args_joined);
        t1 = get_time_in_ns();
        hashes[0] = pox_hash(args_joined_uint8);
        t2 = get_time_in_ns();
        free(args_joined);
        free(args_joined_uint8);
        print_hashes(hashes, 1, argv[1], len_flags, t2 - t1);
    }
    else
    {
        int cursor = 0;
        for (int i = 2; i < argc; i++)
        {
            if (echo_arg)
            {
                printf("Arg %d: %s\n", i - 1, argv[i]);
            }
            t1 = get_time_in_ns();
            uint8_t *arg_uint8 = process_arg(argv[i]);
            hashes[cursor++] = pox_hash(arg_uint8);
            t2 = get_time_in_ns();
            free(arg_uint8);
            total_time += t2 - t1;
        }
        print_hashes(hashes, argc - 2, argv[1], len_flags, total_time);
    }

    return 0;
}
#endif