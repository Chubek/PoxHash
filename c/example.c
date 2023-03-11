#ifndef __EXAMPLE_C
#define __EXAMPLE_C

#ifndef _POXIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif

#ifndef __INCLUDE_STDIO
#define __INCLUDE_STDIO
#include <stdio.h>
#endif
#ifndef __INDLUCE_TIME
#define __INCLUDE_TIME
#include <time.h>
#endif
#ifndef __INCLUDE_LIBPOX
#define __INCLUDE_LIBPOX
#include "libpoxh.h"
#endif

#define SEC_TO_US(sec) ((sec)*1000000)
#define NS_TO_US(ns)    ((ns)/1000)

#define MAX_FLAG_SIZE 24
#define MIN_FLAG_SIZE 3
#define HELP_FLAG_SIZE 3
#define SPACE 32
#define MIN_ARG_NUM 3

#define ERR_OUT(message)                                       \
    printf("\n");                                              \
    printf(message);                                           \
    printf("\n");                                              \
    printf("Please pass \033[1;34m-?-\033[0m to show help\n"); \
    exit(1)

typedef enum FLAGS
{
    FLAG_BENCHMARK = '^',
    FLAG_JOIN = '+',
    FLAG_EVERTHING = '*',
    FLAG_ALL_NON_DEC = '$',
    FLAG_ALL_DECIMAL = '&',
    FLAG_BYTES = 'B',
    FLAG_WORDS = 'W',
    FLAG_DOUBLES = 'D',
    FLAG_QUAD = 'Q',
    FLAG_SEX = 'g',
    FLAG_VIG = 'v',
    FLAG_HEX = 'h',
    FLAG_TET = 't',
    FLAG_DUO = 'd',
    FLAG_OCT = 'o',
    FLAG_SEN = 's',
    FLAG_BIN = 'b',
    FLAG_HELP = '?',
    FLAG_DASH = '-',
} flag_t;

