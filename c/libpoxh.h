//////////////////////////////////////////////
//      PoxHash v1                          //
//      Implementation in Header-Only C     //
//      By Chubak Bidpaa - March 2023       //
//      License: GPLv3                      //
//////////////////////////////////////////////

#ifndef __POX_H
#define __POX_H

#ifndef __POX_HEADERS_STRING
#define __POX_HEADERS_STRING
#include <string.h>
#endif
#ifndef __POX_HEADERS_STDINT
#define __POX_HEADERS_STDINT
#include <stdint.h>
#endif
#ifndef __POX_HEADERS_STDLIB
#define __POX_HEADERS_STDLIB
#include <stdlib.h>
#endif

#define POX_ROUND_PRIME_NUM 90
#define POX_BLOCK_NUM 64
#define POX_8B_PRIME_NUM 54
#define POX_ROUND_NUM 31
#define POX_CHUNK_NUM 16
#define POX_PORTION_NUM 4
#define POX_SD_PRIME_NUM 3
#define POX_MAGIC_PRIME_NUM 2

#define MASK_DWORD_4F4Z 0xffff0000
#define MASK_DWORD_4Z4F 0x0000ffff
#define MASK_WORD_FZFZ 0xf0f0
#define MASK_WORD_ZFZF 0x0f0f
#define MASK_WORD_FZZZ 0xf000
#define MASK_WORD_ZFZZ 0x0f00
#define MASK_WORD_ZZFZ 0x00f0
#define MASK_WORD_ZZZF 0x000f
#define MASK_WORD_ZZFF 0x00ff
#define MASK_WORD_FFZZ 0xff00
#define MASK_WORD_FZZF 0xf00f
#define MASK_WORD_ZFFZ 0x0ff0
#define MASK_WORD_FFFZ 0xfff0
#define MASK_WORD_ZFFF 0x0fff
#define MASK_WORD_ZFZF 0x0f0f
#define MASK_NIBBLET_01 0b01
#define MASK_NIBBLET_10 0b10
#define MASK_NIBBLET_11 0b11
#define MASK_NIBBLET_00 0b00

#define WORD_WIDTH 16
#define BYTE_WIDTH 8
#define SEX_SIZE 3
#define VIG_SIZE 4
#define HEX_SIZE 4
#define TET_SIZE 5
#define DUO_SIZE 5
#define OCT_SIZE 6
#define SEN_SIZE 7
#define BIN_SIZE 16
#define SEX_BASE 60
#define VIG_BASE 20
#define HEX_BASE 16
#define TET_BASE 14
#define DUO_BASE 12
#define OCT_BASE 8
#define SEN_BASE 6
#define BIN_BASE 2
#define BYTE_SIZE POX_BLOCK_NUM / 8
#define SIZE_BIONOM 6
#define SIZE_WORD_ARR(num) sizeof(uint16_t) * num
#define SIZE_WORD sizeof(uint16_t)
#define SIZE_BYTE_ARR(num) sizeof(char) * num
#define SIZE_BYTE sizeof(char)

