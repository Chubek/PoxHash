# PoxHash: A Checksum Hash

Please use the menu below to navigate to each section.

* [Purpose](#purpse)
* [Preservations & Uses](#preservations--uses)
* [Specification](#specification)
* * * [Bit Size & Block Size](#bit-size--block-size)
* * * [Prime Factors](#prime-factors)
* * * [Round Operations](#round-operations)
* * * [Processing the Block](#processing-the-block)
* * * [The Main Hashing](#the-main-hashing)

## Purpose

I wrote Pox for my C++ Python code runner [Slitherbrain](https://raw.githubusercontent.com/Chubek/Slitherbrain). The aim of this repository is to provide the user with ways to generate Pox hashes in several languages both for Slitherbain, and general use.

**Note**: If you fork this repo and make a meaningful contribution, please contact me on Discord, ID in profile, and I will add your repository and credit here.

## Preservations & Uses

Pox is not designed to be universal. Collisions may happen. Pox is also not a cryptographic hashing algorithm and shan't be used for secure hashing.

The best use for Pox is for checksums, such as integrity hashing. I'd argue that universality is not at all needed for a checksum method as what's important is not uniqueness, but hashes being the same across transfer. Ergo, a hash that is consistent is more important than a hash that is universal.

## Specification

### Bit Size & Block Size

Pox is a 64bit hash. In its initial implementation in Slitherbrain codebase, it is comprised of 4 sixteen-bit unsigned integers. As such, Pox's block number is 64 character, meaning it operates on 64 bytes at the same time, or 512 bits. So in other words, Pox is made up of four 16bit factors, and processes 64 bytes ro 512 bits at the same time. The smallest number of bits that Pox accepts is 8, so nibbles are disallowed. 

### Prime Factors

Pox uses prime numbers in a style similar to  SHA-family of algorithms to scramble the bits in each round of hashing. Pox makes uuse of a total 36 number of 16-bit prime numbers, all between 35,000 and 65,000 --- 32 of which is used during the rounds and 4 of which is used to initialize the factors. 

The 32 prime numbers used during the rounds are, in hexadicmal base:

```
0xe537, 0xbd71, 0x9ef9, 0xbbcf, 0xf8dd, 0xceb7, 0xbaa1, 0x8f9f,
0xb0ed, 0xfc4f, 0x9787, 0xf01f, 0xe1d1, 0xbcb9, 0xd565, 0xc011,
0xc1e1, 0xb58d, 0xd4e1, 0x9ea1, 0xee49, 0x97cd, 0xdac9, 0xe257,
0xa32b, 0xafbb, 0xa5e3, 0xfc43, 0xbf71, 0xe401, 0x8ebd, 0xd549
```

and the four numbers used to initialize the factors are:

```
A = 0x9f91;
B = 0xdb3b;
C = 0xc091;
D = 0xac8b;  
```

These numbers were chosen quite randomy, generated using SymPy. Since Pox is not a cryptographic hash I did not see a reason to put scrunity on what the best prime number is. The aim is scrambling, not uniqueness of hash and universality and neiher security. Keep that in mind!

### Round Operations

At the beginning of each round, Pox makes copies of the factors to operate on them. We call them A', B', C' and D'.

In each round, Pox uses three operations for scrambling the data on each factor. I call them Alpha (α), Delta (δ), Theta (θ) and Omega (ω). They all operate on A', B', C', and D' respectively. The functions are as follows:

(It uses bitwise rotations, to left and right. The base bit number for rotation is 16 and number of rotated bits is 4. Rotate left is defined as `RL = (NUM << 4) | (NUM >> (16 - 4))` and rotate right is defined as `RR = (NUM >> 4) | (NUM << (16 - 4)>>)`).

```
α(A') -> A' = A' | RL(A') | RR (A' << 2)
δ(A', B') -> B' = (RL(B') ^ A') | 0xffcd
θ(A', B', C') -> C' = ((A' * RR(B' & A'))) >> 2
ω(A', B', C', D') -> D' = ((A' >> 2) ^ (B' >> 4) ^ (C' >> 6)) | RL(D')
```

After that, for each of the 32 round prime numbers, it will do the following operations.

```
for each prime number:
    A' = B' ^ primenum;
    B' = C' & primenum;
    C' = D' | primenum;
    D' = A' >> 2;
```

At the end, we take the main factors, and add the temporary numbers sto them. However we divide the number by even numbers between 2 and 8.

```
A += A' / 2;
B += B' / 4;
C += C' / 6;
D += D' / 8;
```

### Processing the Block

As we said, size of the block is 64 bytes. We divide this block by 4 portions of 16 byte each. Apply each 4 byte in the following way and apply the round a total of 8 times. The algorithm for processing the block is:


```
BLOCK_NUM = 64
PORTION_NUM = 16
ROUND_NUM = 8
FACTOR_NUM = 4

for i in 0 to BLOCK_NUM inc by PORTION_NUM:
    for j in i to i + PORTION_NUM inc by FACTOR_NM:
        A |= block[j];
        B ^= block[j + 1];
        C &= block[j + 2] + 1;
        D = ~block[j + 3];

        for k in 0 to ROUND_NUM inc by 1:
            applyRound(A, B, C, D);
```

### The Main Hashing

And finally, in the main hashing function, we first set the factors to the 4 prime numbers mentioned above --- then we feed the process block function chnks of 64 byte.

```
A = PRIME_NUM_A
B = PRIME_NUM_B
C = PRIME_NUM_C
D = PRIME_NUM_D

BLOCK_NUM = 64

for i in 0 to bytes_in_message inc by BLOCK_NUM:
    sub_message = message.sub(i, BLOCK_NUM bytes)
    processBlock(sub_message, A, B, C, D)
```

At the end we end up with 4 factors, each 2 bytes. Our hash at the end willl be 8 bytes. We can represend that as a 16-letter hex digest of 8 unsigned integers.