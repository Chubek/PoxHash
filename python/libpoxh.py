##########################################
##      PoxHash v1                      ##
##      Implementation in Python        ##
##      By Chubak Bidpaa - March 2023   ##
##      License: GPLv3                  ##
##########################################

from array import array as __array

__POX_PRIMES = __array('H', [
    0xe537, 0xbd71, 0x9ef9, 0xbbcf, 0xf8dd, 0xceb7, 0xbaa1, 0x8f9f, 0xb0ed,
    0xfc4f, 0x9787, 0xf01f, 0xe1d1, 0xbcb9, 0xd565, 0xc011, 0xc1e1, 0xb58d,
    0xd4e1, 0x9ea1, 0xee49, 0x97cd, 0xdac9, 0xe257, 0xa32b, 0xafbb, 0xa5e3,
    0xfc43, 0xbf71, 0xe401, 0x8ebd, 0xd549
])

__POX_PRIME_A = 0x9f91
__POX_PRIME_B = 0xdb3b
__POX_PRIME_C = 0xc091
__POX_PRIME_D = 0xac8b

__POX_8B_PRIMES = __array('H', [
    0x2, 0x3, 0x5, 0x7, 0xb, 0xd, 0x11, 0x13, 0x17, 0x1d, 0x1f, 0x25, 0x29,
    0x2b, 0x2f, 0x35, 0x3b, 0x3d, 0x43, 0x47, 0x49, 0x4f, 0x53, 0x59, 0x61,
    0x65, 0x67, 0x6b, 0x6d, 0x71, 0x7f, 0x83, 0x89, 0x8b, 0x95, 0x97, 0x9d,
    0xa3, 0xa7, 0xad, 0xb3, 0xb5, 0xbf, 0xc1, 0xc5, 0xc7, 0xd3, 0xdf, 0xe3,
    0xe5, 0xe9, 0xef, 0xf1, 0xfb
])
__POX_MAGIC_PRIMES = __array('H', [0x33, 0x65])
__POX_SINGLE_DIGIT_PRIMES = __array('H', [0x3, 0x5, 0x7])

__POX_BLOCK_NUM = 64
__POX_8B_PRIME_NUM = 54
__POX_CHUNK_NUM = 16
__POX_ROUND_NUM = 8
__POX_PORTION_NUM = 4
__POX_SD_PRIME_NUM = 3
__POX_MAGIC_PRIME_NUM = 2

__WORD_WIDTH = 16
__BYTE_WIDTH = 8
__UINT16_MAX = 2**16 - 1

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

__COMB_BIONOM = [(0, 1), (0, 2), (0, 3), (1, 2), (1, 3), (2, 3)]
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

__OCT_CHARS = ['0', '1', '2', '3', '4', '5', '6', '7']
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

__HEX_SIZE = 4
__DUO_SIZE = 5
__OCT_SIZE = 6
__BIN_SIZE = 16

__HEX_BASE = 16
__DUO_BASE = 12
__OCT_BASE = 8
__BIN_BASE = 2


def __omega(res_array: __array) -> None:
    res_array[0] = (res_array[0] & __MASK_DWORD_4F4Z) >> __WORD_WIDTH


def __epsilon(res_array: __array) -> None:
    res_array[0] &= __MASK_DWORD_4Z4F


def __lamed(res_array: __array, by: int) -> None:
    res_array[0] = (res_array[0] << by) | (res_array[0] >> (__WORD_WIDTH - by))


def __rotate_left(num: int, by: int) -> __array:
    res_array = __array('I', [num])
    __lamed(res_array, by)

    if res_array[0] > __UINT16_MAX:
        __omega(res_array)

    res_uint16 = __array('H', res_array.tolist())

    return res_uint16


def __add_with_overflow(arr_a: __array, arr_b: __array, ind: int):
    a_plus_b = __array('I', [0])

    a_plus_b[0] = arr_a[ind] + arr_b[ind]
    if a_plus_b[0] > __UINT16_MAX:
        __epsilon(a_plus_b)

    arr_a[ind] = a_plus_b[0]


