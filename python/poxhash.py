from array import array

cPOX_PRIMES = array('H', [
    0xe537, 0xbd71, 0x9ef9, 0xbbcf, 0xf8dd, 0xceb7, 0xbaa1, 0x8f9f, 0xb0ed,
    0xfc4f, 0x9787, 0xf01f, 0xe1d1, 0xbcb9, 0xd565, 0xc011, 0xc1e1, 0xb58d,
    0xd4e1, 0x9ea1, 0xee49, 0x97cd, 0xdac9, 0xe257, 0xa32b, 0xafbb, 0xa5e3,
    0xfc43, 0xbf71, 0xe401, 0x8ebd, 0xd549
])

cPOX_PRIME_A = 0x9f91
cPOX_PRIME_B = 0xdb3b
cPOX_PRIME_C = 0xc091
cPOX_PRIME_D = 0xac8b

cPOX_8B_PRIMES = array('H', [
    0x2, 0x3, 0x5, 0x7, 0xb, 0xd, 0x11, 0x13, 0x17, 0x1d, 0x1f, 0x25, 0x29,
    0x2b, 0x2f, 0x35, 0x3b, 0x3d, 0x43, 0x47, 0x49, 0x4f, 0x53, 0x59, 0x61,
    0x65, 0x67, 0x6b, 0x6d, 0x71, 0x7f, 0x83, 0x89, 0x8b, 0x95, 0x97, 0x9d,
    0xa3, 0xa7, 0xad, 0xb3, 0xb5, 0xbf, 0xc1, 0xc5, 0xc7, 0xd3, 0xdf, 0xe3,
    0xe5, 0xe9, 0xef, 0xf1, 0xfb
])
cPOX_MAGIC_PRIMES = array('H', [0x33, 0x65])

cPOX_BLOCK_NUM = 64
cPOX_PORTION_NUM = 16
cPOX_ROUND_NUM = 8
cPOX_FACT_NUM = 4

cMOD_ITER = array('H', [2, 4, 6, 8])

cBIT_ZERO = '0'
cBIT_ONE = '1'

cWORD_WIDTH = 16

cUINT16_MAX = 2**16 - 1
cUINT8_MAX = 2**8 - 1
cNUM_8B_PRIME = 54

cONE_UPPER16 = 0xffff0000
cONE_LOWER16 = 0x0000ffff
cMASK_FZFZ = 0xf0f0
cMASK_ZFZF = 0x0f0f
cMASK_FZZZ = 0xf000
cMASK_ZFZZ = 0x0f00
cMASK_ZZFZ = 0x00f0
cMASK_ZZZF = 0x000f
cMASK_ZZFF = 0x00ff
cMASK_FFZZ = 0xff00
cMASK_FZZF = 0xf00f
cMASK_ZFFZ = 0x0ff0
cMASK_FFFZ = 0xfff0
cMASK_ZFFF = 0x0fff

cCOMB_BIONOM = [(0, 1), (0, 2), (0, 3), (1, 2), (1, 3), (2, 3)]
cRANGE_ZTF = [0, 1, 2, 3]


def f_rotate_left(num: int, by: int) -> array:
    res_array = array('I', [num])
    res_array[0] = (res_array[0] << by) | (res_array[0] >> (cWORD_WIDTH - by))

    if res_array[0] > cUINT16_MAX:
        res_array[0] = (res_array[0] & cONE_UPPER16) >> cWORD_WIDTH

    res_uint16 = array('H', res_array.tolist())

    return res_uint16


def f_rotate_right(num: int, by: int) -> array:
    res_array = array('I', [num])
    res_array[0] = (res_array[0] >> by) | (res_array[0] << (cWORD_WIDTH - by))

    if res_array[0] > cUINT16_MAX:
        res_array[0] = (res_array[0] & cONE_UPPER16) >> cWORD_WIDTH

    res_uint16 = array('H', res_array.tolist())

    return res_uint16


def f_add_with_overflow(arr_a: array, arr_b: array, ind: int):
    uint16_a = arr_a.tolist()[ind]
    uint16_b = arr_b.tolist()[ind]

    a_plus_b = uint16_a + uint16_b
    if a_plus_b > cUINT16_MAX:
        a_plus_b &= cONE_LOWER16 

    arr_a[ind] = a_plus_b


def f_weighted_avg(ls: list[int], weights: list[int]) -> int:
    weighted_avg = 0

    for i, w in zip(ls, weights):
        weighted_avg += i * w

    return weighted_avg // len(ls)


def f_max_and_argmax(ls: array) -> array:
    curr_max = ls[0]
    curr_index = 0

    for i, intgr in enumerate(ls[1:]):
        if intgr > curr_index:
            curr_max = intgr
            curr_index = i

    return array('H', [curr_max, curr_index])