static const uint16_t cPOX_ROUND_PRIMES[POX_ROUND_PRIME_NUM] = {
    0x0377,
    0x0683,
    0x05fb,
    0x05fb,
    0x0665,
    0x074b,
    0x036d,
    0x033d,
    0x0115,
    0x07cf,
    0x0e59,
    0x0e75,
    0x0a75,
    0x119b,
    0x1073,
    0x12b3,
    0x0fd1,
    0x0a75,
    0x0de7,
    0x10bb,
    0x18d1,
    0x1c99,
    0x1723,
    0x1cc9,
    0x20c3,
    0x2327,
    0x2063,
    0x215b,
    0x17e1,
    0x22bd,
    0xf2ff,
    0xf50b,
    0xf4af,
    0xf2b3,
    0xf5fb,
    0xf4af,
    0xf2b9,
    0xf38b,
    0xf4c3,
    0xf5db,
    0x1039,
    0x1003,
    0x0fa1,
    0x0fa3,
    0x0fa7,
    0x8687,
    0x80db,
    0x86d1,
    0x7fcd,
    0x7f43,
    0xa10b,
    0x9e81,
    0x9d15,
    0xa289,
    0xa279,
    0x3e11,
    0x3aa5,
    0x3be3,
    0x3daf,
    0x3bff,
    0xff8f,
    0xff71,
    0xfe03,
    0xfe41,
    0xfe05,
    0xff2f,
    0xfe7b,
    0xfeb3,
    0x0409,
    0x0481,
    0x1d7b,
    0x1c4f,
    0x1e6d,
    0x1b7f,
    0x1e71,
    0xe875,
    0xe2cd,
    0xe351,
    0xe363,
    0xe329,
    0x049d,
    0x0427,
    0xcbb3,
    0x184d,
    0x2ce1,
    0x8861,
    0x59b3,
    0x2077,
    0xff9d,
    0xff2f,
};
static const uint16_t cPOX_8B_PRIMES[POX_8B_PRIME_NUM] = {
    0x2, 0x3, 0x5, 0x7, 0xb, 0xd, 0x11, 0x13, 0x17, 0x1d, 0x1f, 0x25, 0x29,
    0x2b, 0x2f, 0x35, 0x3b, 0x3d, 0x43, 0x47, 0x49, 0x4f, 0x53, 0x59, 0x61,
    0x65, 0x67, 0x6b, 0x6d, 0x71, 0x7f, 0x83, 0x89, 0x8b, 0x95, 0x97, 0x9d,
    0xa3, 0xa7, 0xad, 0xb3, 0xb5, 0xbf, 0xc1, 0xc5, 0xc7, 0xd3, 0xdf, 0xe3,
    0xe5, 0xe9, 0xef, 0xf1, 0xfb};
static const uint16_t cPOX_SINGLE_DIGIT_PRIMES[POX_SD_PRIME_NUM] = {0x3, 0x5, 0x7};
static const uint16_t cPOX_MAGIC_PRIMES[POX_MAGIC_PRIME_NUM] = {0x33, 0x65};

static const uint16_t cPOX_PRIME_INIT_A = 0x17cb;
static const uint16_t cPOX_PRIME_INIT_B = 0x0371;
static const uint16_t cPOX_PRIME_INIT_C = 0x2419;
static const uint16_t cPOX_PRIME_INIT_D = 0xf223;

static const char cSEX_CHARS[SEX_BASE] = {
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
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'L',
    'M',
    'N',
    'O',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z',
    'a',
    'b',
    'c',
    'd',
    'e',
    'f',
    'g',
    'h',
    'i',
    'j',
    'k',
    'l',
    'm',
    'n',
    'o',
    'p',
    'q',
    'r',
    's',
    't',
    'u',
    'v',
    'w',
    'x',
};
static const char cVIG_CHARS[VIG_BASE] = {
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    '@',
    '^',
    '&',
    '*',
    '$',
    '+',
    '!',
    ';',
    ':',
    '~',
};
static const char cHEX_CHARS[HEX_BASE] = {
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
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
};
static const char cTET_CHARS[TET_BASE] = {
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
    'T',
    'E',
    'W',
    'R',
};
static const char cDUO_CHARS[DUO_BASE] = {
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
    '*',
    '#',
};
static const char cOCT_CHARS[OCT_BASE] = {
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
};
static const char cSEN_CHARS[SEN_BASE] = {
    '0', '1', '2', '3', '4', '5'};
static const char cBIN_CHARS[BIN_BASE] = {'0', '1'};
static const size_t cRANGE_ZTF[4] = {0, 1, 2, 3};
static const size_t cCOMB_BIONOM[6][2] = {
    {0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}};

static inline uint16_t log2n(uint16_t num)
{
    return (num > 1) ? (1 + log2n(num / 2)) : 0;
}

static inline uint16_t *copy_to_temp(uint16_t *src)
{
    uint16_t *dst = (uint16_t *)calloc(POX_PORTION_NUM, SIZE_WORD);
    memcpy(dst, src, SIZE_WORD_ARR(POX_PORTION_NUM));
    return dst;
}

