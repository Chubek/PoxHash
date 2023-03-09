##########################################
##      PoxHash v1                      ##
##      Implementation in Nim           ##
##      By Chubak Bidpaa - March 2023   ##
##      License: GPLv3                  ##
##########################################

const POX_PRIMES: array[32, uint16] = [
    0xe537u16, 0xbd71, 0x9ef9, 0xbbcf, 0xf8dd, 0xceb7, 0xbaa1, 0x8f9f, 0xb0ed,
    0xfc4f, 0x9787, 0xf01f, 0xe1d1, 0xbcb9, 0xd565, 0xc011, 0xc1e1, 0xb58d,
    0xd4e1, 0x9ea1, 0xee49, 0x97cd, 0xdac9, 0xe257, 0xa32b, 0xafbb, 0xa5e3,
    0xfc43, 0xbf71, 0xe401, 0x8ebd, 0xd549
]
const POX_8B_PRIMES: array[54, uint16] = [
    0x2u16, 0x3, 0x5, 0x7, 0xb, 0xd, 0x11, 0x13, 0x17, 0x1d, 0x1f, 0x25, 0x29,
    0x2b, 0x2f, 0x35, 0x3b, 0x3d, 0x43, 0x47, 0x49, 0x4f, 0x53, 0x59, 0x61,
    0x65, 0x67, 0x6b, 0x6d, 0x71, 0x7f, 0x83, 0x89, 0x8b, 0x95, 0x97, 0x9d,
    0xa3, 0xa7, 0xad, 0xb3, 0xb5, 0xbf, 0xc1, 0xc5, 0xc7, 0xd3, 0xdf, 0xe3,
    0xe5, 0xe9, 0xef, 0xf1, 0xfb
]
const POX_MAGIC_PRIMES: array[2, uint16] = [0x33u16, 0x65]
const POX_SINGLE_DIGIT_PRIMES: array[3, uint16] = [0x3u16, 0x5, 0x7]

const POX_PRIME_A = 0x9f91u16
const POX_PRIME_B = 0xdb3bu16
const POX_PRIME_C = 0xc091u16
const POX_PRIME_D = 0xac8bu16

const POX_BLOCK_NUM = 64
const POX_8BPRIME_NUM = 54
const POX_PRIME_NUM  = 32
const POX_CHUNK_NUM = 16
const POX_ROUND_NUM = 8
const POX_PORTION_NUM = 4
const POX_SD_PRIME_NUM = 3
const POX_MAGIC_PRIME_NUM = 2


const BIT_WORD_WIDTH_U16 = 16u16
const BIT_WORD_WIDTH_U32 = 16u32
const BIT_BYTE_WIDTH_U16 = 8u16
const BIT_UINT16_MAX_U16 = 65535u16
const BIT_UINT16_MAX_U32 = 65535u32

const MASK_DWORD_4F4Z = 0xffff0000u32
const MASK_DWORD_4Z4F = 0x0000ffffu32
const MASK_WORD_FZFZ = 0xf0f0u16
const MASK_WORD_ZFZF = 0x0f0fu16
const MASK_WORD_FZZZ = 0xf000u16
const MASK_WORD_ZZFZ = 0x00f0u16
const MASK_WORD_ZZZF = 0x000fu16
const MASK_WORD_ZZFF = 0x00ffu16
const MASK_WORD_FFZZ = 0xff00u16
const MASK_WORD_FZZF = 0xf00fu16
const MASK_WORD_FFFZ = 0xfff0u16
const MASK_WORD_ZFFF = 0x0fffu16
const MASK_WORD_01 = 0b01u16
const MASK_WORD_10 = 0b10u16
const MASK_WORD_11 = 0b11u16
const MASK_WORD_00 = 0b00u16

const COMB_BIONOM = @[(0, 1), (0, 2), (0, 3), (1, 2), (1, 3), (2, 3)]