def f_main_and_argmin(ls: array) -> array:
    curr_min = ls[0]
    curr_index = 0

    for i, intgr in enumerate(ls[1:]):
        if intgr < curr_index:
            curr_min = intgr
            curr_index = i

    return array('H', [curr_min, curr_index])


def f_weighted_med(ls: list[int], weights: list[int]) -> int:
    weighted_med = 0

    for i, w in zip(ls, weights):
        weighted_med += i * w

    return (weighted_med + 1) // 2


def f_byte_to_array(barray: bytearray) -> array:
    return array('H', list(barray))


def f_pad_array_with_zero(arr: array) -> array:
    while len(arr) % cPOX_BLOCK_NUM != 0:
        arr.append(0)

    return arr


def f_get_8b_prime(num: int) -> array:
    remainder = num % cNUM_8B_PRIME
    prime = cPOX_8B_PRIMES[remainder]
    return array('H', [prime])


def f_get_bionom(n: int, k: int) -> int:
    if k > n:
        return 0
    if k == 0 or k == n:
        return 1

    return f_get_bionom(n - 1, k - 1) + f_get_bionom(n - 1, k)


def f_pox_factors_to_hex_digest(factor_array: array) -> str:
    hex_str_a = f"{factor_array[0]:04x}"
    hex_str_b = f"{factor_array[1]:04x}"
    hex_str_c = f"{factor_array[2]:04x}"
    hex_str_d = f"{factor_array[3]:04x}"

    return f"{hex_str_a}{hex_str_b}{hex_str_c}{hex_str_d}"


def f_pox_factors_to_byte_array(factor_array: array) -> str:
    return factor_array.tobytes()


def f_pox_alpha(temp_array: array) -> None:
    aleph = (temp_array[0] ^ temp_array[1]) & cMASK_ZZFF
    theh = (temp_array[2] ^ temp_array[3]) & cMASK_FFZZ
    daal = (aleph | theh) % cPOX_8B_PRIMES[0]
    gaaf = (aleph ^ theh) % cPOX_8B_PRIMES[1]

    temp_array[0] >>= daal
    temp_array[1] >>= ((daal + gaaf) % 2) + 1
    temp_array[2] >>= gaaf


def f_pox_delta(temp_array: array) -> None:
    alaf = (temp_array[0] ^ cMASK_FFFZ) % f_get_8b_prime(temp_array[0])[0]
    dalat = (temp_array[1] ^ cMASK_FZZF) % f_get_8b_prime(temp_array[1])[0]
    tit = (temp_array[2] & cMASK_ZFFF) % f_get_8b_prime(temp_array[2])[0]
    gaman = (temp_array[3] & cMASK_FFZZ) % f_get_8b_prime(temp_array[3])[0]

    for _ in range(cPOX_FACT_NUM):
        alaf >>= dalat
        dalat = f_rotate_left(dalat, 2)[0]
        tit >>= gaman
        gaman ^= alaf ^ cMASK_ZZFF

    temp_array[1] ^= temp_array[2] % cPOX_MAGIC_PRIMES[1]
    temp_array[2] ^= alaf + tit
    temp_array[3] ^= tit + gaman


def f_pox_theta(temp_array: array) -> None:
    alef = temp_array[0] % 2
    dalet = temp_array[1] % 2
    tet = temp_array[2] % 2
    gimmel = temp_array[3] % 2

    weighted_avg = f_weighted_avg(temp_array.tolist(),
                                  [alef, dalet, tet, gimmel])

    weighted_med = f_weighted_med(temp_array.tolist(),
                                  [alef, dalet, tet, gimmel])

    weighted_avg &= cONE_LOWER16
    weighted_med &= cONE_LOWER16

    uint16_avg = array('H', [weighted_avg])
    uint16_med = array('H', [weighted_med])

    temp_array[0] ^= ((uint16_avg[0] >> gimmel) ^ cMASK_ZZFF) & cMASK_ZZZF
    temp_array[3] ^= ((uint16_med[0] << alef) ^ cMASK_FZFZ) & cMASK_FZZZ


