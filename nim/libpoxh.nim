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

const NUM_HEX_SIZE  = 4

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
const HEX_DIGITS: array[16, char] = [
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    ]

type
    FactorArray = array[POX_PORTION_NUM, uint16]
    BlockArray = array[POX_BLOCK_NUM, uint16]
    PortionArray = array[POX_PORTION_NUM, uint16]
    InputSeq = seq[byte]
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

proc `++`(a: var int) = inc a

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

proc `+++`(arr: PortionArray): uint16 =
    for i in ...POX_PORTION_NUM:
        result += arr[i]

proc `--->`(sequence: var WordSeq, len: int) = 
    for _ in ...len:
        sequence.add(0u16)

proc `--->`(wseq: var WordSeq, bseq: InputSeq) =
    for (i, b) in enumerate(bseq):
        wseq[i] = ^^b

proc `--->`(arr1: FactorArray, arr2: var FactorArray) =
    for i in ...POX_PORTION_NUM:
        arr2[i] = arr1[i]

proc `--->`(wseqAndStart: (WordSeq, int), blockArray: var BlockArray) =
    var startIndex = wseqAndStart[1]
    var j = 0
    for i in startIndex...POX_BLOCK_NUM:
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

proc catAt(stringA: string, stringB: var string, start: int) =
    var j = start
    for i in ...stringA.len:
        stringB[j] = stringA[i]
        ++j

proc `+`(s1, s2: string): string =
    var contat = newString(s1.len + s2.len)
    catAt(s1, contat, 0)
    catAt(s2, contat, s1.len - 1)
    return contat

proc omega(num: uint32): uint32 = (num & MASK_DWORD_4F4Z) >> (BIT_WORD_WIDTH_U32)
proc epsilon(num: uint32): uint32 = num & MASK_DWORD_4Z4F
proc lamed(num: uint32, by: uint32): uint32 = (num << by) | (num >> (BIT_WORD_WIDTH_U32 - by))

proc `<<<`(num, by: uint16): uint16 =
    var res = ^^^^num
    var byd = ^^^^by
    res = lamed(res, byd)

    if res > BIT_UINT16_MAX_U32:
        res = omega(res)

    result = ^^res 

proc `<<<=`(num: var uint16, by: uint16) = num = num <<< by

proc `+=`(a: var FactorArray, b: (FactorArray, int)) =
    var index = b[1]
    var aa = ^^^^a[index]
    var bb = ^^^^b[0][index]
    var a_plus_b = aa + bb

    if a_plus_b > BIT_UINT16_MAX_U32:
        a_plus_b = epsilon(a_plus_b)

    a[index] = ^^a_plus_b

proc weightedAverage(factors: FactorArray, weights: PortionArray): uint16 =
    var wavg = 0u32
    for i in ...POX_PORTION_NUM:
        wavg += ^^^^factors[i] * ^^^^weights[i]

    wavg //= POX_PORTION_NUM
    if wavg > BIT_UINT16_MAX_U32:
        wavg = omega(wavg)

    result = ^^wavg
    
proc weightedMedian(factors: FactorArray, weights: PortionArray): uint16 =
    var wmed = 0u32
    for i in ...POX_PORTION_NUM:
        wmed += ^^^^factors[i] * ^^^^weights[i]

    wmed = (wmed + 1) // 2
    if wmed > BIT_UINT16_MAX_U32:
        wmed = epsilon(wmed)

    result = ^^wmed

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

proc decimalToHex(dec: uint16): string =
    result = newString(NUM_HEX_SIZE)
    var decCpy = dec
    for i in ...NUM_HEX_SIZE:
        result[NUM_HEX_SIZE - i - 1] = HEX_DIGITS[decCpy % BIT_WORD_WIDTH_U16]
        decCpy //= BIT_WORD_WIDTH_U16

proc factorArrayToHexDigest(warr: FactorArray): string =
    var hex1 = decimalToHex(warr[0])
    var hex2 = decimalToHex(warr[1])
    var hex3 = decimalToHex(warr[2])
    var hex4 = decimalToHex(warr[3])
    
    result = hex1 + hex2 + hex3 + hex4

proc byteArrayToPortionArrayAndPad(barray: InputSeq): WordSeq =
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
        dalat <<<= 2
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
        wavg: uint16
        wmed: uint16

    alef = tempArray[0] % 2
    dalet = tempArray[1] % 2
    tet = tempArray[2] % 2
    gimmel = tempArray[3] % 2

    wavg = weightedAverage(tempArray, [alef, dalet, tet, gimmel])
    wmed = weightedMedian(tempArray, [alef, dalet, tet, gimmel])

    tempArray[0] ^= ((wavg >> gimmel) ^ MASK_WORD_ZZFF) & MASK_WORD_ZZZF
    tempArray[3] ^= ((wmed << alef) ^ MASK_WORD_FZFZ) & MASK_WORD_FZZZ

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
        factorArray += (tempArray, i)

proc poxRound(factorArray: var FactorArray) = 
    var tempArray: FactorArray
    factorArray ---> tempArray
    poxRoundApplyAlphabet(tempArray)
    poxRoundApplyPrime(tempArray)
    poxRoundApplyShuffle(tempArray)
    poxRoundApplyAddition(factorArray, tempArray)

proc poxApplyByte(factorArray: var FactorArray, portion: PortionArray) =
    var 
        avg: uint16
        med: uint16
        sum: uint16
        avgOddFactor: uint16
        medOddFactor: uint16

    sum = +++portion
    avg = sum // POX_PORTION_NUM
    med = (sum + 1) // 2

    avgOddFactor = BIT_UINT16_MAX_U16 * (avg % 2)
    medOddFactor = BIT_UINT16_MAX_U16 * (med % 2)    
   
    factorArray[0] ^= (portion[0] + avg) ^ medOddFactor
    factorArray[1] ^= (portion[1] + med) ^ avgOddFactor
    factorArray[2] ^= (portion[2] + avg) ^ medOddFactor
    factorArray[3] ^= (portion[3] + med) ^ avgOddFactor

proc poxProcessBlock(factorArray: var FactorArray, blockArray: BlockArray) =
    var portion: PortionArray
    for i in ...(POX_BLOCK_NUM, POX_CHUNK_NUM):
        for j in i...(i + POX_CHUNK_NUM, POX_PORTION_NUM):
            (blockArray, j) ---> portion

            for _ in ...POX_ROUND_NUM:
                poxApplyByte(factorArray, portion)
                poxRound(factorArray)


type
    PoxHashTy* = object
        hexdigest*: string
        bytes*: array[8, uint8]
        words*: array[4, uint16]
        doubles*: array[2, uint32]
        quad*: uint64

proc PoxHash*(data: InputSeq): PoxHashTy =
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

    var hexdigest = factorArrayToHexDigest(factorArray)
    var bytes = factorsToByte(factorArray)
    var doubles = wordArrToDoubleArr(factorArray)
    var quad = wordArrToQuad(factorArray)

    var ret: PoxHashTy
    ret = PoxHashTy(
            hexdigest: hexdigest, 
            bytes: bytes, 
            words: factorArray, 
            doubles: doubles, 
            quad: quad
        )

    return ret