static inline uint16_t get_8b_prime(uint16_t num)
{
    uint16_t remainder = num % POX_8B_PRIME_NUM;
    return cPOX_8B_PRIMES[remainder];
}

#define PAD_SIZE(strsize)                \
    while (strsize % POX_BLOCK_NUM != 0) \
    {                                    \
        strsize++;                       \
    }

#define COPY_WORDS_TO_SUBARRAY(wordarr, subarr, start, end) \
    int __jz = 0;                                           \
    for (int __iz = start; __iz < end; __iz++)              \
    {                                                       \
        subarr[__jz++] = wordarr[__iz];                     \
    }

#define SWAP(a, b, T) \
    do                \
    {                 \
        T __tmp = a;  \
        a = b;        \
        b = __tmp;    \
    } while (0)

#define OMEGA(num) \
    num = (num & MASK_DWORD_4F4Z) >> WORD_WIDTH

#define EPSILON(num) \
    num &= MASK_DWORD_4Z4F

#define LAMED(num, by) \
    num = (num << by) | (num >> (WORD_WIDTH - by))

#define GORDA(num, by)                     \
    do                                     \
    {                                      \
        uint32_t __numcpy = (uint32_t)num; \
        LAMED(__numcpy, by);               \
        if (__numcpy > UINT16_MAX)         \
        {                                  \
            OMEGA(__numcpy);               \
        }                                  \
        num = (uint16_t)__numcpy;          \
    } while (0)

#define TASU(a, b, ptr)                          \
    do                                           \
    {                                            \
        uint32_t __a_ttb = (uint32_t)a;          \
        uint32_t __b_ttb = (uint32_t)b;          \
        uint32_t __a_plus_b = __a_ttb + __b_ttb; \
        if (__a_plus_b > UINT16_MAX)             \
            EPSILON(__a_plus_b);                 \
        *ptr = (uint16_t)__a_plus_b;             \
    } while (0)

#define WORD_TO_2BYTE(word, bytelow, bytehigh) \
    bytelow = word & MASK_WORD_ZZFF;           \
    bytehigh = (word & MASK_WORD_FFZZ) >> BYTE_WIDTH;

#define SUM_DOUBLE(array, double, size)         \
    do                                          \
    {                                           \
        for (int __kz = 0; __kz < size; __kz++) \
        {                                       \
            double += array[__kz];              \
        }                                       \
    } while (0)

#define SUM_DOUBLE_WEIGHTS(array, weights, double, size) \
    do                                                   \
    {                                                    \
        for (int __kz = 0; __kz < size; __kz++)          \
        {                                                \
            double += array[__kz] * weights[__kz];       \
        }                                                \
    } while (0)

#define CENTUM(arr, weights, res)                               \
    do                                                          \
    {                                                           \
        uint32_t sum = 0;                                       \
        SUM_DOUBLE_WEIGHTS(arr, weights, sum, POX_PORTION_NUM); \
        sum /= POX_PORTION_NUM;                                 \
        if (sum > UINT16_MAX)                                   \
        {                                                       \
            OMEGA(sum);                                         \
        }                                                       \
        res = (uint16_t)sum;                                    \
    } while (0)

#define SATEM(arr, weights, res)                                \
    do                                                          \
    {                                                           \
        uint32_t sum = 0;                                       \
        SUM_DOUBLE_WEIGHTS(arr, weights, sum, POX_PORTION_NUM); \
        sum = (sum + 1) / 2;                                    \
        if (sum > UINT16_MAX)                                   \
        {                                                       \
            EPSILON(sum);                                       \
        }                                                       \
        res = (uint16_t)sum;                                    \
    } while (0)

#define TAMAAM(arr, res)                       \
    do                                         \
    {                                          \
        uint32_t sum = 0;                      \
        SUM_DOUBLE(arr, sum, POX_PORTION_NUM); \
        sum /= POX_PORTION_NUM;                \
        if (sum > UINT16_MAX)                  \
        {                                      \
            OMEGA(sum);                        \
        }                                      \
        res = (uint16_t)sum;                   \
    } while (0)

