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

#define POX_BLOCK_NUM 64
#define POX_8B_PRIME_NUM 54
#define POX_PRIME_NUM 32
#define POX_CHUNK_NUM 16
#define POX_ROUND_NUM 8
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
#define HEX_SIZE 4
#define OCT_SIZE 6
#define DUO_SIZE 5
#define BIN_SIZE 16
#define SEX_BASE 60
#define HEX_BASE 16
#define OCT_BASE 8
#define DUO_BASE 12
#define BIN_BASE 2
#define BYTE_SIZE POX_BLOCK_NUM / 8
#define SIZE_BIONOM 6
#define SIZE_WORD_ARR(num) sizeof(uint16_t) * num
#define SIZE_WORD sizeof(uint16_t)
#define SIZE_BYTE_ARR(num) sizeof(char) * num
#define SIZE_BYTE sizeof(char)

static const uint16_t cPOX_PRIMES[POX_PRIME_NUM] = {
    0xe537, 0xbd71, 0x9ef9, 0xbbcf, 0xf8dd, 0xceb7, 0xbaa1, 0x8f9f, 0xb0ed,
    0xfc4f, 0x9787, 0xf01f, 0xe1d1, 0xbcb9, 0xd565, 0xc011, 0xc1e1, 0xb58d,
    0xd4e1, 0x9ea1, 0xee49, 0x97cd, 0xdac9, 0xe257, 0xa32b, 0xafbb, 0xa5e3,
    0xfc43, 0xbf71, 0xe401, 0x8ebd, 0xd549};
static const uint16_t cPOX_8B_PRIMES[POX_8B_PRIME_NUM] = {
    0x2, 0x3, 0x5, 0x7, 0xb, 0xd, 0x11, 0x13, 0x17, 0x1d, 0x1f, 0x25, 0x29,
    0x2b, 0x2f, 0x35, 0x3b, 0x3d, 0x43, 0x47, 0x49, 0x4f, 0x53, 0x59, 0x61,
    0x65, 0x67, 0x6b, 0x6d, 0x71, 0x7f, 0x83, 0x89, 0x8b, 0x95, 0x97, 0x9d,
    0xa3, 0xa7, 0xad, 0xb3, 0xb5, 0xbf, 0xc1, 0xc5, 0xc7, 0xd3, 0xdf, 0xe3,
    0xe5, 0xe9, 0xef, 0xf1, 0xfb};