void print_help(char *exec)
{
    printf("\n\033[1;33mPoxHash\033[0m Implementation in C --- Example Runner\n");
    printf("By Chubak Bidpaa; March 2023 -- GPLv3\n");
    printf("\n");
    printf("Examples \033[1m(flags go between two dashes!)\033[0m:\n");
    printf("%s -g^8o- myword1\n", exec);
    printf("%s -E+- mywod to be joined\n", exec);
    printf("%s -*E- word1 word 2\n", exec);
    printf("%s -htd- a_word\n", exec);
    printf("\n");
    printf("\033[1;32mFlags:\033[0m\n");
    printf("\033[1;35m\t`^`\033[0m: Benchmark run\n");
    printf("\033[1;35m\t`+`\033[0m: Join arguments\n");
    printf("\033[1;35m\t`*`\033[0m: Print every digest\n");
    printf("\033[1;35m\t`$`\033[0m: Print every non-decimal digest\n");
    printf("\033[1;35m\t`&`\033[0m: Print every decimal digest\n");
    printf("\033[1;35m\t`B`\033[0m: Print bytes digest\n");
    printf("\033[1;35m\t`W`\033[0m: Print words digest\n");
    printf("\033[1;35m\t`D`\033[0m: Print doubles digest\n");
    printf("\033[1;35m\t`Q`\033[0m: Print quad digest\n");
    printf("\033[1;35m\t`g`\033[0m: Print sexagesimal digest\n");
    printf("\033[1;35m\t`v`\033[0m: Print viggesimal digest\n");
    printf("\033[1;35m\t`h`\033[0m: Print hexadecimal digest\n");
    printf("\033[1;35m\t`t`\033[0m: Print tetradecimal digest\n");
    printf("\033[1;35m\t`d`\033[0m: Print duodecimal digest\n");
    printf("\033[1;35m\t`o`\033[0m: Print octal digest\n");
    printf("\033[1;35m\t`s`\033[0m: Print senary digest\n");
    printf("\033[1;35m\t`b`\033[0m: Print binary digest\n");
    printf("\033[1;35m\t`?`\033[0m: Print Help\n\n");
    free(exec);
    exit(1);
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

int arg_has_flag(char *arg, int len_flags, flag_t must_have)
{
    for (int i = 1; i < len_flags - 1; i++)
    {
        if (arg[i] == must_have)
        {
            return 1;
        }
    }
    return 0;
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

    if (argv[1][0] != '-' || argv[1][len_flags - 1] != '-')
    {
        ERR_OUT("The flag argument must begin and end with `-`");
    }

    char *exec_name = get_exec_name(argv[0]);
    if (!(strncmp(argv[1], "-?-", HELP_FLAG_SIZE)))
        print_help(exec_name);

    int help_passed = arg_has_flag(argv[1], len_flags, FLAG_HELP);
    if (help_passed && len_flags > MIN_FLAG_SIZE)
    {
        ERR_OUT("You may not pass the `?` flag along with other flags");
    }

    if (argc < MIN_ARG_NUM)
    {
        ERR_OUT("You must pass at least one argument to hash");
    }

    int all_flags_passed = arg_has_flag(argv[1], len_flags, FLAG_EVERTHING);
    int all_flags_dec_passed = arg_has_flag(argv[1], len_flags, FLAG_ALL_DECIMAL);
    int all_flags_nondec_passed = arg_has_flag(argv[1], len_flags, FLAG_ALL_NON_DEC);

    for (int i = 1; i < len_flags - 1; i++)
    {
        switch (argv[1][i])
        {
        case FLAG_BENCHMARK:
            continue;
        case FLAG_JOIN:
            continue;
        case FLAG_EVERTHING:
            if (all_flags_dec_passed || all_flags_nondec_passed)
            {
                ERR_OUT("You may not pass `*` when you have passed `$` or `&`");
            }
            continue;
        case FLAG_ALL_NON_DEC:
            if (all_flags_passed)
            {
                ERR_OUT("You may not pass `$` when `*` is passed");
            }
            continue;
        case FLAG_ALL_DECIMAL:
            if (all_flags_passed)
            {
                ERR_OUT("You may not pass `&` when `*` is passed");
            }
            continue;
        case FLAG_BYTES:
            if (all_flags_dec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a decimal digest flag when `*` or `&` is passed");
            }
            continue;
        case FLAG_WORDS:
            if (all_flags_dec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a decimal digest flag when `*` or `&` is passed");
            }
            continue;
        case FLAG_DOUBLES:
            if (all_flags_dec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a decimal digest flag when `*` or `&` is passed");
            }
            continue;
        case FLAG_QUAD:
            if (all_flags_dec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a decimal digest flag when `*` or `&` is passed");
            }
            continue;
        case FLAG_SEX:
            if (all_flags_nondec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a non-decimal digest flag when `*` or `$` is passed");
            }
            continue;
        case FLAG_VIG:
            if (all_flags_nondec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a non-decimal digest flag when `*` or `$` is passed");
            }
            continue;
        case FLAG_HEX:
            if (all_flags_nondec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a non-decimal digest flag when `*` or `$` is passed");
            }
            continue;
        case FLAG_TET:
            if (all_flags_nondec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a non-decimal digest flag when `*` or `$` is passed");
            }
            continue;
        case FLAG_DUO:
            if (all_flags_nondec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a non-decimal digest flag when `*` or `$` is passed");
            }
            continue;
        case FLAG_OCT:
            if (all_flags_nondec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a non-decimal digest flag when `*` or `$` is passed");
            }
            continue;
        case FLAG_SEN:
            if (all_flags_nondec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a non-decimal digest flag when `*` or `$` is passed");
            }
            continue;
        case FLAG_BIN:
            if (all_flags_nondec_passed || all_flags_passed)
            {
                ERR_OUT("You may not pass a non-decimal digest flag when `*` or `$` is passed");
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
    for (int i = 2; i < argc; i++)
    {
        curr_len = strlen(argv[i]);
        for (int j = 0; j < curr_len; j++)
        {
            ret[ret_cursor++] = argv[i][j];
        }
        ++ret_cursor;
    }

    return ret;
}

uint64_t get_time_in_us()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    uint64_t us = SEC_TO_US((uint64_t)ts.tv_sec) + NS_TO_US((uint64_t)ts.tv_nsec);
    return us;
}

int all_are_false(int *arr, int size)
{
    for (int i = 0; i < size; i++) {
        if (arr[i] == 1) {
            return 0;
        }
    }
    return 1;
}

void print_hashes(poxhash_t *hashes, int len_hashes, char *flags, int len_flags, uint64_t total_time, char *joined)
{
    if (arg_has_flag(flags, len_flags, FLAG_BENCHMARK))
        printf("Total microseconds spent for hashing %d bytestring(s): %luus\n", len_hashes, total_time);

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
    int duo = arg_has_flag(flags, len_flags, FLAG_TET);
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
        ERR_OUT("You have not specfied any digests to be printed. Please pass at least one, or `*` for all");
    }

    for (int i = 0; i < len_hashes; i++)
    {
        printf("\nRequested digests for byte string #%u%s\n", i + 1, joined);
        if (everything || all_flags_decimal || byte)
            printf("\tBytes: U8[%hu, %u, %u, %u, %u, %u, %u, %u]\n", hashes[i].bytes[0], hashes[i].bytes[1], hashes[i].bytes[2], hashes[i].bytes[3], hashes[i].bytes[4], hashes[i].bytes[5], hashes[i].bytes[6], hashes[i].bytes[7]);
        if (everything || all_flags_decimal || word)
            printf("\tWords: U16[%hu, %hu, %hu, %hu]\n", hashes[i].words[0], hashes[i].words[1], hashes[i].words[2], hashes[i].words[3]);
        if (everything || all_flags_decimal || dub)
            printf("\tdoubles: U32[%u, %u]\n", hashes[i].doubles[0], hashes[i].doubles[1]);
        if (everything || all_flags_decimal || quad)
            printf("\tQuad: U64[%lu]\n", hashes[i].quad);
        if (everything || all_flags_non_decimal || sex)
            printf("\tSexdigest: %s\n", hashes[i].sexdigest);
        if (everything || all_flags_non_decimal || vig)
            printf("\tVigdigest: %s\n", hashes[i].vigdigest);
        if (everything || all_flags_non_decimal || hex)
            printf("\tHexdigest: %s\n", hashes[i].hexdigest);
        if (everything || all_flags_non_decimal || tet)
            printf("\tTetdigest: %s\n", hashes[i].tetdigest);
        if (everything || all_flags_non_decimal || duo)
            printf("\tDuodigest: %s\n", hashes[i].duodigest);
        if (everything || all_flags_non_decimal || oct)
            printf("\tOctdigest: %s\n", hashes[i].octdigest);
        if (everything || all_flags_non_decimal || sen)
            printf("\tSendgiest: %s\n", hashes[i].sendigest);
        if (everything || all_flags_non_decimal || bin)
            printf("\tBindigest: %s\n", hashes[i].bindigest);
        printf("----\n\n");
    }
    printf("\nFinished run for PoxHash example code (C implementation)\n");
}

int main(int argc, char **argv)
{
    int len_flags = validate_flags(argc, argv);
    printf("\n");

    poxhash_t hashes[argc - 2];
    memset(hashes, 0, (argc - 2) * sizeof(poxhash_t));
    if (arg_has_flag(argv[1], len_flags, FLAG_JOIN))
    {
        char *args_joined = join_args(argc, argv);
        time_t t1 = get_time_in_us();
        hashes[0] = pox_hash(args_joined);
        time_t t2 = get_time_in_us();
        free(args_joined);
        print_hashes(hashes, 1, argv[1], len_flags, t2 - t1, " (joined arguments):");
    }
    else
    {
        uint64_t total_time, t1, t2;
        int cursor = 0;
        for (int i = 2; i < argc; i++)
        {
            t1 = get_time_in_us();
            hashes[cursor++] = pox_hash(argv[i]);
            t2 = get_time_in_us();
            total_time += t2 - t1;
        }
        print_hashes(hashes, argc - 2, argv[1], len_flags, total_time, ":");
    }

    return 0;
}
#endif