#define DECA(arr, res)                         \
    do                                         \
    {                                          \
        uint32_t sum = 0;                      \
        SUM_DOUBLE(arr, sum, POX_PORTION_NUM); \
        sum = (sum + 1) / 2;                   \
        if (sum > UINT16_MAX)                  \
        {                                      \
            EPSILON(sum);                      \
        }                                      \
        res = (uint16_t)sum;                   \
    } while (0)

#define MIN_ARGMIN(arr, min, minindex)                 \
    min = arr[0];                                      \
    minindex = 0;                                      \
    for (int __it = 1; __it < POX_PORTION_NUM; __it++) \
    {                                                  \
        if (arr[__it] < min)                           \
        {                                              \
            min = arr[__it];                           \
            minindex = __it;                           \
        }                                              \
    }

#define MAX_ARGMAX(arr, max, maxindex)                 \
    max = arr[0];                                      \
    maxindex = 0;                                      \
    for (int __ir = 1; __ir < POX_PORTION_NUM; __ir++) \
    {                                                  \
        if (arr[__ir] > max)                           \
        {                                              \
            max = arr[__ir];                           \
            maxindex = __ir;                           \
        }                                              \
    }

#define PAD_SIZE_TO_BLOCK_SIZE(size)  \
    while (size % POX_BLOCK_NUM != 0) \
        ++size;

#define FACTORS_TO_BYTEARR(warr, barr)        \
    WORD_TO_2BYTE(warr[0], barr[0], barr[1]); \
    WORD_TO_2BYTE(warr[1], barr[2], barr[3]); \
    WORD_TO_2BYTE(warr[2], barr[4], barr[5]); \
    WORD_TO_2BYTE(warr[3], barr[6], barr[7]);

#define DECIMAL_TO_BASE(base, size, chars, res, dec, offset)                         \
    do                                                                               \
    {                                                                                \
        int decimal = dec;                                                           \
        for (int __mx = ((size * offset) + size) - 1; __mx >= size * offset; __mx--) \
        {                                                                            \
            res[__mx] = chars[decimal % base];                                       \
            decimal /= base;                                                         \
        }                                                                            \
    } while (0)

#define WORD_TO_BASE(words, base, size, chars, res)                     \
    do                                                                  \
    {                                                                   \
        for (int __fx = 0; __fx < POX_PORTION_NUM; __fx++)              \
        {                                                               \
            DECIMAL_TO_BASE(base, size, chars, res, words[__fx], __fx); \
        }                                                               \
    } while (0)

#define FACTORS_TO_BASEDIGEST(warr, sexdigest, vigdigest, hexdigest, tetdigest, duodigest, octdigest, sendigest, bindigest) \
    WORD_TO_BASE(warr, SEX_BASE, SEX_SIZE, cSEX_CHARS, sexdigest);                                                          \
    WORD_TO_BASE(warr, VIG_BASE, VIG_SIZE, cVIG_CHARS, vigdigest);                                                          \
    WORD_TO_BASE(warr, HEX_BASE, HEX_SIZE, cHEX_CHARS, hexdigest);                                                          \
    WORD_TO_BASE(warr, TET_BASE, TET_SIZE, cTET_CHARS, tetdigest);                                                          \
    WORD_TO_BASE(warr, DUO_BASE, DUO_SIZE, cDUO_CHARS, duodigest);                                                          \
    WORD_TO_BASE(warr, OCT_BASE, OCT_SIZE, cOCT_CHARS, octdigest);                                                          \
    WORD_TO_BASE(warr, SEN_BASE, SEN_SIZE, cSEN_CHARS, sendigest);                                                          \
    WORD_TO_BASE(warr, BIN_BASE, BIN_SIZE, cBIN_CHARS, bindigest);

#define WORD_TO_DUOBLE_WORD(w1, w2, d) \
    d |= (uint32_t)w1;                 \
    d |= ((uint32_t)w2) << 16;

#define FACTORS_TO_DUOBLEARR(warr, darr)            \
    WORD_TO_DUOBLE_WORD(warr[0], warr[1], darr[0]); \
    WORD_TO_DUOBLE_WORD(warr[2], warr[3], darr[1]);