def f_pox_gamma(temp_array: array) -> None:
    min_and_argmin = f_main_and_argmin(temp_array)
    max_and_argmax = f_max_and_argmax(temp_array)

    mmin = min_and_argmin[0]
    mmax = max_and_argmax[0]

    argmin = min_and_argmin[1]
    argmax = max_and_argmax[1]
    aside, beside = [ind for ind in cRANGE_ZTF if ind not in [argmin, argmax]]

    alaph = temp_array[aside] % f_get_8b_prime(temp_array[aside])[0]
    dalath = ~(mmin ^ cMASK_FZFZ) % f_get_8b_prime(mmin)[0]
    teth = mmax % f_get_8b_prime(mmax)[0]
    gamal = temp_array[beside] % f_get_8b_prime((mmin + mmax) // 2)[0]

    temp_array[aside] >>= (alaph ^ cMASK_ZZFZ) % cWORD_WIDTH
    temp_array[argmin] >>= (gamal ^ cMASK_FZZZ) % ((mmax % 2) + 1)
    temp_array[argmax] ^= f_get_bionom(dalath, cWORD_WIDTH // 8) & cMASK_ZFFF
    temp_array[beside] ^= f_get_bionom(teth, cWORD_WIDTH // 8) >> (
        (gamal % 2) + 1)


def f_pox_round_op(temp_array: array) -> None:
    f_pox_alpha(temp_array)
    f_pox_delta(temp_array)
    f_pox_theta(temp_array)
    f_pox_gamma(temp_array)


def f_pox_round_apply_prime(temp_array: array) -> None:
    for pnum in cPOX_PRIMES:
        temp_array[0] ^= pnum
        temp_array[1] &= pnum
        temp_array[2] ^= pnum
        temp_array[3] &= pnum


def f_pox_round_add_tmp_to_facts(factor_array: array,
                                 temp_array: array) -> None:

    f_add_with_overflow(factor_array, temp_array, 0)
    f_add_with_overflow(factor_array, temp_array, 1)
    f_add_with_overflow(factor_array, temp_array, 2)
    f_add_with_overflow(factor_array, temp_array, 3)


def f_pox_apply_shuffle(temp_array: array) -> None:

    def f_inner_pox_shuffle(temp_array: array, indexof: int, indexwith: int):
        tmp = temp_array[indexof]
        temp_array[indexof] = temp_array[indexwith]
        temp_array[indexwith] = tmp

    for iof, iwith in cCOMB_BIONOM:
        f_inner_pox_shuffle(temp_array, iof, iwith)


def f_pox_round(factor_array: array) -> None:
    temporary_array = array('H', factor_array.tolist())

    f_pox_round_op(temporary_array)
    f_pox_round_apply_prime(temporary_array)
    f_pox_apply_shuffle(temporary_array)
    f_pox_round_add_tmp_to_facts(factor_array, temporary_array)


def f_pox_apply_bytes(factor_array: array, subportion: array) -> None:
    avg_subportion = sum(subportion) // cPOX_FACT_NUM
    med_subportion = (sum(subportion) + 1) // cPOX_FACT_NUM
    avg_odd_factor = cUINT16_MAX * (avg_subportion % 2)
    med_odd_factor = cUINT16_MAX * (med_subportion % 2)

    factor_array[0] ^= (subportion[0] + avg_subportion) ^ med_odd_factor
    factor_array[1] ^= (subportion[1] + med_subportion) ^ avg_odd_factor
    factor_array[2] ^= (subportion[2] + avg_subportion) ^ med_odd_factor
    factor_array[3] ^= (subportion[3] + med_subportion) ^ avg_odd_factor


def f_process_block(factor_array: array, block: list[int]) -> None:
    portions = [
        block[i:i + cPOX_PORTION_NUM]
        for i in range(0, cPOX_BLOCK_NUM, cPOX_PORTION_NUM)
    ]

    for portion in portions:
        subportions = [
            portion[j:j + cPOX_FACT_NUM]
            for j in range(0, cPOX_PORTION_NUM, cPOX_FACT_NUM)
        ]

        for k, subportion in enumerate(subportions):
           # print(k, subportion)
            for i in range(cPOX_ROUND_NUM):
                f_pox_apply_bytes(factor_array, subportion)
                f_pox_round(factor_array)

        #    print(factor_array)


def f_pox_hash(to_hash: bytearray) -> any:
    integer_list = f_byte_to_array(to_hash)
    integer_list = f_pad_array_with_zero(integer_list)
    blocks = [
        integer_list[i:i + cPOX_BLOCK_NUM]
        for i in range(0, len(integer_list), cPOX_BLOCK_NUM)
    ]

    factor_array = array(
        'H', [cPOX_PRIME_A, cPOX_PRIME_B, cPOX_PRIME_C, cPOX_PRIME_D])

    for block in blocks:
        f_process_block(factor_array, block)

    hex_digest = f_pox_factors_to_hex_digest(factor_array)
    bytes_array = f_pox_factors_to_byte_array(factor_array)

    return type(
        'PoxHash', (), {
            'rawarray': factor_array,
            'hexdigest': hex_digest,
            'bytearray': bytes_array
        })


print(f_pox_hash(b'abcdefghijk').hexdigest)
