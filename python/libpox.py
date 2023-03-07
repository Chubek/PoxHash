##############################################
##      PoxHash v1                          ##
##      Implementation in Python			##
##      By Chubak Bidpaa - March 2023       ##
##      License: GPLv3                      ##
##############################################

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
__POX_PORTION_NUM = 16
__POX_ROUND_NUM = 8
__POX_FACT_NUM = 4

__WORD_WIDTH = 16
__BYTE_WIDTH = 8
__UINT16_MAX = 2**16 - 1
__NUM_SD_PRIME = 3
__NUM_8B_PRIME = 54

__ONE_UPPER16 = 0xffff0000
__ONE_LOWER16 = 0x0000ffff
__MASK_FZFZ = 0xf0f0
__MASK_ZFZF = 0x0f0f
__MASK_FZZZ = 0xf000
__MASK_ZZFZ = 0x00f0
__MASK_ZZZF = 0x000f
__MASK_ZZFF = 0x00ff
__MASK_FFZZ = 0xff00
__MASK_FZZF = 0xf00f
__MASK_FFFZ = 0xfff0
__MASK_ZFFF = 0x0fff
__MASK_01 = 0b01
__MASK_10 = 0b10
__MASK_11 = 0b11
__MASK_00 = 0b00

__COMB_BIONOM = [(0, 1), (0, 2), (0, 3), (1, 2), (1, 3), (2, 3)]
__RANGE_ZTF = [0, 1, 2, 3]


def __rotate_left(num: int, by: int) -> __array:
    res_array = __array('I', [num])
    res_array[0] = (res_array[0] << by) | (res_array[0] >> (__WORD_WIDTH - by))

    if res_array[0] > __UINT16_MAX:
        res_array[0] = (res_array[0] & __ONE_UPPER16) >> __WORD_WIDTH

    res_uint16 = __array('H', res_array.tolist())

    return res_uint16


def __add_with_overflow(arr_a: __array, arr_b: __array, ind: int):
    uint16_a = arr_a.tolist()[ind]
    uint16_b = arr_b.tolist()[ind]

    a_plus_b = uint16_a + uint16_b
    if a_plus_b > __UINT16_MAX:
        a_plus_b &= __ONE_LOWER16

    arr_a[ind] = a_plus_b


def __weighted_avg(ls: list[int], weights: list[int]) -> int:
    weighted_avg = 0
    for i, w in zip(ls, weights):
        weighted_avg += i * w

    weighted_avg //= __POX_FACT_NUM
    if weighted_avg > __UINT16_MAX:
        weighted_avg = (weighted_avg & __ONE_UPPER16) >> __WORD_WIDTH

    return weighted_avg


def __weighted_med(ls: list[int], weights: list[int]) -> int:
    weighted_med = 0
    for i, w in zip(ls, weights):
        weighted_med += i * w

    weighted_med = (weighted_med + 1) // 2
    if weighted_med > __UINT16_MAX:
        weighted_med &= __ONE_LOWER16

    return weighted_med


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
    remainder = num % __NUM_8B_PRIME
    prime = __POX_8B_PRIMES[remainder]
    return __array('H', [prime])