#define FACTPRS_TO_QUAD(warr, quad)    \
    quad |= (uint64_t)warr[0];         \
    quad |= ((uint64_t)warr[1]) << 16; \
    quad |= ((uint64_t)warr[2]) << 32; \
    quad |= ((uint64_t)warr[3]) << 48;

#define POX_ALPHA(temp_array)                                          \
    uint16_t aleph = (temp_array[0] ^ temp_array[1]) & MASK_WORD_ZZFF; \
    uint16_t daal = (temp_array[2] ^ temp_array[3]) & MASK_WORD_FFZZ;  \
    uint16_t theh = (aleph | daal) % cPOX_8B_PRIMES[0];                \
    uint16_t gaaf = (aleph ^ daal) % cPOX_8B_PRIMES[1];                \
    temp_array[0] >>= theh;                                            \
    temp_array[1] >>= ((theh + gaaf) % 2) + 1;                         \
    temp_array[2] >>= gaaf;

#define POX_DELTA(temp_array)                                                                 \
    uint16_t alaf = (temp_array[0] ^ MASK_WORD_FFFZ) % get_8b_prime(temp_array[0]);           \
    uint16_t dalat = (temp_array[1] ^ MASK_WORD_FZZF) % get_8b_prime(temp_array[1]);          \
    uint16_t tit = (temp_array[2] & MASK_WORD_ZFFF) % get_8b_prime(temp_array[2]);            \
    uint16_t gaman = (temp_array[3] & MASK_WORD_FFZZ) % get_8b_prime(temp_array[3]);          \
    for (int ___qz = 0; ___qz < POX_PORTION_NUM; ___qz++)                                     \
    {                                                                                         \
        alaf >>= cPOX_SINGLE_DIGIT_PRIMES[dalat % POX_SD_PRIME_NUM];                          \
        GORDA(dalat, 2);                                                                      \
        tit >>= cPOX_SINGLE_DIGIT_PRIMES[gaman % POX_SD_PRIME_NUM];                           \
        gaman ^= (alaf ^ MASK_WORD_ZZFF) >> cPOX_SINGLE_DIGIT_PRIMES[tit % POX_SD_PRIME_NUM]; \
    }                                                                                         \
    temp_array[1] ^= temp_array[2] % cPOX_MAGIC_PRIMES[alaf % POX_MAGIC_PRIME_NUM];           \
    temp_array[2] ^= alaf + tit;                                                              \
    temp_array[3] ^= tit + gaman;

#define POX_THETA(temp_array)                                             \
    uint16_t alef = temp_array[0] % 2;                                    \
    uint16_t dalet = temp_array[1] % 2;                                   \
    uint16_t tet = temp_array[2] % 2;                                     \
    uint16_t gimmel = temp_array[3] % 2;                                  \
    uint16_t ctm, sdm;                                                    \
    uint16_t weights[POX_PORTION_NUM] = {alef, dalet, tet, gimmel};       \
    ctm = 0;                                                              \
    sdm = 0;                                                              \
    CENTUM(temp_array, weights, ctm);                                     \
    SATEM(temp_array, weights, sdm);                                      \
    temp_array[0] ^= ((ctm >> gimmel) ^ MASK_WORD_ZZFF) & MASK_WORD_ZZZF; \
    temp_array[3] ^= ((sdm << alef) ^ MASK_WORD_FZFZ) & MASK_WORD_FZZZ;