static const uint16_t cPOX_MAGIC_PRIMES[POX_MAGIC_PRIME_NUM] = {0x33, 0x65};
static const uint16_t cPOX_SINGLE_DIGIT_PRIMES[POX_SD_PRIME_NUM] = {0x3, 0x5, 0x7};
static const char cHEX_CHARS[16] = {
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
static const char cOCT_CHARS[8] = {
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7'};
static const char cDUO_CHARS[12] = {
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
static const char cSEX_CHARS[60] = {
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
static const char cBIN_CHARS[2] = {'0', '1'};
static const size_t cRANGE_ZTF[4] = {0, 1, 2, 3};
static const size_t cCOMB_BIONOM[6][2] = {
    {0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}};
static const uint16_t cPOX_PRIME_A = 0x9f91;
static const uint16_t cPOX_PRIME_B = 0xdb3b;
static const uint16_t cPOX_PRIME_C = 0xc091;
static const uint16_t cPOX_PRIME_D = 0xac8b;

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

#define FACTORS_TO_BASEDIGEST(warr, sexdigest, hexdigest, duodigest, octdigest, bindigest) \
    WORD_TO_BASE(warr, SEX_BASE, SEX_SIZE, cSEX_CHARS, sexdigest);                         \
    WORD_TO_BASE(warr, HEX_BASE, HEX_SIZE, cHEX_CHARS, hexdigest);                         \
    WORD_TO_BASE(warr, DUO_BASE, DUO_SIZE, cDUO_CHARS, duodigest);                         \
    WORD_TO_BASE(warr, OCT_BASE, OCT_SIZE, cOCT_CHARS, octdigest);                         \
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
        temp_array[0] ^= pnum;            \
        temp_array[1] &= pnum;            \
        temp_array[2] ^= pnum;            \
        temp_array[3] &= pnum;            \
    } while (0)

#define POX_ROUND_APPLY_PRIME(temp_array)               \
    for (int __iw = 0; __iw < POX_PRIME_NUM; __iw++)    \
    {                                                   \
        POX_APPLY_PRIME(temp_array, cPOX_PRIMES[__iw]); \
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

#define POX_APPLY_BYTES(factor_array, portion, ng, chu, yo, eo, index)  \
    uint16_t portion_unsigned[POX_PORTION_NUM] = {                      \
        (uint16_t)((uint8_t)portion[0]),                                \
        (uint16_t)((uint8_t)portion[1]),                                \
        (uint16_t)((uint8_t)portion[2]),                                \
        (uint16_t)((uint8_t)portion[3]),                                \
    };                                                                  \
    uint16_t tmt, dca, odd_factor_tmt, odd_factor_dca;                  \
    tmt = 0;                                                            \
    dca = 0;                                                            \
    TAMAAM(portion_unsigned, tmt);                                      \
    DECA(portion_unsigned, dca);                                        \
    odd_factor_tmt = UINT16_MAX * (tmt % 2);                            \
    odd_factor_dca = UINT16_MAX * (dca % 2);                            \
    ng = (portion_unsigned[0] + index) % POX_PORTION_NUM;               \
    chu = (portion_unsigned[1] + index) % POX_PORTION_NUM;              \
    yo = (portion_unsigned[2] + index) % POX_PORTION_NUM;               \
    eo = (portion_unsigned[3] + index) % POX_PORTION_NUM;               \
    factor_array[ng] ^= (portion_unsigned[eo] | tmt) ^ odd_factor_dca;  \
    factor_array[chu] ^= (portion_unsigned[yo] & dca) ^ odd_factor_tmt; \
    factor_array[yo] ^= (portion_unsigned[chu] ^ tmt) ^ odd_factor_dca; \
    factor_array[eo] ^= (portion_unsigned[ng] | dca) ^ odd_factor_tmt;

#define POX_ROUND_ACTION(factor_array, portion, ng, chu, yo, eo, index) \
    POX_APPLY_BYTES(factor_array, portion, ng, chu, yo, eo, index);     \
    POX_ROUND(factor_array);

#define POX_PROCESS_APPLY(factor_arry, block_array, portion_array, pstart, pend)  \
    do                                                                            \
    {                                                                             \
        uint16_t ng, chu, yo, eo;                                                 \
        COPY_WORDS_TO_SUBARRAY(block_array, portion_array, pstart, pend);         \
        for (uint16_t __ie = 0; __ie < POX_ROUND_NUM; __ie++)                     \
        {                                                                         \
            POX_ROUND_ACTION(factor_array, portion_array, ng, chu, yo, eo, __ie); \
        }                                                                         \
    } while (0)

#define POX_PROCESS_BLOCK(factor_array, data, block_array, portion_array, bstart, bend)                \
    COPY_WORDS_TO_SUBARRAY(data, block_array, bstart, bend);                                           \
    for (int __ip = 0; __ip < POX_BLOCK_NUM; __ip += POX_CHUNK_NUM)                                    \
    {                                                                                                  \
        for (int __jt = __ip; __jt < __ip + POX_CHUNK_NUM; __jt += POX_PORTION_NUM)                    \
        {                                                                                              \
            POX_PROCESS_APPLY(factor_array, block_array, portion_array, __jt, __jt + POX_PORTION_NUM); \
        }                                                                                              \
    }

typedef struct PoxHashTy
{
    char sexdigest[(SEX_SIZE * 4) + 1];
    char hexdigest[(HEX_SIZE * 4) + 1];
    char duodigest[(DUO_SIZE * 4) + 1];
    char octdigest[(OCT_SIZE * 4) + 1];
    char bindigest[(BIN_SIZE * 4) + 1];
    uint8_t bytes[BYTE_SIZE];
    uint16_t words[POX_PORTION_NUM];
    uint32_t doubles[POX_PORTION_NUM / 2];
    uint64_t quad;
} poxhash_t;

#define INIT_POXHASH(poxhash, factor_array)                                                                                             \
    memset(poxhash.sexdigest, 0, SIZE_BYTE_ARR((SEX_SIZE * 4) + 1));                                                                    \
    memset(poxhash.hexdigest, 0, SIZE_BYTE_ARR((HEX_SIZE * 4) + 1));                                                                    \
    memset(poxhash.duodigest, 0, SIZE_BYTE_ARR((DUO_SIZE * 4) + 1));                                                                    \
    memset(poxhash.octdigest, 0, SIZE_BYTE_ARR((OCT_SIZE * 4) + 1));                                                                    \
    memset(poxhash.bindigest, 0, SIZE_BYTE_ARR((BIN_SIZE * 4) + 1));                                                                    \
    memset(poxhash.bytes, 0, SIZE_BYTE_ARR(BYTE_SIZE));                                                                                 \
    memset(poxhash.words, 0, SIZE_WORD_ARR(POX_PORTION_NUM));                                                                           \
    memset(poxhash.doubles, 0, sizeof(uint32_t) * 2);                                                                                   \
    poxhash.quad = 0;                                                                                                                   \
    FACTORS_TO_BASEDIGEST(factor_array, poxhash.sexdigest, poxhash.hexdigest, poxhash.duodigest, poxhash.octdigest, poxhash.bindigest); \
    FACTORS_TO_BYTEARR(factor_array, poxhash.bytes);                                                                                    \
    FACTORS_TO_DUOBLEARR(factor_array, poxhash.doubles);                                                                                \
    FACTPRS_TO_QUAD(factor_array, poxhash.quad);                                                                                        \
    memcpy(poxhash.words, factor_array, SIZE_WORD_ARR(POX_PORTION_NUM));

/**
 * Converts the given byte array into a struct PoxHashTy (alias poxhash_t) object
 * Parameters:
 *      char *data
 *
 * Returns:
 *      struct PoxHashTy (poxhash_t)
 *          PoxHashTy.hexdigest: char[17] (null-terminated)
 *          PoxHashTy.duodigest: char[21] (null-terminated)
 *          PoxHashTy.octdigest: char[25] (null-terminated)
 *          PoxHashTy.bindigest: char[65] (null-terminated)
 *          PoxHashTy.bytes: uint8_t[8]
 *          PoxHashTy.words: uint16_t[4]
 *          PoxHashTy.doubles uint32_t[2]
 *          PoxHashTy.quad  uint64_t
 */
extern inline poxhash_t pox_hash(char *data)
{
    size_t length_data = strlen(data);

    char block_array[POX_BLOCK_NUM] = {0};
    char portion_array[POX_PORTION_NUM] = {0};
    uint16_t factor_array[POX_PORTION_NUM] = {
        cPOX_PRIME_A, cPOX_PRIME_B, cPOX_PRIME_C, cPOX_PRIME_D};

    size_t lengh_old = length_data;
    PAD_SIZE(length_data);
    char data_padded[length_data];
    memset(data_padded, 0, SIZE_BYTE_ARR(length_data));
    memcpy(data_padded, data, SIZE_BYTE_ARR(lengh_old));

    for (int i = 0; i < length_data; i += POX_BLOCK_NUM)
    {
        POX_PROCESS_BLOCK(factor_array, data_padded, block_array, portion_array, i, i + POX_BLOCK_NUM);
    }

    poxhash_t result;
    INIT_POXHASH(result, factor_array);

    return result;
}

#endif
