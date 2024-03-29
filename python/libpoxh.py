#####################################################################################
##                       PoxHash implementation in Python                          ##
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

from array import array as __array

# CONSTANTS

## INITIAL PRIME NUMBERS
## https://github.com/Chubek/PoxHash/blob/master/SPEC.md#initial-prime-numbers
__POX_PRIME_INIT_A = 0x17cb
__POX_PRIME_INIT_B = 0x0371
__POX_PRIME_INIT_C = 0x2419
__POX_PRIME_INIT_D = 0xf223

## SIZE CONSTANTS
## https://github.com/Chubek/PoxHash/blob/master/SPEC.md#size-constants
__POX_ROUND_PRIME_NUM = 90
__POX_BLOCK_NUM = 64
__POX_8B_PRIME_NUM = 54
__POX_ROUND_NUM = 31
__POX_CHUNK_NUM = 16
__POX_PORTION_NUM = 4
__POX_MASKS_ARRAY_NUM = 4
__POX_SD_PRIME_NUM = 3
__POX_MAGIC_PRIME_NUM = 2

## BIT-RELATED CONSTANTS
## https://github.com/Chubek/PoxHash/blob/master/SPEC.md#bit-related-constants
__WORD_WIDTH = 16
__BYTE_WIDTH = 8
__UINT16_MAX = 65535

## MASKS
## https://github.com/Chubek/PoxHash/blob/master/SPEC.md#masks
__MASK_QWORD_14Z2F = 0x00000000000000ff
__MASK_DWORD_4F4Z = 0xffff0000
__MASK_DWORD_4Z4F = 0x0000ffff
__MASK_WORD_FZFZ = 0xf0f0
__MASK_WORD_ZFZF = 0x0f0f
__MASK_WORD_FZZZ = 0xf000
__MASK_WORD_ZZFZ = 0x00f0
__MASK_WORD_ZZZF = 0x000f
__MASK_WORD_ZZFF = 0x00ff
__MASK_WORD_FFZZ = 0xff00
__MASK_WORD_FZZF = 0xf00f
__MASK_WORD_FFFZ = 0xfff0
__MASK_WORD_ZFFF = 0x0fff
__MASK_NIBBLET_01 = 0b01
__MASK_NIBBLET_10 = 0b10
__MASK_NIBBLET_11 = 0b11
__MASK_NIBBLET_00 = 0b00
__MASKS_ARRAY = __array(
    'H',
    [__MASK_WORD_FFZZ, __MASK_WORD_ZFFF, __MASK_WORD_FFFZ, __MASK_WORD_ZZFF])

## PRIME_ARRAYS
## https://github.com/Chubek/PoxHash/blob/master/SPEC.md#prime-arrays
__POX_ROUND_PRIMES = __array('H', [
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
])

__POX_8B_PRIMES = __array('H', [
    0x2, 0x3, 0x5, 0x7, 0xb, 0xd, 0x11, 0x13, 0x17, 0x1d, 0x1f, 0x25, 0x29,
    0x2b, 0x2f, 0x35, 0x3b, 0x3d, 0x43, 0x47, 0x49, 0x4f, 0x53, 0x59, 0x61,
    0x65, 0x67, 0x6b, 0x6d, 0x71, 0x7f, 0x83, 0x89, 0x8b, 0x95, 0x97, 0x9d,
    0xa3, 0xa7, 0xad, 0xb3, 0xb5, 0xbf, 0xc1, 0xc5, 0xc7, 0xd3, 0xdf, 0xe3,
    0xe5, 0xe9, 0xef, 0xf1, 0xfb
])
__POX_SINGLE_DIGIT_PRIMES = __array('H', [0x3, 0x5, 0x7])
__POX_MAGIC_PRIMES = __array('H', [0x33, 0x65])


## MISC
## https://github.com/Chubek/PoxHash/blob/master/SPEC.md#misc
__COMB_BIONOM = [(0, 1), (0, 2), (0, 3), (1, 2), (1, 3), (2, 3)]