const SEX_CHARS: array[60, char] = [
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 
]
const HEX_CHARS: array[16, char] = [
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    ]
const DUO_CHARS: array[12, char] = [
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '#',
    ]
const OCT_CHARS: array[8, char] = [
        '0', '1', '2', '3', '4', '5', '6', '7',
    ]
const BIN_CHARS: array[2, char] = [
    '0', '1',
    ]

const SEX_SIZE = 3
const HEX_SIZE = 4
const DUO_SIZE = 5
const OCT_SIZE = 6
const BIN_SIZE = 16
const SEX_BASE: uint16 = 60
const HEX_BASE: uint16 = 16
const DUO_BASE: uint16 = 12
const OCT_BASE: uint16 = 8
const BIN_BASE: uint16 = 2

type
    FactorArray = array[POX_PORTION_NUM, uint16]
    BlockArray = array[POX_BLOCK_NUM, uint16]
    PortionArray = array[POX_PORTION_NUM, uint16]
    ByteSeq = seq[byte]
    WordSeq = seq[uint16]

template `<->`(a, b: untyped): untyped =
    var tmp = a
    a = b
    b = tmp


proc `//`[T](a, b: T): T = a div b
proc `>>`[T](a, b: T): T = a shr b
proc `<<`[T](a, b: T): T = a shl b
proc `&`[T](a, b: T): T = a and b
proc `|`[T](a, b: T): T = a or b
proc `^`[T](a, b: T): T = a xor b
proc `%`[T](a, b: T): T = a mod b

proc `//=`[T](a: var T, b: T) = a = a // b
proc `>>=`[T](a: var T, b: T) = a = a >> b
proc `&=`[T](a: var T, b: T) = a = a & b
proc `^=`[T](a: var T, b: T) = a = a ^ b
proc `|=`[T](a: var T, b: T) = a = a | b

proc `^^^^*`[T](a: T): uint64 =  cast[uint64](a)
proc `^^^^`[T](a: T): uint32 =  cast[uint32](a)
proc `^^`[T](a: T): uint16 =  cast[uint16](a)
proc `^`[T](a: T): uint8 = cast[uint8](a)
proc `^-`[T](a: T): int8 = cast[int8](a)


proc `++`(a: var int) = inc a
proc `--`(a: var int) = dec a


iterator `...`(b: int): int =
    var i = 0
    while i < b:
        yield i
        ++i

iterator `...`(a, b: int): int =
    var i = a
    while i < b:
        yield i
        ++i

iterator `<...`(a, b: int): int =
    var i = b - 1
    while i >= a:
        yield i
        --i

iterator `<..+`(a, b: int): int =
    for i in a<...(a + b):
        yield i

iterator `...`(a: int, b: (int, int)): int =
    var i = a
    while i < b[0]:
        yield i
        i += b[1]

iterator `...`(a: (int, int)): int =
    for i in 0...a:
        yield i

iterator `..+`(a, b: int): int =
    for i in a...(a + b):
        yield i

iterator `...`[T](sequence: seq[T]): T =
    for i in ...sequence.len:
        yield sequence[i]

iterator enumerate[T](sequence: seq[T]): (int, T) =
    for i in ...sequence.len:
        yield (i, sequence[i])

proc `--->`(sequence: var WordSeq, len: int) = 
    for _ in ...len:
        sequence.add(0u16)

proc `--->`(wseq: var WordSeq, bseq: ByteSeq) =
    for (i, b) in enumerate(bseq):
        wseq[i] = ^^(^(^-b))

proc `--->`(arr1: FactorArray, arr2: var FactorArray) =
    for i in ...POX_PORTION_NUM:
        arr2[i] = arr1[i]

proc `--->`(wseqAndStart: (WordSeq, int), blockArray: var BlockArray) =
    var startIndex = wseqAndStart[1]
    var j = 0
    for i in startIndex..+POX_BLOCK_NUM:
        blockArray[j] = wseqAndStart[0][i]
        ++j