#define POX_GAMMA(temp_array)                                                     \
    uint16_t mmin, argmin, mmax, argmax, ay, dee, thorn, gee;                     \
    MIN_ARGMIN(temp_array, mmin, argmin);                                         \
    MAX_ARGMAX(temp_array, mmax, argmax);                                         \
    ay = argmin & MASK_NIBBLET_01;                                                \
    dee = argmax ^ MASK_NIBBLET_10;                                               \
    thorn = argmin & MASK_NIBBLET_11;                                             \
    gee = argmax ^ MASK_NIBBLET_00;                                               \
    uint16_t alaph = temp_array[ay] % get_8b_prime(temp_array[thorn]);            \
    uint16_t dalath = (get_8b_prime(mmax) ^ MASK_WORD_ZFZF) % get_8b_prime(mmin); \
    uint16_t teth = mmax % get_8b_prime(mmax);                                    \
    uint16_t gamal = temp_array[dee] % get_8b_prime((mmin + mmax) / 2);           \
    temp_array[ay] >>= (alaph ^ MASK_WORD_ZZFZ) % WORD_WIDTH;                     \
    temp_array[dee] >>= (gamal ^ MASK_WORD_FZZZ) % ((mmax % 2) + 1);              \
    temp_array[thorn] ^= log2n(dalath) & MASK_WORD_ZFFF;                          \
    temp_array[gee] ^= (uint16_t)log2n(teth) >> ((gamal % 2) + 1);

#define POX_ALPHA_WRAP(temp_array) \
    do                             \
    {                              \
        POX_ALPHA(temp_array);     \
    } while (0)
#define POX_DELTA_WRAP(temp_array) \
    do                             \
    {                              \
        POX_DELTA(temp_array);     \
    } while (0)
#define POX_THETA_WRAP(temp_array) \
    do                             \
    {                              \
        POX_THETA(temp_array);     \
    } while (0)
#define POX_GAMMA_WRAP(temp_array) \
    do                             \
    {                              \
        POX_GAMMA(temp_array);     \
    } while (0)

#define POX_ROUND_ALPHABET(temp_array) \
    POX_ALPHA_WRAP(temp_array);        \
    POX_DELTA_WRAP(temp_array);        \
    POX_THETA_WRAP(temp_array);        \
    POX_GAMMA_WRAP(temp_array);

#define POX_APPLY_PRIME(temp_array, pnum) \
    do                                    \
    {                                     \
        temp_array[0] %= pnum;            \
        temp_array[1] %= pnum;            \
        temp_array[2] %= pnum;            \
        temp_array[3] %= pnum;            \
    } while (0)

#define POX_ROUND_APPLY_PRIME(temp_array)                     \
    for (int __iw = 0; __iw < POX_ROUND_PRIME_NUM; __iw++)    \
    {                                                         \
        POX_APPLY_PRIME(temp_array, cPOX_ROUND_PRIMES[__iw]); \
    }

#define POX_ADD_TEMP_TO_FACT_INDEX(factor_array, temp_array, index)         \
    do                                                                      \
    {                                                                       \
        TASU(factor_array[index], temp_array[index], &factor_array[index]); \
    } while (0)

#define POX_ROUND_ADD_TEMP_TO_FACTS(factor_array, temp_array) \
    POX_ADD_TEMP_TO_FACT_INDEX(factor_array, temp_array, 0);  \
    POX_ADD_TEMP_TO_FACT_INDEX(factor_array, temp_array, 1);  \
    POX_ADD_TEMP_TO_FACT_INDEX(factor_array, temp_array, 2);  \
    POX_ADD_TEMP_TO_FACT_INDEX(factor_array, temp_array, 3);

#define POX_ROUND_APPLY_SHUFFLE(temp_array)                                                   \
    for (int __iy = 0; __iy < SIZE_BIONOM; __iy++)                                            \
    {                                                                                         \
        SWAP(temp_array[cCOMB_BIONOM[__iy][0]], temp_array[cCOMB_BIONOM[__iy][1]], uint16_t); \
    }

#define POX_ROUND(factor_array)                                                                                  \
    uint16_t temp_array[POX_PORTION_NUM] = {factor_array[0], factor_array[1], factor_array[2], factor_array[3]}; \
    POX_ROUND_ALPHABET(temp_array);                                                                              \
    POX_ROUND_APPLY_PRIME(temp_array);                                                                           \
    POX_ROUND_APPLY_SHUFFLE(temp_array);                                                                         \
    POX_ROUND_ADD_TEMP_TO_FACTS(factor_array, temp_array);

