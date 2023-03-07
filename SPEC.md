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

```
#BLOCK_NUM = 64 ---> Size of each block
#8B_PRIME_NUM = 54 ---> Number o 8bit primes in the collective [...]8B_PRIMES
#PRIME_NUM = 32 ---> Number of 16bit primes n the collective [...]PRIMES
#CHUNK_NUM = 16 ---> Size of each block chunk
#ROUND_NUM = 8 ---> Number of rounds
#PORTION_NUM = 4 ---> Size of chunk portion, and also the number of factors!
#SD_PRIME_NUM = 3 ---> Number of single-digit primes (3, 5, 7) in [...]SD_PRIMES
#MAGIC_PRIME_NUM = 2 ---> Number of prime numbers in [...]MAGIC_PRIMES

These are the primes which the factor array is instanciated with:

#PRIME_A = 0x9f91
#PRIME_B = 0xdb3b
#PRIME_C = 0xc091
#PRIME_D = 0xac8b

These are bitwise contracts:
#WORD_WIDTH = 16 ---> 16 bits in a word
#BYTE_WIDTH = 8 ---> 8 bits in a byte
#MAX_WORD = 0xffff ---> We need this for multiplication. Equivalent of 65535u16

These are the masks we will use for bitwise operations:
#DOUBLE_WORD_UPPER_ONE = 0xffff0000
#DOUBLE_WORD_LOWER_ONE = 0x0000ffff
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
#NIBBLE_01 = 0b01
#NIBBLE_10 = 0b10
#NIBBLE_11 = 0b11
#NIBBLE_00 = 0b00
```