proc `--->`(barrAndStart: (BlockArray, int), portionArray: var PortionArray) =
    var startIndex = barrAndStart[1]
    var j = 0
    for i in startIndex..+POX_PORTION_NUM:
        portionArray[j] = barrAndStart[0][i]
        ++j

proc `:::`(num: uint16): uint16 =
    return POX_8B_PRIMES[num % POX_8BPRIME_NUM]

template convertBasesFromDecimal(base, size, chars, decimal, res, offset: untyped): untyped =
    var dec = decimal
    for i in (offset * size)<..+size:
        res[i] = chars[dec % base]
        dec //= base

proc omega(num: uint32): uint32 = (num & MASK_DWORD_4F4Z) >> (BIT_WORD_WIDTH_U32)
proc epsilon(num: uint32): uint32 = num & MASK_DWORD_4Z4F
proc ladca(num: uint32, by: uint32): uint32 = (num << by) | (num >> (BIT_WORD_WIDTH_U32 - by))

proc gorda(num, by: uint16): uint16 =
    var res = ^^^^num
    var byd = ^^^^by
    res = ladca(res, byd)

    if res > BIT_UINT16_MAX_U32:
        res = omega(res)

    result = ^^res 

proc tasu(factorArray: var FactorArray, tempArray: FactorArray, i: int) =
    var aa = ^^^^factorArray[i]
    var bb = ^^^^tempArray[i]
    var a_plus_b = aa + bb

    if a_plus_b > BIT_UINT16_MAX_U32:
        a_plus_b = epsilon(a_plus_b)

    factorArray[i] = ^^a_plus_b

proc centum(factors: FactorArray, weights: PortionArray): uint16 =
    var ctm = 0u32
    for i in ...POX_PORTION_NUM:
        ctm += ^^^^factors[i] * ^^^^weights[i]

    ctm //= POX_PORTION_NUM
    if ctm > BIT_UINT16_MAX_U32:
        ctm = omega(ctm)

    result = ^^ctm
    
proc satum(factors: FactorArray, weights: PortionArray): uint16 =
    var stm = 0u32
    for i in ...POX_PORTION_NUM:
        stm += ^^^^factors[i] * ^^^^weights[i]

    stm = (stm + 1) // 2
    if stm > BIT_UINT16_MAX_U32:
        stm = epsilon(stm)

    result = ^^stm

proc tamaam(factors: FactorArray): uint16 =
    var tmt = 0u32
    for i in ...POX_PORTION_NUM:
        tmt += ^^^^factors[i]

    tmt //= POX_PORTION_NUM
    if tmt > BIT_UINT16_MAX_U32:
        tmt = omega(tmt)

    result = ^^tmt
    
proc deca(factors: FactorArray): uint16 =
    var dca = 0u32
    for i in ...POX_PORTION_NUM:
        dca += ^^^^factors[i]

    dca = (dca + 1) // 2
    if dca > BIT_UINT16_MAX_U32:
        dca = epsilon(dca)

    result = ^^dca

proc minAndArgMin(factors: FactorArray): (uint16, uint16) =
    var currMin = factors[0]
    var currIndex = 0u16

    for i in ...POX_PORTION_NUM:
        if factors[i] < currMin:
            currMin = factors[i]
            currIndex = ^^i

    result = (currMin, currIndex)

proc maxAndArgMax(factors: FactorArray): (uint16, uint16) =
    var currMax = factors[0]
    var currIndex = 0u16

    for i in ...POX_PORTION_NUM:
        if factors[i] > currMax:
            currMax = factors[i]
            currIndex = ^^i

    result = (currMax, currIndex)

proc wordUpperBits(word: uint16): uint8 = ^((word & MASK_WORD_FFZZ) >> BIT_BYTE_WIDTH_U16)
proc wordLowerBits(word: uint16): uint8 = ^(word & MASK_WORD_ZZFF)
proc wordToByte(word: uint16): (uint8, uint8) = (wordLowerBits(word), wordUpperBits(word))
proc factorsToByte(factors: FactorArray): array[8, uint8] =
    var j = 0
    for i in ...POX_PORTION_NUM:
        (result[j], result[j + 1]) = wordToByte(factors[i])
        j += 2

