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
#define POX_PORTION_NUM 16
#define POX_ROUND_NUM 8
#define POX_FACT_NUM 4
#define POX_MAGIC_NUM 2

#define ONE_UPPER16 0xffff0000
#define ONE_LOWER16 0x0000ffff
#define MASK_FZFZ 0xf0f0
#define MASK_ZFZF 0x0f0f
#define MASK_FZZZ 0xf000
#define MASK_ZFZZ 0x0f00
#define MASK_ZZFZ 0x00f0
#define MASK_ZZZF 0x000f
#define MASK_ZZFF 0x00ff
#define MASK_FFZZ 0xff00
#define MASK_FZZF 0xf00f
#define MASK_ZFFZ 0x0ff0
#define MASK_FFFZ 0xfff0
#define MASK_ZFFF 0x0fff
#define MASK_ZFZF 0x0f0f

#define WORD_WIDTH 16
#define BYTE_WIDTH 8
#define HEX_SIZE WORD_WIDTH / 4
#define BYTE_SIZE POX_BLOCK_NUM / 8
#define SIZE_BIONOM 6
#define SIZE_SD_PRIME 3
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
static const uint16_t cPOX_MAGIC_PRIMES[POX_MAGIC_NUM] = {0x33, 0x65};
static const uint16_t cPOX_SINGLE_DIGIT_PRIMES[SIZE_SD_PRIME] = {0x3, 0x5, 0x7};
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

static inline void *dec2hex(uint16_t dec, char out[HEX_SIZE])
{
    for (int i = 0; i < HEX_SIZE; i++)
    {
        out[HEX_SIZE - i - 1] = cHEX_CHARS[dec % WORD_WIDTH];
        dec /= 16;
    }
}

static inline uint16_t *copy_to_temp(uint16_t *src)
{
    uint16_t *dst = (uint16_t *)calloc(POX_FACT_NUM, SIZE_WORD);
    memcpy(dst, src, SIZE_WORD_ARR(POX_FACT_NUM));
    return dst;
}

static inline uint16_t get_8b_prime(uint16_t num)
{
    uint16_t remainder = num % POX_8B_PRIME_NUM;
    return cPOX_8B_PRIMES[remainder];
}

static inline uint16_t sum_portion(char *arr)
{
    uint16_t sum = 0;
    for (int i = 0; i < POX_FACT_NUM; i++)
        sum += arr[i];
    return sum;
}

static inline uint16_t weighted_avg(uint16_t arr[POX_FACT_NUM], uint16_t weights[POX_FACT_NUM])
{
    uint16_t result = 0;
    for (int i = 0; i < POX_FACT_NUM; i++)
    {
        result += arr[i] * weights[i];
    }

    result /= POX_FACT_NUM;
    if (result > UINT16_MAX)
        result = (result & ONE_UPPER16) >> WORD_WIDTH;

    return (uint16_t)result;
}