#define POX_APPLY_BYTES(factor_array, portion, index)                       \
    do                                                                      \
    {                                                                       \
        uint16_t tmt, dca, odd_factor_tmt, odd_factor_dca;                  \
        uint16_t ng, chu, yo, eo, zam, pez, dit, kit;                       \
        tmt = 0;                                                            \
        dca = 0;                                                            \
        TAMAAM(portion, tmt);                                               \
        DECA(portion, dca);                                                 \
        odd_factor_tmt = UINT16_MAX * (tmt % 2);                            \
        odd_factor_dca = UINT16_MAX * (dca % 2);                            \
        ng = (portion[0] + index) % POX_PORTION_NUM;                        \
        chu = (portion[1] + index) % POX_PORTION_NUM;                       \
        yo = (portion[2] + index) % POX_PORTION_NUM;                        \
        eo = (portion[3] + index) % POX_PORTION_NUM;                        \
        zam = portion[0] % cPOX_8B_PRIMES[portion[chu] % POX_8B_PRIME_NUM]; \
        pez = portion[1] % cPOX_8B_PRIMES[portion[yo] % POX_8B_PRIME_NUM];  \
        dit = portion[2] % cPOX_8B_PRIMES[portion[eo] % POX_8B_PRIME_NUM];  \
        kit = portion[3] % cPOX_8B_PRIMES[portion[ng] % POX_8B_PRIME_NUM];  \
        factor_array[ng] ^= ((portion[eo] | tmt) ^ odd_factor_dca) | zam;   \
        factor_array[chu] ^= ((portion[yo] & dca) ^ odd_factor_tmt) ^ pez;  \
        factor_array[yo] ^= ((portion[chu] ^ tmt) ^ odd_factor_dca) | dit;  \
        factor_array[eo] ^= ((portion[ng] | dca) ^ odd_factor_tmt) ^ kit;   \
    } while (0)

#define POX_ROUND_ACTION(factor_array, portion, index) \
    POX_APPLY_BYTES(factor_array, portion, index);     \
    POX_ROUND(factor_array);

#define POX_PROCESS_APPLY(factor_arry, block_array, portion_array, pstart, pend) \
    do                                                                           \
    {                                                                            \
        COPY_WORDS_TO_SUBARRAY(block_array, portion_array, pstart, pend);        \
        for (uint16_t __ie = 0; __ie < POX_ROUND_NUM; __ie++)                    \
        {                                                                        \
            POX_ROUND_ACTION(factor_array, portion_array, __ie);                 \
        }                                                                        \
    } while (0)

#define POX_PROCESS_BLOCK(factor_array, message, block_array, portion_array, bstart, bend)             \
    COPY_WORDS_TO_SUBARRAY(message, block_array, bstart, bend);                                        \
    for (int __ip = 0; __ip < POX_BLOCK_NUM; __ip += POX_CHUNK_NUM)                                    \
    {                                                                                                  \
        for (int __jt = __ip; __jt < __ip + POX_CHUNK_NUM; __jt += POX_PORTION_NUM)                    \
        {                                                                                              \
            POX_PROCESS_APPLY(factor_array, block_array, portion_array, __jt, __jt + POX_PORTION_NUM); \
        }                                                                                              \
    }

typedef struct PoxDigest
{
    char sexdigest[(SEX_SIZE * 4) + 1];
    char vigdigest[(VIG_SIZE * 4) + 1];
    char hexdigest[(HEX_SIZE * 4) + 1];
    char tetdigest[(TET_SIZE * 4) + 1];
    char duodigest[(DUO_SIZE * 4) + 1];
    char octdigest[(OCT_SIZE * 4) + 1];
    char sendigest[(SEN_SIZE * 4) + 1];
    char bindigest[(BIN_SIZE * 4) + 1];
    uint8_t bytes[BYTE_SIZE];
    uint16_t words[POX_PORTION_NUM];
    uint32_t doubles[POX_PORTION_NUM / 2];
    uint64_t quad;
} poxhash_t;

