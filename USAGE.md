# Using the Formal Implementations of PoxHash

This repository contains implementations of PoxHash in Header-Only C, Go, Python, Rust, Nim and JS. If you wish to know more about PoxHash read the [specifications](/SPEC.md).

All the implementations are in functional paradigm. They all rely on zero 3rd party libraries and calls the respective standard library of the language is minimal at best.

To use the code just download the file into your working directory and import the functions mentioned in the [interfaces](#interfaces) section.

# Importing

## C

Download to your working directory:

```bash
wget https://raw.githubusercontent.com/Chubek/PoxHash/master/c/libpoxh.h -qO $(pwd)/libpoxh.h
```

It should be mentioned that the C implementation is header-only and does not need object linking.

This assumes `libpoxh.h` is in the same level as you `*.c` file.

```c
#ifndef __POX_H_INC   // include guards are not necessary, but recommended
#define __POX_H_INC
#include "libpoxh.h";
#endif
#ifndef __STDINT_H_INC
#define __STDINT_H_INC
#include <stdint.h> // we need this for uint8_t
#endif
```

## Go

Download to your working directory:

```bash
wget https://raw.githubusercontent.com/Chubek/PoxHash/master/go/libpoxh.go -qO $(pwd)/libpoxh.go
```

Make sure your directory structure looks like this:

```
project_dir/
  libpoxh/
    libpoxh.go
+ main.go
```

Assuming your project name is `projectname`:

```go
import (
    "fmt" // just for the sake of demonstration
    "projectname/libpoxh"
)
```

## JavaScript

Download to your working directory:

```bash
wget https://raw.githubusercontent.com/Chubek/PoxHash/master/js/libpoxh.js -qO $(pwd)/libpoxh.js
```

Running JavaScript is simple. Just download the file into your directory and import it using CommonJS import syntax.

```js
const libpox = require("libpoxh");
```

## Nim

Download to your working directory:

```bash
wget https://raw.githubusercontent.com/Chubek/PoxHash/master/nim/libpoxh.nim -qO $(pwd)/libpoxh.nim
```

After downloading, import it like this:

```nim
form libpoxh import PoxHash, PoxDigest
```

## Python

Download to your working directory:

```bash
wget https://raw.githubusercontent.com/Chubek/PoxHash/master/python/libpoxh.python -qO $(pwd)/libpoxh.py
```

It really depends on how you are doing imports in your project. But assuming you are not doing relative imports, and your file is on the same level as the `lixpoxh.py`:

```
from libpoxh import pox_hash, PoxDigest
```

## Rust

### Cargo

First `cd` into your project folder, where `Cargo.toml` is, and:
Download to your working directory:

```bash
wget https://raw.githubusercontent.com/Chubek/PoxHash/master/rust/libpoxh.ts -qO $(pwd)/src/libpoxh.rs
```

```rust
use libpoxh::{pox_hash, PoxDigest};
```

Then you can use `cargo run` and `cargo build` normally.

### Raw Compiler (rustc)

After downloading the file, compile it into an `rlib` file using:

```bash
rustc libpoxh.rs -O --crate-type lib --crate-name libpoxh -o libpoxrust.rlib
```

and when you import:

```rust
extern crate libpoxh;
use libpoxh::{pox_hash, PoxDigest};
```

And compile with `--extern` flag to link:

```bash
rustc myapplication.rs -O --extern libpoxh=libpoxrust.rlib -o myapplicationbinary
```

# Interface

All the implementations provide a simple function that will accept the message in unsigned byte form. They are, as follows:

| Language | Function | Accepts          | Returns                        |
| -------- | -------- | ---------------- | ------------------------------ |
| C        | pox_hash | uint8_t\*        | struct PoxDigest (poxdigest_t) |
| Go       | PoxHash  | []uint8          | type struct PoxDigest          |
| JS       | poxHash  | Uint8Array       | PoxDigest typefunc             |
| Nim      | PoxHash  | seq[uint8]       | object PoxDigest               |
| Python   | pox_hash | array.array('B') | class PoxDigest                |
| Rust     | pox_hash | &Vec<u8>         | struct PoxDigest               |

## Input Type

All the variables are a variation of an unsigned 8-bit integer array.

For static languages, it is obvious, but for Python, you need to convert a regular bytearray into a uint8 array like this:

```python
def to_uint8_array(message: bytearray) -> any:
    from array import array
    array('B', message)
```

And for JavaScript:

```js
const toUint8Array = (message) => {
  return new Uint8Array(message);
};
```

If you wish to hash a character string, strings in Python and JS strings are not representations of the underlying bytes. You need to encode them first. Be very careful about encoding, as different codecs have different corresponding bytes for each 'raw' Python/JS glyph.

To endcode an string to UTF-8 in JavaScript:

```js
const stringToByteArray = (str) => {
  let utf8Encode = new TextEncoder();
  byteArray = utf8Encode.encode(str);
  return byteArray;
};
```

And in python you can just do `myMessage.encode('utf8')`.

Rust, Go and Nim character strings can be easily converted to byte rrays and from there, unsigned bytearrays. In C, the char type is the same as the byte type. C has no special type for graphemes. Whereas in Rust and Nim the `char` type and in Go the `rune` type represents a character, in C they're one and the same.

To convert a string into bytes:

- Go:

```go
signedByteArray := []byte(myStr)
```

- Rust converts strings to unsigned bytes by default:

```rust
let signed_byte_array = my_str.as_bytes().to_vec();
```

- Nim:

```nim
var signedByteArray = toOpenArrayByte(myStr)
```

Use the following ways to convert data in int8, byte or glyph (char) type into uint8 form in static languages:

- C:

```c
const uint8_t unsigned_array = (uint8_t *)signed_array;
```

- Go:

```go
unsignedArray := []uint8(signedArray)
```

- Nim:

```nim
import sequtils
var unsignedArray = map(signedArray, proc(x: byte): uint8 = cast[uint8](x))
```

- Rust:

```
let unsigned_array: Vec<u8> = signed_array.into_iter().map(|b| b as u8).collect();

// if you have a string just do this
let unsigned_array = myString.as_bytes().to_vec();
```

### Shortcut: Convrt String Straight to U8[]

- Go:

```go
unsignedArray := []uint8(myStr)
```

- Nim:

```nim
import sequtils
var unsignedArray = map(myStr, proc(x: char): uint8 = cast[uint8](x))
```

## PoxDigest Result Type

All these functions return an structured object, be it a struct, a class, or anything else that is a structured object, called `PoxDigest`. Across the implementations, this structured object has 11 properties (also called 'fields') that are identical in content and name (except in Go, where they only slightly differ). These properties are digests in various forms, decimal or non-decimal, decimal in several bit widths and non-decimal in several bases. Let's go through them.

### PoxDigest Object Fields/Properties

#### Field Names

| In Go     | In Others | Decenary    |
| --------- | --------- | ----------- |
| Sexdigest | sexdigest | Non-Decimal |
| Vigdigest | vigdiest  | Non-Decimal |
| Hexdigest | hexdigest | Non-Decimal |
| Tetdigest | tetdigest | Non-Decimal |
| Duodigest | duodigest | Non-Decimal |
| Octdigest | octdigest | Non-Decimal |
| Sendigest | sendigest | Non-Decimal |
| Bindigest | bindigest | Non-Decimal |
| Bytes     | bytes     | Decimal     |
| Words     | words     | Decimal     |
| Doubles   | doubles   | Decimal     |
| Quad      | quad      | Decimal     |

#### Non-Decimal Digest Field Types

| Language   | Type                                   |
| ---------- | -------------------------------------- |
| C          | char[digest_size + 1], null-terminated |
| Go         | string                                 |
| JavaScript | string                                 |
| Nim        | string                                 |
| Python     | str                                    |
| Rust       | String                                 |

Refer to [Non-Decimal Digests](#non-decimal-digests) section for more info

#### Decimal Digest Field Types

##### Bytes

| Language | Type                |
| -------- | ------------------- |
| C        | uint8_t[8]          |
| Go       | [8]uint8            |
| JS       | Uint8Array[8]       |
| Nim      | array[8, uint8]     |
| Python   | array.array('B', 8) |
| Rust     | [u8; 8]             |

##### Words

| Language | Type                |
| -------- | ------------------- |
| C        | uint16_t[4]         |
| Go       | [4]uint16           |
| JS       | Uint16Array[4]      |
| Nim      | array[4, uint16]    |
| Python   | array.array('H', 4) |
| Rust     | [u16; 4]            |

##### Doubles

| Language | Type                |
| -------- | ------------------- |
| C        | uint32_t[2]         |
| Go       | [2]uint32           |
| JS       | Uint32Array[2]      |
| Nim      | array[4, uint32]    |
| Python   | array.array('I', 2) |
| Rust     | [u32; 2]            |

##### Quad

| Language | Type                |
| -------- | ------------------- |
| C        | uint64_t            |
| Go       | uint64              |
| JS       | BigInt.asUintN(64)  |
| Nim      | uint64              |
| Python   | array.array('Q', 1) |
| Rust     | u64                 |

Refer to [Decimal Digests](#decimal-digests) section for more info.

### Note on `PoxDigest` in Rust

The `PoxDigest` type in Rust implements the following traits:

```rust
std::clone::Clone;
std::default::Default;
std::fmt::Display;
std::fmt::Debug;
std::fmt::Binary;
std::fmt::LowerHex;
std::fmt::UpperHex;
std::fmt::Octal;
std::fmt::Pointer;
```

As such, the following operatons are allowed for `PoxDigest`:

```rust
let mut default_digest = libpoxh::PoxDigest::default();
let mut digest_vec = vec![libpoxh::PoxDigest::default(); SIZE];
let digest_clone = default_digest.clone();
println!("Digests Debug:\n{:?}", digest_clone);
println!("Digests Display:\n{}", digest_clone);
println!("Bindgest:\n{:b}", digest_clone);
println!("Hexdigest in UPPERCASE:\n{:X}", digest_clone);
println!("Hexdigest in lowercase:\n{:x}", digest_clone);
println!("Octdigest:\n{:o}", digest_clone);
println!("Pointer to Original: {:p}", default_digest);
println!("Pointer to Clone: {:p}", digest_clone);
```

### Note on `type struct PoxDigest` in Go

This type has the predetermined string formatting function `Format` implemented. As such, for example, the following:

```go
fmt.Printf("%b", myPoxDigestInstance)
hexUpperPox = fmt.Sprintf("%X", myPoxDigestInstance)
```

Will print out `PoxDigest.Bindigest` and `PoxDigest.HexDiest` (in uppercase) repectively.

The following table indicates all the possible string formatting flags and their corresponding output:

| Flag | Output                  |
| ---- | ----------------------- |
| b    | Bindigest               |
| X    | Hexdigest, uppercase    |
| x    | Hexdigest, lowercase    |
| o    | Octdigest               |
| s    | Sendigest               |
| Y    | Vigdigest, uppercase    |
| y    | Vigdigest, lowercase    |
| G    | Sexdigest, uppercase    |
| g    | Sexdigest, lowercase    |
| U    | Tetdigest, uppercase    |
| u    | Tetdigest, lowercase    |
| d    | Duodigest               |
| B    | Bytees                  |
| W    | Words                   |
| D    | Doubles                 |
| Q    | Quad                    |
| F    | All decimal digests     |
| N    | All non-decimal digests |
| A    | Everything              |
| \*   | `EUF` unless predefiend |

### Note on `struct PoxDigest` in C

The type alias `poxdigest_t` is defined as:

```c
typedef poxdigest_t struct PoxDigest
```

This is the type that the functiion `pox_hash` returns, and it must be used at all times when dealing with PoxHash digests.

# Exported Symbols from `libpoxh.*` Files

Invariably, in all of the implementations, the only smybols exprted as the hasher function, and the structured type containing the digests. No other symbol has been exported from these files. The `libpoxh.h` file mostly relies on macros rather than functions, so there's little symbol save for the constants, all of which are `static` meaning they are private to the header file. In Go all the symbols start with lowercase letters, save for the two exported functions. JavaScript uses `module.exports` --- and Nim uses the `*` notation to export the two symbols. Rust uses `pub` to export the only two symbols defined within the module (other code are in submodules) and Python uses the `__` prefix before all variables, making them private to the file.

# Digests

I will now explain the different forms of digest contained within `PoxDigest` objects.

## Non-Decimal Digests

There are 8 non-decimal digests in the digest object. They are all created by the usual formula of converting a decimal number to undecenary bases, that is, determining a set of digits that must be equal to the ordinal value of the base. So for base 16, we ned 16 digits, for base 20, we need 20 and so on. Usually, people select the same Indo-Persian numerals for digits ranging from 0 to 9, and if the base is larger than 10, we choose an alphabet letter for the remainder. After we determine these digits we divide the decimal number by that base and choose the digit based on the remainder and we do this until the decimal is 0. We then reverse the sequence and that will be our number in that base.

For example, to convert decimal number `2` to binary we divide it by 2, getting 1 and our reminder will be `0`. So our first digit is 0. Now we divide the result of the previous division, 1, by 2 and we get 0 so we must stop. But befoe that we select the resi;t pf `1 mod 2` that is `1` as our second digit. We have `[0, 1]` but now we reverse it and we get `[1, 0]`. That is the binary value for 2.

**Note**: One in binary is 0b01 and two is 0b10. But in our calculations we can decide to go Big-Endian so they will be reversed. That is why as specs say, we always use Litttle-Endian in PoxHash.

We thusly use this formula to convert our 4 word factors to undecenary digests. The final digest must be paddd by the equivalent of digit 0 in the base, and the final size of the digest will be number of digits of 65535 (maximum unsigned word) in that base, tims 4. So, for example, 65535 in hexadecimal is 0xFFFF so we have 4 digits, if we pad each factored in hex with 0, then we end up with a string of length 16.

Let's go through properties of each of them.

### Digest Bases, Size and Total Size

Let's take a look at the base number of each undecenary digest, the number of digits of 65535 in it, and at the end the length of the final digest which as stated before is the aforementioned maximum digits times 4.

| PoxDigest Field | Base Num | Base Name    | 65535 Len | Total Len |
| --------------- | -------- | ------------ | --------- | --------- |
| sexdigest       | 60       | Sexagesimal  | 3         | 12        |
| vigdigest       | 20       | Vigesimal    | 4         | 16        |
| hexdigest       | 16       | Hexadecimal  | 4         | 16        |
| tetdigest       | 14       | Tetradecimal | 5         | 20        |
| duodigest       | 12       | Duodecimal   | 5         | 20        |
| octdigest       | 8        | Octal        | 6         | 24        |
| sendigest       | 6        | Senary       | 7         | 28        |
| bindigest       | 2        | Binary       | 16        | 64        |

It is not gauranteed that the 4 final unsigned word digests will translate to completely unique-looking non-binary digests for messages that are very similar. For example uint8 array [2, 129]'s hexdigest may look unique, but its sexdigest may not.

### Digits in PoxHash Standard

Each of these digests have digits that may or may not be exclusive to PoxHash standard specified in [SPEC.md](//SPEC.md). For Hexadecimal we use the usual notation, but for other larger-than-10 bases we use our own character-digit standard. They are as follows.

#### Binary

| n   | d   |
| --- | --- |
| 0   | '0' |
| 1   | '1' |

#### Senary

| n   | d   |
| --- | --- |
| 0   | '0' |
| 1   | '1' |
| 2   | '2' |
| 3   | '3' |
| 4   | '4  |
| 5   | '5' |

#### Octal

| n   | d   |
| --- | --- |
| 0   | '0' |
| 1   | '1' |
| 2   | '2' |
| 3   | '3' |
| 4   | '4  |
| 5   | '5' |
| 6   | '6' |
| 7   | '7' |

#### Duodecimal

| n   | d   | n   | d    | n   | d   |
| --- | --- | --- | ---- | --- | --- |
| 0   | '0' | 1   | '1'  | 2   | '2' |
| 3   | '3' | 4   | '4'  | 5   | '5' |
| 6   | '6' | 7   | '7'  | 8   | '8' |
| 9   | '9' | 10  | '\*' | 11  | '#' |

#### Tetradecimal

| n   | d   | n   | d   | n   | d   | n   | d   | n   | d   |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 0   | '0' | 1   | '1' | 2   | '2' | 3   | '3' | 4   | '4' |
| 7   | '7' | 8   | '8' | 9   | '9' | 10  | 'T' | 11  | 'E' |

#### Hexadecimal

| n   | d   | n   | d   | n   | d   | n   | d   |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 0   | '0' | 1   | '1' | 2   | '2' | 3   | '3' |
| 4   | '4' | 5   | '5' | 6   | '6' | 7   | '7' |
| 8   | '8' | 9   | '9' | 10  | 'A' | 11  | 'B' |
| 12  | 'C' | 13  | 'D' | 14  | 'E' | 15  | 'F' |

#### Vigesimal

| n   | d   | n   | d   | n   | d   | n   | d    | n   | d   |
| --- | --- | --- | --- | --- | --- | --- | ---- | --- | --- |
| 0   | 'A' | 1   | 'B' | 2   | 'C' | 3   | 'D'  | 4   | 'E' |
| 5   | 'F' | 6   | 'G' | 7   | 'H' | 8   | 'I'  | 9   | 'J' |
| 10  | '@' | 11  | '^' | 12  | '&' | 13  | '\*' | 14  | '$' |
| 15  | '+' | 16  | '!' | 17  | ';' | 18  | ':'  | 19  | '~' |

#### Sexagesimal

| n   | d   | n   | d   | n   | d   | n   | d   | n   | d   |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 0   | '0' | 1   | '1' | 2   | '2' | 3   | '3' | 4   | '4' |
| 5   | '5' | 6   | '6' | 7   | '7' | 8   | '8' | 9   | '9' |
| 10  | 'A' | 11  | 'B' | 12  | 'C' | 13  | 'D' | 14  | 'E' |
| 15  | 'F' | 16  | 'G' | 17  | 'H' | 18  | 'I' | 19  | 'J' |
| 20  | 'K' | 21  | 'L' | 22  | 'M' | 23  | 'N' | 24  | 'O' |
| 25  | 'P' | 26  | 'Q' | 27  | 'R' | 28  | 'S' | 29  | 'T' |
| 30  | 'U' | 31  | 'V' | 32  | 'W' | 33  | 'X' | 34  | 'Y' |
| 35  | 'Z' | 36  | 'a' | 37  | 'b' | 38  | 'c' | 39  | 'd' |
| 40  | 'e' | 41  | 'f' | 42  | 'g' | 43  | 'h' | 44  | 'i' |
| 45  | 'j' | 46  | 'k' | 47  | 'l' | 48  | 'm' | 49  | 'n' |
| 50  | 'o' | 51  | 'p' | 52  | 'q' | 53  | 'r' | 54  | 's' |
| 55  | 't' | 56  | 'u' | 57  | 'v' | 58  | 'w' | 59  | 'x' |

## Decimal Digests

The main decimal digest we have is the word digest, which are 4 unsigned 16-bit integers which the hashing operation has been carried on. The bytes are those 4 words decomposed into a uint8 array of size 8. Doubles are the reverse of that, every couple of uint16s have been merged into one uint32. Quad takes those 4 words and merges them into one uint64.

# Examples

Below we will provide examples for each language. Importinh has been ommitted, see [importing section](#importing) above for more info. The examples also ignore the execution context (function or top-level).

## C

```c
uint8_t message[] = "SmallPox";
poxdiest_t digest = pox_hash(message);
if (digest.hexdigest != "06BD634EC1FCA83A") {
    exit(1);
}
exit(0);
```

## Go

```go
message := []byte("SmallPox")
var digest PoxDigest = PoxHash([]uint8(message))
if digest.Hexdigest != "06BD634EC1FCA83A" {
    os.Exit(1)
}
os.Exit(0)
```

## JavaScript

```js
const utf8Encode = new TextEncoder();
const message = utf8Encode.encode("SmallPox");
const digest = poxHash(message);
if (digest.hexdigest != "06BD634EC1FCA83A") {
  process.exit(1);
}
process.exit(0);
```

## Nim

```nim
var
    message: seq[uint8] = map("SmallPox", proc(x: char): uint8 = cast[uint8](x))
    digest: PoxDigest

digest = PoxHash(message)

if digest.hexdigest != "06BD634EC1FCA83A":
    quit(1)
quit(0)
```

## Python

```python
from array import array

message = array('B', "SmallPox".encode())
digest = pox_hash(message)

if digest.hexdigest != "06BD634EC1FCA83A":
    exit(1)
exit(0)
```

## Rust

```rust
let message = String::from("SmallPox").as_bytes().to_vec();
let digest = pox_hash(&message);

if digest.hexdigest != "06BD634EC1FCA83A" {
    std::process::exit(1);
}
std::process::exit(0);
```

# Diving Deeper

- Specifications of PoxHash -> [SPEC.md](/SPEC.md)
- Using the example bash runners -> [RUNNERS.md](/RUNNERS.md)
- Rolling out your own implementation -> [CONTRIBUTE.md](/CONTRIBUTE.md)