static inline uint16_t weighted_med(uint16_t arr[POX_FACT_NUM], uint16_t weights[POX_FACT_NUM])
{
    uint32_t result = 0;
    for (int i = 0; i < POX_FACT_NUM; i++)
    {
        result += arr[i] * weights[i];
    }

    result = (result + 1) / 2;
    if (result > UINT16_MAX)
        result &= ONE_LOWER16;

    return (uint16_t)result;
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

#define BITWISE_ROTATE_LEFT(num, by)                \
    num = (num << by) | (num >> (WORD_WIDTH - by)); \
    if (num > UINT16_MAX)                           \
    {                                               \
        num = (num & ONE_UPPER16) >> WORD_WIDTH;    \
    }

#define ADD_WITH_OVERFLOW(a, b, ptr)             \
    do                                           \
    {                                            \
        uint32_t __a_ttb = (uint32_t)a;          \
        uint32_t __b_ttb = (uint32_t)b;          \
        uint32_t __a_plus_b = __a_ttb + __b_ttb; \
        if (__a_plus_b > UINT16_MAX)             \
            __a_plus_b &= ONE_LOWER16;           \
        *ptr = (uint16_t)__a_plus_b;             \
    } while (0)

#define WORD_TO_2BYTE(word, bytelow, bytehigh) \
    bytelow = word & MASK_ZZFF;                \
    bytehigh = (word & MASK_FFZZ) >> BYTE_WIDTH;

#define AVG_PORTION(arr, res)            \
    do                                   \
    {                                    \
        uint16_t sum = sum_portion(arr); \
        res = sum / POX_FACT_NUM;        \
    } while (0)

#define MED_PORTION(arr, res)                \
    do                                       \
    {                                        \
        uint16_t sum = sum_portion(arr) + 1; \
        res = sum / 2;                       \
    } while (0);

#define MIN_ARGMIN(arr, min, minindex)              \
    min = arr[0];                                   \
    minindex = 0;                                   \
    for (int __it = 1; __it < POX_FACT_NUM; __it++) \
    {                                               \
        if (arr[__it] < min)                        \
        {                                           \
            min = arr[__it];                        \
            minindex = __it;                        \
        }                                           \
    }

#define MAX_ARGMAX(arr, max, maxindex)              \
    max = arr[0];                                   \
    maxindex = 0;                                   \
    for (int __ir = 1; __ir < POX_FACT_NUM; __ir++) \
    {                                               \
        if (arr[__ir] > max)                        \
        {                                           \
            max = arr[__ir];                        \
            maxindex = __ir;                        \
        }                                           \
    }

#define FILTER_INDEX_ARR(argmin, argmax, aside, beside)             \
    uint16_t *__resultarr[2] = {&aside, &beside};                   \
    int __iadded = 0;                                               \
    for (int __k = 0; __k < POX_FACT_NUM; __k++)                    \
    {                                                               \
        if (cRANGE_ZTF[__k] != argmin && cRANGE_ZTF[__k] != argmax) \
            *__resultarr[__iadded++] = cRANGE_ZTF[__k];             \
    }

#define PAD_SIZE_TO_BLOCK_SIZE(size)  \
    while (size % POX_BLOCK_NUM != 0) \
        ++size;

#define FACTORS_TO_BYTEARR(warr, barr)        \
    WORD_TO_2BYTE(warr[0], barr[0], barr[1]); \
    WORD_TO_2BYTE(warr[1], barr[2], barr[3]); \
    WORD_TO_2BYTE(warr[2], barr[4], barr[5]); \
    WORD_TO_2BYTE(warr[3], barr[6], barr[7]);

#define FACTORS_TO_HEXDIGEST(warr, digest)                                            \
    char hex[HEX_SIZE];                                                               \
    for (int __io = 0; __io < POX_FACT_NUM; __io++)                                   \
    {                                                                                 \
        dec2hex(warr[__io], hex);                                                     \
        int __kl = 0;                                                                 \
        for (int __jo = HEX_SIZE * __io; __jo < (HEX_SIZE * __io) + HEX_SIZE; __jo++) \
        {                                                                             \
            digest[__jo] = hex[__kl++];                                               \
        }                                                                             \
    }

#define POX_ALPHA(temp_array)                                     \
    uint16_t aleph = (temp_array[0] ^ temp_array[1]) & MASK_ZZFF; \
    uint16_t theh = (temp_array[2] ^ temp_array[3]) & MASK_FFZZ;  \
    uint16_t daal = (aleph | theh) % cPOX_8B_PRIMES[0];           \
    uint16_t gaaf = (aleph ^ theh) % cPOX_8B_PRIMES[1];           \
    temp_array[0] >>= daal;                                       \
    temp_array[1] >>= ((daal + gaaf) % 2) + 1;                    \
    temp_array[2] >>= gaaf;

#define POX_DELTA(temp_array)                                                         \
    uint16_t alaf = (temp_array[0] ^ MASK_FFFZ) % get_8b_prime(temp_array[0]);        \
    uint16_t dalat = (temp_array[1] ^ MASK_FZZF) % get_8b_prime(temp_array[1]);       \
    uint16_t tit = (temp_array[2] & MASK_ZFFF) % get_8b_prime(temp_array[2]);         \
    uint16_t gaman = (temp_array[3] & MASK_FFZZ) % get_8b_prime(temp_array[3]);       \
    for (int ___qz = 0; ___qz < POX_FACT_NUM; ___qz++)                                \
    {                                                                                 \
        alaf >>= cPOX_SINGLE_DIGIT_PRIMES[dalat % SIZE_SD_PRIME];                     \
        BITWISE_ROTATE_LEFT(dalat, 2);                                                \
        tit >>= cPOX_SINGLE_DIGIT_PRIMES[gaman % SIZE_SD_PRIME];                      \
        gaman ^= (alaf ^ MASK_ZZFF) >> cPOX_SINGLE_DIGIT_PRIMES[tit % SIZE_SD_PRIME]; \
    }                                                                                 \
    temp_array[1] ^= temp_array[2] % cPOX_MAGIC_PRIMES[1];                            \
    temp_array[2] ^= alaf + tit;                                                      \
    temp_array[3] ^= tit + gaman;

#define POX_THETA(temp_array)                                    \
    uint16_t alef = temp_array[0] % 2;                           \
    uint16_t dalet = temp_array[1] % 2;                          \
    uint16_t tet = temp_array[2] % 2;                            \
    uint16_t gimmel = temp_array[3] % 2;                         \
    uint16_t wavg, wmed;                                         \
    uint16_t weights[POX_FACT_NUM] = {alef, dalet, tet, gimmel}; \
    wavg = weighted_avg(temp_array, weights);                    \
    wmed = weighted_med(temp_array, weights);                    \
    temp_array[0] ^= ((wavg >> gimmel) ^ MASK_ZZFF) & MASK_ZZZF; \
    temp_array[3] ^= ((wmed << alef) ^ MASK_FZFZ) & MASK_FZZZ;

#define POX_GAMMA(temp_array)                                                \
    uint16_t mmin, argmin, mmax, argmax, aside, beside;                      \
    MIN_ARGMIN(temp_array, mmin, argmin);                                    \
    MAX_ARGMAX(temp_array, mmax, argmax);                                    \
    FILTER_INDEX_ARR(argmin, argmax, aside, beside);                         \
    uint16_t alaph = temp_array[aside] % get_8b_prime(temp_array[aside]);    \
    uint16_t dalath = (get_8b_prime(mmax) ^ MASK_ZFZF) % get_8b_prime(mmin); \
    uint16_t teth = mmax % get_8b_prime(mmax);                               \
    uint16_t gamal = temp_array[beside] % get_8b_prime((mmin + mmax) / 2);   \
    temp_array[aside] >>= (alaph ^ MASK_ZZFZ) % WORD_WIDTH;                  \
    temp_array[argmin] >>= (gamal ^ MASK_FZZZ) % ((mmax % 2) + 1);           \
    temp_array[argmax] ^= log2n(dalath) & MASK_ZFFF;                         \
    temp_array[beside] ^= (uint16_t)log2n(teth) >> ((gamal % 2) + 1);

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

#define POX_ROUND_OP(temp_array) \
    POX_ALPHA_WRAP(temp_array);  \
    POX_DELTA_WRAP(temp_array);  \
    POX_THETA_WRAP(temp_array);  \
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

#define POX_ADD_TEMP_TO_FACT_INDEX(factor_array, temp_array, index)                      \
    do                                                                                   \
    {                                                                                    \
        ADD_WITH_OVERFLOW(factor_array[index], temp_array[index], &factor_array[index]); \
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

#define POX_ROUND(factor_array)                                                                               \
    uint16_t temp_array[POX_FACT_NUM] = {factor_array[0], factor_array[1], factor_array[2], factor_array[3]}; \
    POX_ROUND_OP(temp_array);                                                                                 \
    POX_ROUND_APPLY_PRIME(temp_array);                                                                        \
    POX_ROUND_APPLY_SHUFFLE(temp_array);                                                                      \
    POX_ROUND_ADD_TEMP_TO_FACTS(factor_array, temp_array);

#define POX_APPLY_BYTES(factor_array, portion)                             \
    uint16_t avg, median, odd_factor_avg, odd_factor_med;                  \
    AVG_PORTION(portion, avg);                                             \
    MED_PORTION(portion, median);                                          \
    odd_factor_avg = UINT16_MAX * (avg % 2);                               \
    odd_factor_med = UINT16_MAX * (median % 2);                            \
    factor_array[0] ^= (((uint16_t)portion[0]) + avg) ^ odd_factor_med;    \
    factor_array[1] ^= (((uint16_t)portion[1]) + median) ^ odd_factor_avg; \
    factor_array[2] ^= (((uint16_t)portion[2]) + avg) ^ odd_factor_med;    \
    factor_array[3] ^= (((uint16_t)portion[3]) + median) ^ odd_factor_avg;

#define POX_ROUND_ACTION(factor_array, portion) \
    POX_APPLY_BYTES(factor_array, portion);     \
    POX_ROUND(factor_array);

#define POX_PROCESS_APPLY(factor_arry, block_array, portion_array, pstart, pend) \
    do                                                                           \
    {                                                                            \
        COPY_WORDS_TO_SUBARRAY(block_array, portion_array, pstart, pend);        \
        for (int __ie = 0; __ie < POX_ROUND_NUM; __ie++)                         \
        {                                                                        \
            POX_ROUND_ACTION(factor_array, portion_array);                       \
        }                                                                        \
    } while (0)

#define POX_PROCESS_BLOCK(factor_array, data, block_array, portion_array, bstart, bend)             \
    COPY_WORDS_TO_SUBARRAY(data, block_array, bstart, bend);                                        \
    for (int __ip = 0; __ip < POX_BLOCK_NUM; __ip += POX_PORTION_NUM)                               \
    {                                                                                               \
        for (int __jt = __ip; __jt < __ip + POX_PORTION_NUM; __jt += POX_FACT_NUM)                  \
        {                                                                                           \
            POX_PROCESS_APPLY(factor_array, block_array, portion_array, __jt, __jt + POX_FACT_NUM); \
        }                                                                                           \
    }

typedef struct PoxHash
{
    char hexdigest[WORD_WIDTH + 1];
    uint8_t bytes[BYTE_SIZE];
    uint16_t factors[POX_FACT_NUM];
} poxhash_t;

#define INIT_POXHASH(poxhash, factor_array)                      \
    memset(poxhash.hexdigest, 0, SIZE_BYTE_ARR(WORD_WIDTH + 1)); \
    memset(poxhash.bytes, 0, SIZE_BYTE_ARR(BYTE_SIZE));          \
    memset(poxhash.factors, 0, SIZE_WORD_ARR(POX_FACT_NUM));     \
    FACTORS_TO_HEXDIGEST(factor_array, poxhash.hexdigest);       \
    FACTORS_TO_BYTEARR(factor_array, poxhash.bytes);             \
    memcpy(poxhash.factors, factor_array, SIZE_WORD_ARR(POX_FACT_NUM));

extern inline poxhash_t pox_hash(char *data)
{
    size_t length_data = strlen(data);

    char block_array[POX_BLOCK_NUM] = {0};
    char portion_array[POX_FACT_NUM] = {0};
    uint16_t factor_array[POX_FACT_NUM] = {
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