#define INIT_POXHASH(poxhash, factor_array)                                                                                                                                                      \
    memset(poxhash.sexdigest, 0, SIZE_BYTE_ARR((SEX_SIZE * 4) + 1));                                                                                                                             \
    memset(poxhash.vigdigest, 0, SIZE_BYTE_ARR((VIG_SIZE * 4) + 1));                                                                                                                             \
    memset(poxhash.hexdigest, 0, SIZE_BYTE_ARR((HEX_SIZE * 4) + 1));                                                                                                                             \
    memset(poxhash.tetdigest, 0, SIZE_BYTE_ARR((TET_SIZE * 4) + 1));                                                                                                                             \
    memset(poxhash.duodigest, 0, SIZE_BYTE_ARR((DUO_SIZE * 4) + 1));                                                                                                                             \
    memset(poxhash.octdigest, 0, SIZE_BYTE_ARR((OCT_SIZE * 4) + 1));                                                                                                                             \
    memset(poxhash.sendigest, 0, SIZE_BYTE_ARR((SEN_SIZE * 4) + 1));                                                                                                                             \
    memset(poxhash.bindigest, 0, SIZE_BYTE_ARR((BIN_SIZE * 4) + 1));                                                                                                                             \
    memset(poxhash.bytes, 0, SIZE_BYTE_ARR(BYTE_SIZE));                                                                                                                                          \
    memset(poxhash.words, 0, SIZE_WORD_ARR(POX_PORTION_NUM));                                                                                                                                    \
    memset(poxhash.doubles, 0, sizeof(uint32_t) * 2);                                                                                                                                            \
    poxhash.quad = 0;                                                                                                                                                                            \
    FACTORS_TO_BASEDIGEST(factor_array, poxhash.sexdigest, poxhash.vigdigest, poxhash.hexdigest, poxhash.tetdigest, poxhash.duodigest, poxhash.octdigest, poxhash.sendigest, poxhash.bindigest); \
    FACTORS_TO_BYTEARR(factor_array, poxhash.bytes);                                                                                                                                             \
    FACTORS_TO_DUOBLEARR(factor_array, poxhash.doubles);                                                                                                                                         \
    FACTPRS_TO_QUAD(factor_array, poxhash.quad);                                                                                                                                                 \
    memcpy(poxhash.words, factor_array, SIZE_WORD_ARR(POX_PORTION_NUM));

/**
 * Converts the given message into a struct PoxDigest (alias poxhash_t) object
 * Parameters:
 *      uint8_t *message
 *
 * Returns:
 *      struct PoxDigest (poxhash_t)
 *          PoxDigest.hexdigest: char[13] (null-terminated)
 *          PoxDigest.vigdigest: char[17] (null-terminated)
 *          PoxDigest.hexdigest: char[17] (null-terminated)
 *          PoxDigest.tetodigest: char[21] (null-terminated)
 *          PoxDigest.duodigest: char[21] (null-terminated)
 *          PoxDigest.octdigest: char[25] (null-terminated)
 *          PoxDigest.sendigest: char[29] (null-terminated)
 *          PoxDigest.bindigest: char[65] (null-terminated)
 *          PoxDigest.bytes: uint8_t[8]
 *          PoxDigest.words: uint16_t[4]
 *          PoxDigest.doubles uint32_t[2]
 *          PoxDigest.quad  uint64_t
 */
extern inline poxhash_t pox_hash(uint8_t *message)
{
    size_t length_message = strlen(message);

    uint8_t block_array[POX_BLOCK_NUM] = {0};
    uint8_t portion_array[POX_PORTION_NUM] = {0};
    uint16_t factor_array[POX_PORTION_NUM] = {
        cPOX_PRIME_INIT_A, cPOX_PRIME_INIT_B, cPOX_PRIME_INIT_C, cPOX_PRIME_INIT_D};

    size_t lengh_old = length_message;
    PAD_SIZE(length_message);
    uint8_t message_padded[length_message];
    memset(message_padded, 0, SIZE_BYTE_ARR(length_message));
    memcpy(message_padded, message, SIZE_BYTE_ARR(lengh_old));

    for (int i = 0; i < length_message; i += POX_BLOCK_NUM)
    {
        POX_PROCESS_BLOCK(factor_array, message_padded, block_array, portion_array, i, i + POX_BLOCK_NUM);
    }

    poxhash_t result;
    INIT_POXHASH(result, factor_array);

    return result;
}

#endif
