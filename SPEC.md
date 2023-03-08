# PoxHash Specification


## Overview

PoxHash is a block hashing algorithm by the author of this document, Chubak Bidpaa (Chubak#7400 on Discord) that focuses on possible universality and irreversibility. Speed was not a factor in design of PoxHash. However the implementations in this repository are fast enough.

Pox accepts byte buffers but it operates on words. Some implementations cast the bytes into uint16 before the rounds start, but some implementations such as C cast them in-action. 

There are 4 word facors at play in Pox. All the factors are initialized with 4 different randomly-chosen uint16 prime numbers. Prime numbers play a large role in Pox, as we'll see.

A Pox round is comprised of 4 operations. The *Alphabet Operation*, the *Prime Apply Operation*, the *Shuffle Operation* and the *Addition with Special Overflow* operation. Each of these will be explained soon.

Each round is applied to each 4 byte of the passed buffer a total of 8 times. The block size is 64 bytes and each block is split into 4 portions of 16 in size. So in other words, The byte buffer (after being padded with 0s so the size would be divisible by 64) is split into blocks of 64, and each block is split into chunks of 16, and each chunk is split up into portions of 4, and each of these portions is put through the round 8 times.

Pox uses a special operation to apply byte portion to factors, as we'll see. After the blocks have ran, the factors can be converted into bytes, hexdigest, or used raw. All the implementations in this repository pass an object that has all these 3.

The steps of this of the process can be described as such:

```
    Key operations and concepts are in all-caps
    Key numeric constants are prefixed with '$'

    PASS: BYTEBUFF of size N
    STEP 0-A: PAD the BYTEBUFF with 0s after converting to WORDARRAY
    STEP 0-B: INITIATE the $4 FACTORs with their PRIME constants
    STEP 1: SPLIT the WORDARRAY into M BLOCKs of $64
    STEP 2: SPLIT the BLOCK into 4 CHUNKs of $16
    STEP 3: SPLIT the CHUNK into 4 PORTIONs of $4
    STEP 4-A: APPLY THE WORD PORTION to the each of the FACTORs
    STEP 4-B: APPLY ROUND
        ROUND STEP 0: Copy each FACTOR into TEMPORARIES
        ROUND STEP 1: ALPHABET OPERATION on TEMPORARIES
        ROUND STEP 2: PRIME APPLY OPERATION on TEMPORARIES
        ROUND STEP 3: SHUFFLE OPERATION on TEMPORARIES
        ROUND STEP 4: ADD the TEMPORARIES to FACTORs
    FINALIZE: CONVERT the FACTORs into HEXDIGEST and BYTEARRAY
```

In the upcomin sections we will explain all these steps in detail.

## Constants

We mentioned that there are several constant, standard values in use at the overall algorithm. There's two types of constants, collective constants and numeric constants. First, let's take a look at the numeric constants:

I'll denote numeric constants with `#` and collective constants with `[...]`

### Numeric COnstants

```
#BLOCK_NUM = 64 -> Size of each block
#8B_PRIME_NUM = 54 -> Number o 8bit primes in the collective [...]8B_PRIMES
#PRIME_NUM = 32 -> Number of 16bit primes n the collective [...]PRIMES
#CHUNK_NUM = 16 -> Size of each block chunk
#ROUND_NUM = 8 -> Number of rounds
#PORTION_NUM = 4 -> Size of chunk portion, and also the number of factors!
#SD_PRIME_NUM = 3 -> Number of single-digit primes (3, 5, 7) in [...]SD_PRIMES
#MAGIC_PRIME_NUM = 2 -> Number of prime numbers in [...]MAGIC_PRIMES

These are the primes which the factor array is instanciated with:

#PRIME_A = 0x9f91
#PRIME_B = 0xdb3b
#PRIME_C = 0xc091
#PRIME_D = 0xac8b

These are bitwise contracts:
#WORD_WIDTH = 16 -> 16 bits in a word
#BYTE_WIDTH = 8 -> 8 bits in a byte
#MAX_WORD = 0xffff -> We need this for multiplication. Equivalent of 65535u16

These are the masks we will use for bitwise operations:
#DOUBLE_WORD_4F4Z = 0xffff0000
#DOUBLE_WORD_4Z4F = 0x0000ffff
#WORD_FZFZ = 0xf0f0
#WORD_ZFZF = 0x0f0f
#WORD_FZZZ = 0xf000
#WORD_ZZFZ = 0x00f0
#WORD_ZZZF = 0x000f
#WORD_ZZFF = 0x00ff
#WORD_FFZZ = 0xff00
#WORD_FZZF = 0xf00f
#WORD_FFFZ = 0xfff0
#WORD_ZFFF = 0x0fff
#NIBBLET_01 = 0b01
#NIBBLET_10 = 0b10
#NIBBLET_11 = 0b11
#NIBBLET_00 = 0b00

#COMB_BIONOM_SIZE = 6 -> size of [...]COMB_BIONOM
```
### Collective Constants

Now, the collective constants are:

```
These are the random prime numbers that will be used in Apply Prime operation during each round
[...]PRIMES = [
    0xe537, 0xbd71, 0x9ef9, 0xbbcf, 0xf8dd, 0xceb7, 0xbaa1, 0x8f9f, 0xb0ed,
    0xfc4f, 0x9787, 0xf01f, 0xe1d1, 0xbcb9, 0xd565, 0xc011, 0xc1e1, 0xb58d,
    0xd4e1, 0x9ea1, 0xee49, 0x97cd, 0xdac9, 0xe257, 0xa32b, 0xafbb, 0xa5e3,
    0xfc43, 0xbf71, 0xe401, 0x8ebd, 0xd549
]

These are the 8-bit primes, which are used in alphabet operations
[...]8B_PRIMS = [
    0x2, 0x3, 0x5, 0x7, 0xb, 0xd, 0x11, 0x13, 0x17, 0x1d, 0x1f, 0x25, 0x29,
    0x2b, 0x2f, 0x35, 0x3b, 0x3d, 0x43, 0x47, 0x49, 0x4f, 0x53, 0x59, 0x61,
    0x65, 0x67, 0x6b, 0x6d, 0x71, 0x7f, 0x83, 0x89, 0x8b, 0x95, 0x97, 0x9d,
    0xa3, 0xa7, 0xad, 0xb3, 0xb5, 0xbf, 0xc1, 0xc5, 0xc7, 0xd3, 0xdf, 0xe3,
    0xe5, 0xe9, 0xef, 0xf1, 0xfb
]

[...]MAGIC_PRIMES = [0x33, 0x65] -> Primes that's been proven to be good for hashing, 51 and 101 in decimal

[...]SD_PRIMES = [0x3, 0x5, 0x7] -> Single digit primes

These are the bionomial coefficients (combinations) of 4 and 2, they will be used in shuffling
[...]COMB_BIONOM = [(0, 1), (0, 2), (0, 3), (1, 2), (1, 3), (2, 3)] 
```
Let's get to the next section wherein we explain our operetional contracts.

## Operational Contracts

We need to set an standard for the operations we're going to use. These operations indclude bitwise, statistical and calculative operations. We will also define a few terms.

### Bit Size Terminology

In this document we will use the x86-64 word size as the size of our words, that is, 16bits. So we will have:

```
Quad Word -> 64bits
Double WOrd -> 32bits
Word -> 16bits
Byte -> 8bits
```

We will also use the term *nibblet* to refer to 2bits. *Nibbles*, on the other hand, are 4bits. We have no use for quads or nibbles.

### Basic Arithmetic Operations

We define the following arithmetic operations:

```
($a, $b: Numeric)ADD = $a + $b
($a, $b: Numeric)MUL = $a * $b
($a, $b: Numeric)INTDIV = floor($a / $b)
($a, $b: Numeric)MOD = $a % $b
($arr: NumericArray)SUM = $arr[0] + $arr[1] + ... + $arr[n]
```

### Bitwise Operations

#### Basic

The basic bitwise operators are defined as below. 1 and 0 are bits.

|Operation|(0, 0)|(0, 1)|(1, 1)|
|---------|------|------|------|
|**Xor**|0|1|0|
|**Or**|0|1|1|
|**And**|0|0|1|

**Not** is unary and is defined as `Not(1) = 0` and `Not(0) = 1`.

There's two bitwise operations that operate on on number given an amount. They are **Left Shift** and **Right Shift**. They shift the bits to left and right given a number as the amount. They replace the bits that were shifted with 0.


```
Shl(0b0011, 1) -> 0b0110
Shr(0b110, 1) -> 0b0011
```

### Compound

We defne the following bitwise operations that we will later use. Operations will be denoted by `(*params: <paramtype>)`. When we refer to these operations, we will omit the params.

```
($a: DoubleWord)OMEGA = Shr(And($a, #DOUBLE_WORD_4F4Z), #WORD_WIDTH)
($a: DoubleWord)EPSILON = And($a, #DOUBLE_WORD_4Z4F)
($a: DoubleWord, $by: Numeric)LAMED = Or(Shl($a, $by), Shr($a, (#WORD_WIDTH - $by)))
```
(Lamed is a Phonecian letter name)

Now let's define our custom rotate-bits-left function, with a twist.

```
($num: Word, $by: Numeric)ROTATE_LEFT =
        $tmpDouble = $num as DoubleWord
        $tmpDouble = LAMED($tmpDouble)
        $tmpDOuble = OMEGA($mpDuble) only if $tmpDouble > #MAX_WORD
        return $tmpDouble as Word
```

## Statistical Operations

To make the hash irreversible, pox applies four statistical accumulators at two sages of the operation. Average and median, applied when portions are applied to the factor array (as we'll see!) and their weighted counteparts, during the alphabet operations.

We hereby define these operatios:

```
($arr: WordArray)AVERAGE = INTDIV(SUM(arr), #PORTION_NUM)
($arr: WordArray)MEDIAN = INTDIV(SUM(arr) + 1, 2)

($arr: WordArray, $weights: NumericArray)AVERAGE_WEIGHTED = 
        $wavg: DoubleWord = 0
        for $i in 0...#PORTION_NUM:
            ADD($wavg, $arr[$i] * $weights[$i])
        INTDIV($wavg, #PORTION_NUM)
        OMEGA($wavg) only if $wavg > #MAX_WORD
        return $wavg as Word

($arr: WordArray, $weights: NumericArray)AVERAGE_WEIGHTED = 
        $wmed: DoubleWord = 0
        for $i in 0...#PORTION_NUM:
            ADD($wmed, $arr[$i] * $weights[$i])
        INTDIV($wmed + 1, 2)
        EPSILON($wmed) only if $wmed > #MAX_WORD
        return $wmed as Word
```

## Compound Arithmetic Operations

During the rounds, we apply the round ot a temporary copy of the factors array. We need to add it up at the end. We use a special arithmetic operation for it.

```
($a: Word, $b: Word)SPECIAL_ADD =
        $aa = $a as DoubleWord
        $bb = $b as DoubleWord
        $sum: DoubleWord = $aa + $bb
        EPSILON($sum) only if $sum > #MAX_WORD
        return $sum as Word
```

## Alphabet Operations