## CONVERSION CONSTANTS
__SEX_CHARS = [
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
]
__VIG_CHARS = [
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "@",
    "^",
    "&",
    "*",
    "$",
    "+",
    "!",
    ";",
    ":",
    "~",
]
__HEX_CHARS = [
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
]
__TET_CHARS = [
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "T",
    "E",
    "W",
    "R",
]
__OCT_CHARS = ['0', '1', '2', '3', '4', '5', '6', '7']
__SEN_CHARS = [
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
]
__DUO_CHARS = [
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
]
__BIN_CHARS = ['0', '1']

__SEX_SIZE = 3
__VIG_SIZE = 4
__HEX_SIZE = 4
__TET_SIZE = 5
__DUO_SIZE = 5
__OCT_SIZE = 6
__SEN_SIZE = 7
__BIN_SIZE = 16

__SEX_BASE = 60
__VIG_BASE = 20
__HEX_BASE = 16
__TET_BASE = 14
__DUO_BASE = 12
__OCT_BASE = 8
__SEN_BASE = 6
__BIN_BASE = 2

######## TOOLS ########


def __max_and_argmax(ls: __array) -> __array:
    curr_max = ls[0]
    curr_index = 0

    for i, intgr in enumerate(ls[1:]):
        if intgr > curr_max:
            curr_max = intgr
            curr_index = i + 1

    return __array('H', [curr_max, curr_index])


def __min_and_argmin(ls: __array) -> __array:
    curr_min = ls[0]
    curr_index = 0

    for i, intgr in enumerate(ls[1:]):
        if intgr < curr_min:
            curr_min = intgr
            curr_index = i + 1

    return __array('H', [curr_min, curr_index])


def __byte_to_array(barray: bytearray) -> __array:
    return __array('H', list(barray))