def __weighted_avg(ls: list[int], weights: list[int]) -> int:
    weighted_avg = __array('I', [0])
    for i, w in zip(ls, weights):
        weighted_avg[0] += i * w

    weighted_avg[0] //= __POX_PORTION_NUM
    if weighted_avg[0] > __UINT16_MAX:
        __omega(weighted_avg)

    return weighted_avg.tolist()[0]


def __weighted_med(ls: list[int], weights: list[int]) -> int:
    weighted_med = __array('I', [0])
    for i, w in zip(ls, weights):
        weighted_med[0] += i * w

    weighted_med[0] = (weighted_med[0] + 1) // 2
    if weighted_med[0] > __UINT16_MAX:
        __epsilon(weighted_med)

    return weighted_med.tolist()[0]


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


def __pad_array_with_zero(arr: __array) -> __array:
    while len(arr) % __POX_BLOCK_NUM != 0:
        arr.append(0)

    return arr


def __get_8b_prime(num: int) -> __array:
    remainder = num % __POX_8B_PRIME_NUM
    prime = __POX_8B_PRIMES[remainder]
    return __array('H', [prime])


def __log_2_n(num: int) -> int:
    return 1 + __log_2_n(num // 2) if (num > 1) else 0


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

def __dec_to_base(
        size: int, base: int, chars: list[str], 
        res: list[str], dec: int, offset: int) -> None:
    for i in reversed(range(offset * size, size + (offset * size))):
        res[i] = chars[dec % base]
        dec //= base

def __pox_factors_to_bin_digest(factor_array: __array) -> str:
    bin = ['0'] * (__BIN_SIZE * __POX_PORTION_NUM)
    for i, factor in enumerate(factor_array):
        __dec_to_base(__BIN_SIZE, __BIN_BASE, __BIN_CHARS, bin, factor, i)
    return ''.join(bin)


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


def __pox_factors_to_hex_digest(factor_array: __array) -> str:
    hex = ['0'] * (__HEX_SIZE * __POX_PORTION_NUM)
    for i, factor in enumerate(factor_array):
        __dec_to_base(__HEX_SIZE, __HEX_BASE, __HEX_CHARS, hex, factor, i)
    return ''.join(hex)


def __pox_factors_to_byte_array(factor_array: __array) -> __array:
    ret = []
    for word in factor_array:
        ret.extend(__word_to_byte(word))
    return __array('B', ret)


def __pox_alpha(temp_array: __array) -> None:
    aleph = (temp_array[0] ^ temp_array[1]) & __MASK_WORD_ZZFF
    theh = (temp_array[2] ^ temp_array[3]) & __MASK_WORD_FFZZ
    daal = (aleph | theh) % __POX_8B_PRIMES[0]
    gaaf = (aleph ^ theh) % __POX_8B_PRIMES[1]

    temp_array[0] >>= daal
    temp_array[1] >>= ((daal + gaaf) % 2) + 1
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
        dalat = __rotate_left(dalat, 2)[0]
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

    weighted_avg = __weighted_avg(temp_array.tolist(),
                                  [alef, dalet, tet, gimmel])

    weighted_med = __weighted_med(temp_array.tolist(),
                                  [alef, dalet, tet, gimmel])

    temp_array[0] ^= (
        (weighted_avg >> gimmel) ^ __MASK_WORD_ZZFF) & __MASK_WORD_ZZZF
    temp_array[3] ^= (
        (weighted_med << alef) ^ __MASK_WORD_FZFZ) & __MASK_WORD_FZZZ


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


def __pox_round_apply_alphabet(temp_array: __array) -> None:
    __pox_alpha(temp_array)
    __pox_delta(temp_array)
    __pox_theta(temp_array)
    __pox_gamma(temp_array)


def __pox_round_apply_prime(temp_array: __array) -> None:
    for pnum in __POX_PRIMES:
        temp_array[0] ^= pnum
        temp_array[1] &= pnum
        temp_array[2] ^= pnum
        temp_array[3] &= pnum


def __pox_round_add_tmp_to_facts(factor_array: __array,
                                 temp_array: __array) -> None:
    __add_with_overflow(factor_array, temp_array, 0)
    __add_with_overflow(factor_array, temp_array, 1)
    __add_with_overflow(factor_array, temp_array, 2)
    __add_with_overflow(factor_array, temp_array, 3)


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


def __pox_apply_bytes(factor_array: __array, subportion: __array,
                      index: int) -> None:
    avg_subportion = sum(subportion) // __POX_PORTION_NUM
    med_subportion = (sum(subportion) + 1) // 2
    avg_odd_factor = __UINT16_MAX * (avg_subportion % 2)
    med_odd_factor = __UINT16_MAX * (med_subportion % 2)

    ng = (subportion[0] + index) % __POX_PORTION_NUM
    chu = (subportion[1] + index) % __POX_PORTION_NUM
    yo = (subportion[2] + index) % __POX_PORTION_NUM
    eo = (subportion[3] + index) % __POX_PORTION_NUM

    factor_array[ng] ^= (subportion[eo] + avg_subportion) ^ med_odd_factor
    factor_array[chu] ^= (subportion[yo] + med_subportion) ^ avg_odd_factor
    factor_array[yo] ^= (subportion[chu] + avg_subportion) ^ med_odd_factor
    factor_array[eo] ^= (subportion[ng] + med_subportion) ^ avg_odd_factor


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


class PoxHashTy:
    import array as array

    hexdigest: str
    duodigest: str
    octdigest: str
    bindigest: str
    bytes: array
    words: array
    doubles: array
    quad: array

    def __init__(self, hexdgest: str, duodigest: str, octdigest: str,
                 bindigest: str, bytes: array, words: array, doubles: array,
                 quad: array) -> None:
        self.hexdigest = hexdgest
        self.duodigest = duodigest
        self.octdigest = octdigest
        self.bindigest = bindigest
        self.bytes = bytes
        self.words = words
        self.doubles = doubles
        self.quad = quad


def pox_hash(data: bytearray) -> PoxHashTy:
    """
    Converts the given argument to a PoxHashTy object

    Parametes:
        data: bytearray
    
    Returns:
        PoxHashTy
            PoxHashTy.hexdigest: string
            PoxHashTy.bytes: array.array('B', 8)
            PoxHashTy.words: array.array('H', 4)
            PoxHashTy.doubles: array.array('I', 2)
            PoxHashTy.quad: array.array('Q', 1)    
    """

    integer_list = __byte_to_array(data)
    integer_list = __pad_array_with_zero(integer_list)
    blocks = [
        integer_list[i:i + __POX_BLOCK_NUM]
        for i in range(0, len(integer_list), __POX_BLOCK_NUM)
    ]

    factor_array = __array(
        'H', [__POX_PRIME_A, __POX_PRIME_B, __POX_PRIME_C, __POX_PRIME_D])

    for block in blocks:
        __pox_process_block(factor_array, block)

    hexdigest = __pox_factors_to_hex_digest(factor_array)
    duodigest = __pox_factors_to_duo_digest(factor_array)
    octdigest = __pox_factors_to_oct_digest(factor_array)
    bindigest = __pox_factors_to_bin_digest(factor_array)
    bytes = __pox_factors_to_byte_array(factor_array)
    doubles = __pox_factors_to_doubles(factor_array)
    quad = __pox_factor_doubles_to_quad(doubles)

    return PoxHashTy(hexdgest=hexdigest,
                     duodigest=duodigest,
                     octdigest=octdigest,
                     bindigest=bindigest,
                     bytes=bytes,
                     words=factor_array,
                     doubles=doubles,
                     quad=quad)