proc wordToDouble(w1, w2: uint16): uint32 =
    result |= ^^^^w1
    result |= (^^^^w2) << 16
proc wordArrToDoubleArr(warr: FactorArray): array[2, uint32] =
    result[0] = wordToDouble(warr[0], warr[1])
    result[1] = wordToDouble(warr[2], warr[3]) 

proc wordArrToQuad(warr: FactorArray): uint64 =
    result |= ^^^^*warr[0]
    result |= (^^^^*warr[1]) << 16
    result |= (^^^^*warr[2]) << 32
    result |= (^^^^*warr[3]) << 48

proc wordArrayToSexDigest(warr: FactorArray): string =
    var sex = newString(SEX_SIZE * POX_PORTION_NUM)
    for i in ...POX_PORTION_NUM:
        var word = warr[i]
        convertBasesFromDecimal(SEX_BASE, SEX_SIZE, SEX_CHARS, word, sex, i)
    return sex

proc wordArrayToHexDigest(warr: FactorArray): string =
    var hex = newString(HEX_SIZE * POX_PORTION_NUM)
    for i in ...POX_PORTION_NUM:
        var word = warr[i]
        convertBasesFromDecimal(HEX_BASE, HEX_SIZE, HEX_CHARS, word, hex, i)
    return hex

proc wordArrayToDuoDigest(warr: FactorArray): string =
    var duo = newString(DUO_SIZE * POX_PORTION_NUM)
    for i in ...POX_PORTION_NUM:
        var word = warr[i]
        convertBasesFromDecimal(DUO_BASE, DUO_SIZE, DUO_CHARS, word, duo, i)
    return duo

proc wordArrayToOctDigest(warr: FactorArray): string =
    var oct = newString(OCT_SIZE * POX_PORTION_NUM)
    for i in ...POX_PORTION_NUM:
        var word = warr[i]
        convertBasesFromDecimal(OCT_BASE, OCT_SIZE, OCT_CHARS, word, oct, i)
    return oct

proc wordArrayToBinDigest(warr: FactorArray): string =
    var bin = newString(BIN_SIZE * POX_PORTION_NUM)
    for i in ...POX_PORTION_NUM:
        var word = warr[i]
        convertBasesFromDecimal(BIN_BASE, BIN_SIZE, BIN_CHARS, word, bin, i)
    return bin

proc byteArrayToPortionArrayAndPad(barray: ByteSeq): WordSeq =
    var length = barray.len
    while ^^^^length % POX_BLOCK_NUM != 0: ++length
    result ---> length
    result ---> barray