def __log_2_n(num: int) -> int:
    return 1 + __log_2_n(num // 2) if (num > 1) else 0


######## /TOOLS ########
#-------------------------#
######## CONVERSION ########
# https://github.com/Chubek/PoxHash/blob/master/SPEC.md#part-g-conversion--preparation-prep-methods


def __word_to_byte(word: int) -> tuple[int, int]:
    lower = word & __MASK_WORD_ZZFF
    upper = (word & __MASK_WORD_FFZZ) >> __BYTE_WIDTH

    return (lower, upper)


def __word_to_doubles(w1: int, w2: int, darr: __array, index: int) -> None:
    word_to_double = __array('I', [w1, w2])
    darr[index] |= word_to_double[0]
    darr[index] |= word_to_double[1] << 16


def __pox_factors_to_doubles(warr: __array) -> __array:
    double_arr = __array('I', [0, 0])
    __word_to_doubles(warr[0], warr[1], double_arr, 0)
    __word_to_doubles(warr[2], warr[3], double_arr, 1)

    return double_arr


def __pox_factor_doubles_to_quad(darr: __array) -> __array:
    double_quad_arr = __array('Q', darr)
    quad = __array('Q', [0])

    quad[0] |= double_quad_arr[0]
    quad[0] |= double_quad_arr[1] << 32

    return quad


def __dec_to_base(size: int, base: int, chars: list[str], res: list[str],
                  dec: int, offset: int) -> None:
    for i in reversed(range(offset * size, size + (offset * size))):
        res[i] = chars[dec % base]
        dec //= base


def __pox_factors_to_bin_digest(factor_array: __array) -> str:
    bin = ['0'] * (__BIN_SIZE * __POX_PORTION_NUM)
    for i, factor in enumerate(factor_array):
        __dec_to_base(__BIN_SIZE, __BIN_BASE, __BIN_CHARS, bin, factor, i)
    return ''.join(bin)


def __pox_factors_to_sen_digest(factor_array: __array) -> str:
    sen = ['0'] * (__SEN_SIZE * __POX_PORTION_NUM)
    for i, factor in enumerate(factor_array):
        __dec_to_base(__SEN_SIZE, __SEN_BASE, __SEN_CHARS, sen, factor, i)
    return ''.join(sen)


def __pox_factors_to_oct_digest(factor_array: __array) -> str:
    oct = ['0'] * (__OCT_SIZE * __POX_PORTION_NUM)
    for i, factor in enumerate(factor_array):
        __dec_to_base(__OCT_SIZE, __OCT_BASE, __OCT_CHARS, oct, factor, i)
    return ''.join(oct)


def __pox_factors_to_duo_digest(factor_array: __array) -> str:
    duo = ['0'] * (__DUO_SIZE * __POX_PORTION_NUM)
    for i, factor in enumerate(factor_array):
        __dec_to_base(__DUO_SIZE, __DUO_BASE, __DUO_CHARS, duo, factor, i)
    return ''.join(duo)


def __pox_factors_to_tet_digest(factor_array: __array) -> str:
    tet = ['0'] * (__TET_SIZE * __POX_PORTION_NUM)
    for i, factor in enumerate(factor_array):
        __dec_to_base(__TET_SIZE, __TET_BASE, __TET_CHARS, tet, factor, i)
    return ''.join(tet)


def __pox_factors_to_hex_digest(factor_array: __array) -> str:
    hex = ['0'] * (__HEX_SIZE * __POX_PORTION_NUM)
    for i, factor in enumerate(factor_array):
        __dec_to_base(__HEX_SIZE, __HEX_BASE, __HEX_CHARS, hex, factor, i)
    return ''.join(hex)


def __pox_factors_to_vig_digest(factor_array: __array) -> str:
    vig = ['0'] * (__VIG_SIZE * __POX_PORTION_NUM)
    for i, factor in enumerate(factor_array):
        __dec_to_base(__VIG_SIZE, __VIG_BASE, __VIG_CHARS, vig, factor, i)
    return ''.join(vig)


def __pox_factors_to_sex_digest(factor_array: __array) -> str:
    sex = ['0'] * (__SEX_SIZE * __POX_PORTION_NUM)
    for i, factor in enumerate(factor_array):
        __dec_to_base(__SEX_SIZE, __SEX_BASE, __SEX_CHARS, sex, factor, i)
    return ''.join(sex)


def __pox_factors_to_byte_array(factor_array: __array) -> __array:
    ret = []
    for word in factor_array:
        ret.extend(__word_to_byte(word))
    return __array('B', ret)


######## /CONVERSION ########
#---------------------------#
######## BITWISE OPS ########
# https://github.com/Chubek/PoxHash/blob/master/SPEC.md#part-b-bitwise-operations


def __octopad(arr: __array) -> __array:
    original_len = len(arr)
    n = original_len
    while len(arr) % __POX_BLOCK_NUM != 0:
        arr.append(arr[n % original_len] ^ (n & __MASK_QWORD_14Z2F))
        n += arr[n % original_len]
    return arr

def __omega(res_array: __array) -> None:
    res_array[0] = (res_array[0] & __MASK_DWORD_4F4Z) >> __WORD_WIDTH


def __epsilon(res_array: __array) -> None:
    res_array[0] &= __MASK_DWORD_4Z4F


def __lamed(res_array: __array, by: int) -> None:
    res_array[0] = (res_array[0] << by) | (res_array[0] >> (__WORD_WIDTH - by))


######## /BITWISE OPS ########
#----------------------------#
######## BESPOKE OPS ########
# https://github.com/Chubek/PoxHash/blob/master/SPEC.md#part-c-bespoke-operations


def __gorda(num: int, by: int) -> __array:
    res_array = __array('I', [num])
    __lamed(res_array, by)

    if res_array[0] > __UINT16_MAX:
        __omega(res_array)

    res_uint16 = __array('H', res_array.tolist())

    return res_uint16


def __tasu(arr_a: __array, arr_b: __array, ind: int):
    a_plus_b = __array('I', [0])

    a_plus_b[0] = arr_a[ind] + arr_b[ind]
    if a_plus_b[0] > __UINT16_MAX:
        __epsilon(a_plus_b)

    arr_a[ind] = a_plus_b[0]


def __centum(ls: list[int], weights: list[int]) -> int:
    centum = __array('I', [0])
    for i, w in zip(ls, weights):
        centum[0] += i * w

    centum[0] //= __POX_PORTION_NUM
    if centum[0] > __UINT16_MAX:
        __omega(centum)

    return centum[0]


def __satem(ls: list[int], weights: list[int]) -> int:
    satem = __array('I', [0])
    for i, w in zip(ls, weights):
        satem[0] += i * w

    satem[0] = (satem[0] + 1) // 2
    if satem[0] > __UINT16_MAX:
        __epsilon(satem)

    return satem[0]


def __tamaam(ls: list[int]) -> int:
    tamaam = __array('I', [0])
    for w in ls:
        tamaam[0] += w

    tamaam[0] //= __POX_PORTION_NUM
    if tamaam[0] > __UINT16_MAX:
        __omega(tamaam)

    return tamaam[0]


def __deca(ls: list[int]) -> int:
    deca = __array('I', [0])
    for w in ls:
        deca[0] += w

    deca[0] = (deca[0] + 1) // 2
    if deca[0] > __UINT16_MAX:
        __epsilon(deca)

    return deca[0]


def __get_8b_prime(num: int) -> __array:
    remainder = num % __POX_8B_PRIME_NUM
    prime = __POX_8B_PRIMES[remainder]
    return __array('H', [prime])


######## /BESPOKE OPS ########
#----------------------------#
######## ALPHABET OPS ########
# https://github.com/Chubek/PoxHash/blob/master/SPEC.md#part-d-alphabet-operations


def __pox_alpha(temp_array: __array) -> None:
    aleph = (temp_array[0] ^ temp_array[1]) & __MASK_WORD_ZZFF
    daal = (temp_array[2] ^ temp_array[3]) & __MASK_WORD_FFZZ
    theh = (aleph | daal) % __POX_8B_PRIMES[0]
    gaaf = (aleph ^ daal) % __POX_8B_PRIMES[1]

    temp_array[0] >>= theh
    temp_array[1] >>= ((theh + gaaf) % 2) + 1
    temp_array[2] >>= gaaf


def __pox_delta(temp_array: __array) -> None:
    alaf = (temp_array[0] ^ __MASK_WORD_FFFZ) % __get_8b_prime(
        temp_array[0])[0]
    dalat = (temp_array[1] ^ __MASK_WORD_FZZF) % __get_8b_prime(
        temp_array[1])[0]
    tit = (temp_array[2] & __MASK_WORD_ZFFF) % __get_8b_prime(temp_array[2])[0]
    gaman = (temp_array[3] & __MASK_WORD_FFZZ) % __get_8b_prime(
        temp_array[3])[0]

    for _ in range(__POX_PORTION_NUM):
        alaf >>= __POX_SINGLE_DIGIT_PRIMES[dalat % __POX_SD_PRIME_NUM]
        dalat = __gorda(dalat, 2)[0]
        tit >>= __POX_SINGLE_DIGIT_PRIMES[gaman % __POX_SD_PRIME_NUM]
        gaman ^= (alaf ^ __MASK_WORD_ZZFF
                  ) >> __POX_SINGLE_DIGIT_PRIMES[tit % __POX_SD_PRIME_NUM]

    temp_array[1] ^= temp_array[2] % __POX_MAGIC_PRIMES[alaf %
                                                        __POX_MAGIC_PRIME_NUM]
    temp_array[2] ^= alaf + tit
    temp_array[3] ^= tit + gaman


def __pox_theta(temp_array: __array) -> None:
    alef = temp_array[0] % 2
    dalet = temp_array[1] % 2
    tet = temp_array[2] % 2
    gimmel = temp_array[3] % 2

    ctm = __centum(temp_array.tolist(), [alef, dalet, tet, gimmel])

    stm = __satem(temp_array.tolist(), [alef, dalet, tet, gimmel])

    temp_array[0] ^= ((ctm >> gimmel) ^ __MASK_WORD_ZZFF) & __MASK_WORD_ZZZF
    temp_array[3] ^= ((stm << alef) ^ __MASK_WORD_FZFZ) & __MASK_WORD_FZZZ


def __pox_gamma(temp_array: __array) -> None:
    min_and_argmin = __min_and_argmin(temp_array)
    max_and_argmax = __max_and_argmax(temp_array)

    mmin = min_and_argmin[0]
    mmax = max_and_argmax[0]
    argmin = min_and_argmin[1]
    argmax = max_and_argmax[1]
    ay, dee, thorn, gee = argmin & __MASK_NIBBLET_01, argmax ^ __MASK_NIBBLET_10, argmin & __MASK_NIBBLET_11, argmax ^ __MASK_NIBBLET_00

    alaph = temp_array[ay] % __get_8b_prime(temp_array[thorn])[0]
    dalath = (__get_8b_prime(mmax)[0]
              ^ __MASK_WORD_ZFZF) % __get_8b_prime(mmin)[0]
    teth = mmax % __get_8b_prime(mmax)[0]
    gamal = temp_array[dee] % __get_8b_prime((mmin + mmax) // 2)[0]

    temp_array[ay] >>= (alaph ^ __MASK_WORD_ZZFZ) % __WORD_WIDTH
    temp_array[dee] >>= (gamal ^ __MASK_WORD_FZZZ) % ((mmax % 2) + 1)
    temp_array[thorn] ^= __log_2_n(dalath) & __MASK_WORD_ZFFF
    temp_array[gee] ^= __log_2_n(teth) >> ((gamal % 2) + 1)


######## /ALPHABET OPS ########
#-----------------------------#
########   ROUND OPS   ########
# https://github.com/Chubek/PoxHash/blob/master/SPEC.md#part-e-round-methods


def __pox_round_apply_alphabet(temp_array: __array) -> None:
    __pox_alpha(temp_array)
    __pox_delta(temp_array)
    __pox_theta(temp_array)
    __pox_gamma(temp_array)


def __pox_round_apply_bahman(temp_array: __array, pnum: int):
    cica = pnum % __POX_PORTION_NUM
    mica = (cica + 1) % __POX_PORTION_NUM
    nica = (mica + 2) % __POX_PORTION_NUM
    wica = (nica + 3) % __POX_PORTION_NUM

    mianju = temp_array[cica] % __POX_MASKS_ARRAY_NUM
    mianja = temp_array[mica] % __POX_MASKS_ARRAY_NUM

    sosu = temp_array[nica] % __POX_ROUND_PRIME_NUM
    sosa = temp_array[wica] % __POX_ROUND_PRIME_NUM

    temp_array[cica] ^= (temp_array[mica] << cica) & __MASKS_ARRAY[mianju]
    temp_array[wica] &= temp_array[wica] ^ __POX_ROUND_PRIMES[sosu]
    temp_array[nica] ^= (temp_array[cica] << (wica * 2)) & __MASKS_ARRAY[mianja]
    temp_array[mica] |= temp_array[nica] | __POX_ROUND_PRIMES[sosa]


def __pox_round_apply_prime(temp_array: __array) -> None:
    for i in range(__POX_ROUND_PRIME_NUM):
        curr_pnum = __POX_ROUND_PRIMES[i]
        temp_array[0] %= curr_pnum
        temp_array[1] %= curr_pnum
        temp_array[2] %= curr_pnum
        temp_array[3] %= curr_pnum
        __pox_round_apply_bahman(temp_array, curr_pnum)


def __pox_round_add_tmp_to_facts(factor_array: __array,
                                 temp_array: __array) -> None:
    __tasu(factor_array, temp_array, 0)
    __tasu(factor_array, temp_array, 1)
    __tasu(factor_array, temp_array, 2)
    __tasu(factor_array, temp_array, 3)


def __pox_round_apply_shuffle(temp_array: __array) -> None:

    def f_inner_pox_shuffle(temp_array: __array, indexof: int, indexwith: int):
        tmp = temp_array[indexof]
        temp_array[indexof] = temp_array[indexwith]
        temp_array[indexwith] = tmp

    for iof, iwith in __COMB_BIONOM:
        f_inner_pox_shuffle(temp_array, iof, iwith)


def __pox_round(factor_array: __array) -> None:
    temporary_array = __array('H', factor_array.tolist())
    __pox_round_apply_alphabet(temporary_array)
    __pox_round_apply_prime(temporary_array)
    __pox_round_apply_shuffle(temporary_array)
    __pox_round_add_tmp_to_facts(factor_array, temporary_array)


########   /ROUND OPS   ########
#------------------------------#
########   BLOCK OPS   #########
# https://github.com/Chubek/PoxHash/blob/master/SPEC.md#part-f-block-methods


def __pox_apply_bytes(factor_array: __array, subportion: __array,
                      index: int) -> None:
    tmt = __tamaam(subportion)
    dca = __deca(subportion)
    tmt_odd_factor = __UINT16_MAX ^ (tmt % (dca + 2))
    dca_odd_factor = __UINT16_MAX ^ (dca % (tmt + 3))
    ng = (subportion[0] + index) % __POX_PORTION_NUM
    chu = (subportion[1] + index) % __POX_PORTION_NUM
    yo = (subportion[2] + index) % __POX_PORTION_NUM
    eo = (subportion[3] + index) % __POX_PORTION_NUM

    zam = subportion[0] % __POX_8B_PRIMES[subportion[chu] % __POX_8B_PRIME_NUM]
    pez = subportion[1] % __POX_8B_PRIMES[subportion[yo] % __POX_8B_PRIME_NUM]
    dit = subportion[2] % __POX_8B_PRIMES[subportion[eo] % __POX_8B_PRIME_NUM]
    kit = subportion[3] % __POX_8B_PRIMES[subportion[ng] % __POX_8B_PRIME_NUM]

    factor_array[ng] ^= ((
        (subportion[eo] >> chu) | tmt) ^ dca_odd_factor) | zam
    factor_array[chu] ^= ((subportion[yo] & dca) ^ tmt_odd_factor) ^ pez
    factor_array[yo] ^= ((subportion[chu] ^ tmt) ^ dca_odd_factor) | dit
    factor_array[eo] ^= (((subportion[ng] >> yo) | dca) ^ tmt_odd_factor) ^ kit

    factor_array[0] >>= subportion[3] % (ng + 1)
    factor_array[1] >>= subportion[2] % (chu + 1)
    factor_array[2] ^= subportion[1] >> (dca % 2)
    factor_array[3] >>= subportion[0] % (eo + 1)


def __pox_process_block(factor_array: __array, block: list[int]) -> None:
    portions = [
        block[i:i + __POX_CHUNK_NUM]
        for i in range(0, __POX_BLOCK_NUM, __POX_CHUNK_NUM)
    ]

    for portion in portions:
        subportions = [
            portion[j:j + __POX_PORTION_NUM]
            for j in range(0, __POX_CHUNK_NUM, __POX_PORTION_NUM)
        ]

        for k, subportion in enumerate(subportions):
            for i in range(__POX_ROUND_NUM):
                __pox_apply_bytes(factor_array, subportion, i)
                __pox_round(factor_array)


########   /BLOCK OPS   #########


class PoxDigest:
    import array as array

    sexdigest: str
    vigdiest: str
    hexdigest: str
    tetdigest: str
    duodigest: str
    octdigest: str
    sendigest: str
    bindigest: str
    bytes: array
    words: array
    doubles: array
    quad: array

    def __init__(self, sexdigest: str, vigdigest: str, hexdigest: str,
                 tetdigest: str, duodigest: str, octdigest: str,
                 sendigest: str, bindigest: str, bytes: array, words: array,
                 doubles: array, quad: array) -> None:
        self.sexdigest = sexdigest
        self.vigdigest = vigdigest
        self.hexdigest = hexdigest
        self.tetdigest = tetdigest
        self.duodigest = duodigest
        self.octdigest = octdigest
        self.sendigest = sendigest
        self.bindigest = bindigest
        self.bytes = bytes
        self.words = words
        self.doubles = doubles
        self.quad = quad

    def __dict__(self) -> dict[str, any]:
        return {
            "Undecenary": {
                "Sexdigest": self.sexdigest,
                "Vigdigest": self.vigdigest,
                "Hexdigest": self.hexdigest,
                "Tetdigest": self.tetdigest,
                "Duodigest": self.duodigest,
                "Octdigest": self.octdigest,
                "Sendigest": self.sendigest,
                "Bindigest": self.bindigest,
            },
            "Decenary": {
                "Bytes": self.bytes.tolist(),
                "Words": self.words.tolist(),
                "Doubles": self.doubles.tolist(),
                "Quad": self.quad.tolist(),
            }
        }

    def __repr__(self) -> str:
        import json
        return json.dumps(self.__dict__(), indent=4)

    def __str__(self) -> str:
        return f"""
            - UNDECENARY:
            + Sexdigest: {self:G},  
            + Vigdigest: {self:V},
            + Hexdigest: {self:H},
            + Tetdigest: {self:T},
            + Duodigest: {self:D},
            + Octdigest: {self:O},
            + Sendigest: {self:S},
            + Bindigest: {self:B},

            - DECENARY:
            + Bytes:     {self:8},
            + Words:     {self:4},
            + Doubles:   {self:2},
            + Quad:      {self:1},
        """

    def __format__(self, spec: str) -> str:
        if spec == 'g' or spec == 'G' or spec == 'sex' or spec == 'SEX':
            return self.sexdigest
        elif spec == 'v' or spec == 'V' or spec == 'vig' or spec == 'VIG':
            return self.vigdigest
        elif spec == 'h' or spec == 'hex':
            return self.hexdigest
        elif spec == 'H' or spec == 'HEX':
            return self.hexdigest.upper()
        elif spec == 't' or spec == 'tet':
            return self.tetdigest
        elif spec == 'T' or spec == 'TET':
            return self.tetdigest.upper()
        elif spec == 'd' or spec == 'D' or spec == 'duo' or spec == 'DUO':
            return self.duodigest
        elif spec == 'o' or spec == 'O' or spec == 'oct' or spec == 'OCT':
            return self.octdigest
        elif spec == 's' or spec == 'S' or spec == 'sen' or spec == 'SEN':
            return self.sendigest
        elif spec == 'b' or spec == 'B' or spec == 'bin' or spec == 'BIN':
            return self.bindigest
        elif spec == '8' or spec == 'u8' or spec == 'Bytes':
            return f"U8[{self.bytes[0]}, {self.bytes[1]}, {self.bytes[2]}, {self.bytes[3]}, {self.bytes[4]}, {self.bytes[5]}, {self.bytes[6]}, {self.bytes[7]}]"
        elif spec == '4' or spec == 'u16' or spec == 'Words':
            return f"U16[{self.words[0]}, {self.words[1]}, {self.words[2]}, {self.words[3]}]"
        elif spec == '2' or spec == 'u32' or spec == 'Doubles':
            return f"U32[{self.doubles[0]}, {self.doubles[1]}]"
        elif spec == '1' or spec == 'u64' or spec == 'Quad':
            return f"U64[{self.quad[0]}]"
        else:
            return "EUF"


def pox_hash(message: any) -> PoxDigest:
    """
    Converts the given message to a PoxDigest object

    Parametes:
        message: array.array('B', message_data)
    
    Returns:
        PoxDigest
            PoxDigest.sexdigest: string
            PoxDigest.vigdigest: string
            PoxDigest.hexdigest: string
            PoxDigest.tetdigest: string
            PoxDigest.duodigest: string
            PoxDigest.octdigest: string
            PoxDigest.sendigest: string
            PoxDigest.bindigest: string
            PoxDigest.bytes: array.array('B', 8)
            PoxDigest.words: array.array('H', 4)
            PoxDigest.doubles: array.array('I', 2)
            PoxDigest.quad: array.array('Q', 1)    
    """

    ubyte_array = __octopad(message)
    if ubyte_array is None:
        return None

    blocks = [
        ubyte_array[i:i + __POX_BLOCK_NUM]
        for i in range(0, len(ubyte_array), __POX_BLOCK_NUM)
    ]

    factor_array = __array('H', [
        __POX_PRIME_INIT_A, __POX_PRIME_INIT_B, __POX_PRIME_INIT_C,
        __POX_PRIME_INIT_D
    ])

    for block in blocks:
        __pox_process_block(factor_array, block)

    sexdigest = __pox_factors_to_sex_digest(factor_array)
    vigdigest = __pox_factors_to_vig_digest(factor_array)
    hexdigest = __pox_factors_to_hex_digest(factor_array)
    tetdigest = __pox_factors_to_tet_digest(factor_array)
    duodigest = __pox_factors_to_duo_digest(factor_array)
    octdigest = __pox_factors_to_oct_digest(factor_array)
    sendigest = __pox_factors_to_sen_digest(factor_array)
    bindigest = __pox_factors_to_bin_digest(factor_array)
    bytes = __pox_factors_to_byte_array(factor_array)
    doubles = __pox_factors_to_doubles(factor_array)
    quad = __pox_factor_doubles_to_quad(doubles)

    return PoxDigest(sexdigest=sexdigest,
                     vigdigest=vigdigest,
                     hexdigest=hexdigest,
                     tetdigest=tetdigest,
                     duodigest=duodigest,
                     octdigest=octdigest,
                     sendigest=sendigest,
                     bindigest=bindigest,
                     bytes=bytes,
                     words=factor_array,
                     doubles=doubles,
                     quad=quad)