def __log_2_n(num: int) -> int:
    return 1 + __log_2_n(num // 2) if (num > 1) else 0


def __word_to_byte(word: int) -> tuple[int, int]:
    lower = word & __MASK_ZZFF
    upper = (word & __MASK_FFZZ) >> __BYTE_WIDTH

    return (lower, upper)


def __pox_factors_to_hex_digest(factor_array: __array) -> str:
    hex_str_a = f"{factor_array[0]:04x}"
    hex_str_b = f"{factor_array[1]:04x}"
    hex_str_c = f"{factor_array[2]:04x}"
    hex_str_d = f"{factor_array[3]:04x}"

    return f"{hex_str_a}{hex_str_b}{hex_str_c}{hex_str_d}".upper()


def __pox_factors_to_byte_array(factor_array: __array) -> __array:
    ret = []
    for word in factor_array:
        ret.extend(__word_to_byte(word))

    return __array('B', ret)


def __pox_alpha(temp_array: __array) -> None:
    aleph = (temp_array[0] ^ temp_array[1]) & __MASK_ZZFF
    theh = (temp_array[2] ^ temp_array[3]) & __MASK_FFZZ
    daal = (aleph | theh) % __POX_8B_PRIMES[0]
    gaaf = (aleph ^ theh) % __POX_8B_PRIMES[1]

    temp_array[0] >>= daal
    temp_array[1] >>= ((daal + gaaf) % 2) + 1
    temp_array[2] >>= gaaf


def __pox_delta(temp_array: __array) -> None:
    alaf = (temp_array[0] ^ __MASK_FFFZ) % __get_8b_prime(temp_array[0])[0]
    dalat = (temp_array[1] ^ __MASK_FZZF) % __get_8b_prime(temp_array[1])[0]
    tit = (temp_array[2] & __MASK_ZFFF) % __get_8b_prime(temp_array[2])[0]
    gaman = (temp_array[3] & __MASK_FFZZ) % __get_8b_prime(temp_array[3])[0]

    for _ in range(__POX_FACT_NUM):
        alaf >>= __POX_SINGLE_DIGIT_PRIMES[dalat % __NUM_SD_PRIME]
        dalat = __rotate_left(dalat, 2)[0]
        tit >>= __POX_SINGLE_DIGIT_PRIMES[gaman % __NUM_SD_PRIME]
        gaman ^= (alaf ^ __MASK_ZZFF
                  ) >> __POX_SINGLE_DIGIT_PRIMES[tit % __NUM_SD_PRIME]

    temp_array[1] ^= temp_array[2] % __POX_MAGIC_PRIMES[1]
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

    temp_array[0] ^= ((weighted_avg >> gimmel) ^ __MASK_ZZFF) & __MASK_ZZZF
    temp_array[3] ^= ((weighted_med << alef) ^ __MASK_FZFZ) & __MASK_FZZZ


def __pox_gamma(temp_array: __array) -> None:
    min_and_argmin = __min_and_argmin(temp_array)
    max_and_argmax = __max_and_argmax(temp_array)

    mmin = min_and_argmin[0]
    mmax = max_and_argmax[0]
    argmin = min_and_argmin[1]
    argmax = max_and_argmax[1]
    ay, dee, thorn, gee = argmin & __MASK_01, argmax ^ __MASK_10, argmin & __MASK_11, argmax ^ __MASK_00

    alaph = temp_array[ay] % __get_8b_prime(temp_array[thorn])[0]
    dalath = (__get_8b_prime(mmax)[0] ^ __MASK_ZFZF) % __get_8b_prime(mmin)[0]
    teth = mmax % __get_8b_prime(mmax)[0]
    gamal = temp_array[dee] % __get_8b_prime((mmin + mmax) // 2)[0]

    temp_array[ay] >>= (alaph ^ __MASK_ZZFZ) % __WORD_WIDTH
    temp_array[dee] >>= (gamal ^ __MASK_FZZZ) % ((mmax % 2) + 1)
    temp_array[thorn] ^= __log_2_n(dalath) & __MASK_ZFFF
    temp_array[gee] ^= __log_2_n(teth) >> ((gamal % 2) + 1)


def __pox_round_apply_alphabet(temp_array: __array) -> None:
    print(temp_array[0],temp_array[1], temp_array[2], temp_array[3] )
    __pox_alpha(temp_array)
    print(temp_array[0],temp_array[1], temp_array[2], temp_array[3] )

    __pox_delta(temp_array)
    print(temp_array[0],temp_array[1], temp_array[2], temp_array[3] )

    __pox_theta(temp_array)
    print(temp_array[0],temp_array[1], temp_array[2], temp_array[3] )

    __pox_gamma(temp_array)
    print(temp_array[0],temp_array[1], temp_array[2], temp_array[3] )


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


def __pox_apply_bytes(factor_array: __array, subportion: __array) -> None:
    avg_subportion = sum(subportion) // __POX_FACT_NUM
    med_subportion = (sum(subportion) + 1) // 2
    avg_odd_factor = __UINT16_MAX * (avg_subportion % 2)
    med_odd_factor = __UINT16_MAX * (med_subportion % 2)

    factor_array[0] ^= (subportion[0] + avg_subportion) ^ med_odd_factor
    factor_array[1] ^= (subportion[1] + med_subportion) ^ avg_odd_factor
    factor_array[2] ^= (subportion[2] + avg_subportion) ^ med_odd_factor
    factor_array[3] ^= (subportion[3] + med_subportion) ^ avg_odd_factor


def __pox_process_block(factor_array: __array, block: list[int]) -> None:
    portions = [
        block[i:i + __POX_PORTION_NUM]
        for i in range(0, __POX_BLOCK_NUM, __POX_PORTION_NUM)
    ]

    for portion in portions:
        subportions = [
            portion[j:j + __POX_FACT_NUM]
            for j in range(0, __POX_PORTION_NUM, __POX_FACT_NUM)
        ]

        for k, subportion in enumerate(subportions):
            for i in range(__POX_ROUND_NUM):
                __pox_apply_bytes(factor_array, subportion)
                __pox_round(factor_array)


class PoxHashTy:
    import array as array
    
    hexdigest: str
    bytes: array
    factors: array

    def __init__(self, hexdgest: str, bytes: array,
                 factors: array) -> None:
        self.hexdigest = hexdgest
        self.bytes = bytes
        self.factors = factors


def pox_hash(to_hash: bytearray) -> any:
    integer_list = __byte_to_array(to_hash)
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
    bytes = __pox_factors_to_byte_array(factor_array)

    return PoxHashTy(hexdgest=hexdigest, bytes=bytes, factors=factor_array)