proc log2N(num: uint16): uint16 = 
    if num > 1: return 1 + log2N(num // 2) else: return 0

proc poxAlpha(tempArray: var FactorArray) =
    var 
      aleph: uint16
      theh: uint16
      daal: uint16
      gaaf: uint16

    aleph = (tempArray[0] ^ tempArray[1]) & MASK_WORD_ZZFF
    theh = (tempArray[2] ^ tempArray[3]) & MASK_WORD_FFZZ
    daal = (aleph | theh) % POX_8B_PRIMES[0]
    gaaf = (aleph ^ theh) % POX_8B_PRIMES[1]

    tempArray[0] >>= daal
    tempArray[1] >>= ((daal + gaaf) % 2) + 1
    tempArray[2] >>= gaaf

proc poxDelta(tempArray: var FactorArray) =
    var 
        alaf: uint16 
        dalat: uint16
        tit: uint16
        gaman: uint16

    alaf = (tempArray[0] ^  MASK_WORD_FFFZ) % :::tempArray[0]
    dalat = (tempArray[1] ^  MASK_WORD_FZZF) % :::tempArray[1]
    tit = (tempArray[2] &  MASK_WORD_ZFFF) % :::tempArray[2]
    gaman = (tempArray[3] &  MASK_WORD_FFZZ) % :::tempArray[3]

    for _ in ...POX_PORTION_NUM:
        alaf >>=  POX_SINGLE_DIGIT_PRIMES[dalat % POX_SD_PRIME_NUM]
        dalat = gorda(dalat, 2)
        tit >>=  POX_SINGLE_DIGIT_PRIMES[gaman % POX_SD_PRIME_NUM]
        gaman ^= (alaf ^  MASK_WORD_ZZFF) >>  POX_SINGLE_DIGIT_PRIMES[tit % POX_SD_PRIME_NUM]
    
    tempArray[1] ^= tempArray[2] %  POX_MAGIC_PRIMES[alaf % POX_MAGIC_PRIME_NUM]
    tempArray[2] ^= alaf + tit
    tempArray[3] ^= tit + gaman

proc poxTheta(tempArray: var FactorArray) =
    var 
        alef: uint16
        dalet: uint16
        tet: uint16
        gimmel: uint16
        ctm: uint16
        stm: uint16

    alef = tempArray[0] % 2
    dalet = tempArray[1] % 2
    tet = tempArray[2] % 2
    gimmel = tempArray[3] % 2

    ctm = centum(tempArray, [alef, dalet, tet, gimmel])
    stm = satum(tempArray, [alef, dalet, tet, gimmel])

    tempArray[0] ^= ((ctm >> gimmel) ^ MASK_WORD_ZZFF) & MASK_WORD_ZZZF
    tempArray[3] ^= ((stm << alef) ^ MASK_WORD_FZFZ) & MASK_WORD_FZZZ

proc poxGamma(tempArray: var FactorArray) =
    var 
        alaph: uint16 
        dalath: uint16
        teth: uint16
        gamal: uint16

        ay: uint16
        dee: uint16
        thorn: uint16
        gee: uint16

    var (mmax, argmax) = maxAndArgmax(tempArray)
    var (mmin, argmin) = minAndArgmin(tempArray)
    ay = argmin & MASK_WORD_01
    dee = argmax ^ MASK_WORD_10
    thorn = argmin & MASK_WORD_11
    gee = argmax ^ MASK_WORD_00

    alaph = tempArray[ay] % :::tempArray[thorn]
    dalath = (:::mmax ^ MASK_WORD_ZFZF) % :::mmin
    teth = mmax % :::mmax
    gamal = tempArray[dee] % :::(^^((^^^^(mmin) + ^^^^(mmax)) // 2))

    tempArray[ay] >>= (alaph ^ MASK_WORD_ZZFZ) % BIT_WORD_WIDTH_U16
    tempArray[dee] >>= (gamal ^ MASK_WORD_FZZZ) % ((mmax % 2) + 1)
    tempArray[thorn] ^= log2N(dalath) & MASK_WORD_ZFFF
    tempArray[gee] ^= log2N(teth) >> ((gamal % 2) + 1)

proc poxRoundApplyAlphabet(tempArray: var FactorArray) =
    poxAlpha(tempArray)
    poxDelta(tempArray)
    poxTheta(tempArray)
    poxGamma(tempArray)

proc poxRoundApplyPrime(tempArray: var FactorArray) =
    for i in ...POX_PRIME_NUM:
        tempArray[0] ^= POX_PRIMES[i]
        tempArray[1] &= POX_PRIMES[i]
        tempArray[2] ^= POX_PRIMES[i]
        tempArray[3] &= POX_PRIMES[i]

proc poxRoundApplyShuffle(tempArray: var FactorArray) =
    for (iof, iwith) in ...COMB_BIONOM:
        tempArray[iof] <-> tempArray[iwith]

proc poxRoundApplyAddition(factorArray: var FactorArray, tempArray: FactorArray) =
    for i in ...POX_PORTION_NUM:
        tasu(factorArray, tempArray, i)

proc poxRound(factorArray: var FactorArray) = 
    var tempArray: FactorArray
    factorArray ---> tempArray
    poxRoundApplyAlphabet(tempArray)
    poxRoundApplyPrime(tempArray)
    poxRoundApplyShuffle(tempArray)
    poxRoundApplyAddition(factorArray, tempArray)

proc poxApplyByte(factorArray: var FactorArray, portion: PortionArray, index: uint16) =
    var 
        tmt: uint16
        dca: uint16
        tmtOddFactor: uint16
        dcaOddFactor: uint16
    
    tmt = tamaam(portion)
    dca = deca(portion)    
    tmtOddFactor = BIT_UINT16_MAX_U16 * (tmt % 2)
    dcaOddFactor = BIT_UINT16_MAX_U16 * (dca % 2)    

    var ng = (portion[0] + index) % POX_PORTION_NUM
    var chu = (portion[1] + index) % POX_PORTION_NUM
    var yo = (portion[2] + index) % POX_PORTION_NUM
    var eo = (portion[3] + index) % POX_PORTION_NUM 

    factorArray[ng] ^= (portion[eo] | tmt) ^ dcaOddFactor
    factorArray[chu] ^= (portion[yo] & dca) ^ tmtOddFactor
    factorArray[yo] ^= (portion[chu] ^ tmt) ^ dcaOddFactor
    factorArray[eo] ^= (portion[ng] | dca) ^ tmtOddFactor

proc poxProcessBlock(factorArray: var FactorArray, blockArray: BlockArray) =
    var portion: PortionArray
    for i in ...(POX_BLOCK_NUM, POX_CHUNK_NUM):
        for j in i...(i + POX_CHUNK_NUM, POX_PORTION_NUM):
            (blockArray, j) ---> portion

            for m in ...POX_ROUND_NUM:
                poxApplyByte(factorArray, portion, ^^m)
                poxRound(factorArray)

type
    PoxHashTy* = object
        sexdigest*: string
        hexdigest*: string
        duodigest*: string
        octdigest*: string
        bindigest*: string
        bytes*: array[8, uint8]
        words*: array[4, uint16]
        doubles*: array[2, uint32]
        quad*: uint64

proc PoxHash*(data: ByteSeq): PoxHashTy =
    ## Converts the given byte seq into a PoxHashTy object
    ## Parameters:
    ##       data: seq[byte]
    ## 
    ## Returns:
    ##      PoxHashTy
    ##          PoxHashTy.hexdigest: string
    ##          PoxHashTy.bytes: array[8, uint8]
    ##          PoxHashTy.words: array[4, uint16]
    ##          PoxHashTy.doubles: array[2, uint32]
    ##          PoxHashTy.quad: uint64
    var padded = byteArrayToPortionArrayAndPad(data)
    var factorArray: FactorArray = [POX_PRIME_A, POX_PRIME_B, POX_PRIME_C, POX_PRIME_D]
    var blockArray: BlockArray

    for i in ...(padded.len, POX_BLOCK_NUM):
        (padded, i) ---> blockArray
        poxProcessBlock(factorArray, blockArray)

    var sexdigest = wordArrayToSexDigest(factorArray)
    var hexdigest = wordArrayToHexDigest(factorArray)
    var duodigest = wordArrayToDuoDigest(factorArray)
    var octdigest = wordArrayToOctDigest(factorArray)
    var bindigest = wordArrayToBinDigest(factorArray)
    var bytes = factorsToByte(factorArray)
    var doubles = wordArrToDoubleArr(factorArray)
    var quad = wordArrToQuad(factorArray)

    var ret: PoxHashTy
    ret = PoxHashTy(
            sexdigest: sexdigest,
            hexdigest: hexdigest, 
            duodigest: duodigest,
            octdigest: octdigest,
            bindigest: bindigest,
            bytes: bytes, 
            words: factorArray, 
            doubles: doubles, 
            quad: quad
        )

    return ret