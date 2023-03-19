# Guide for Runners and `poxh.sh`

## The Bash Files (`./poxh.sh`)

This repository contains 7 files named `poxh.sh`. 6 of them are inside implementation directories and the one that ties them together is in the top level folder.

The job of `poxh.sh` at the root folder is to accept a language name, alias or code as an argument and execute the corresponding `poxh.sh` file inside the implementation directories. The Shell scripts inside these directories. For JS and Python, the shell will just invoke the interpret. For for the static languages, the shell will first check if a compiled binary exists in `/tmp` folder inside a predestined path reserved for each language. If not, it will compile, and then execute the PoxHash Runner binary object file. You can also enforce recompilation, as we'll see.

The argument syntax for the main `poxh.sh` at the top level directory is:

```
./poxh.sh <language name, alias or code> [-s silent?] [-c compile?] <runner flags> [arguments passed to runner]
```

**Note**: if you run `./poxh.sh -h` it will print help.

For example:

```
./poxh.sh c -h- Masud Masvd
```

This command will call the C implementation, hash `Masud` and `Masvd` separately, and output the hexdigest --- as determined by the flag `h` between the two dashes. The result is:

```
+C
   PoxHashRunner   |   Header-Only C   |  March 2023 - Chubak Bidpaa  |  MIT
Hexdigest: A8EBEFCC7CBFA5B4
----
Hexdigest: B6486A8598B501F0
----
```

You may notice that the script echoes the name of our selected implementation. We can easily turn that off by passing an `-s` after our language argument:

```
./poxh.sh c -s -h- Masud Masvd
```

You can pass `-c` flag to force recompilation for static languages:

```
./poxh.sh nim -c -h- ABCD
```

This script accepts the following arguments as language names, aliases or codese:

**Note**: all UPPERCASE lettrs in the language alias will be converted to lowercase so it does not matter if you passs the language name with different letter casing.

| Language   | Main Arg   | Alias(es) | Code |
| ---------- | ---------- | --------- | ---- |
| C          | c          | c, cc     | 1    |
| Go         | go         | g         | 2    |
| Nim        | nim        | n         | 3    |
| Rust       | rust       | r, rs     | 4    |
| Python     | python     | p, py     | 5    |
| JavaScript | javascript | j, js     | 6    |

We can run the script in the following ways using the `for..loop` statement in Bash:

```
for i in {1..6}; do ./poxh.sh $i -s -h- Melvin Samuel
for ii in {1..6..2}; do ./poxh.sh $i -s -h- Melvin Samuel
for l in c g p; do ./poxh.sh $l -s -h- Melvin Samuel
for ll in cc rs; do ./poxh.sh $ll -s -h- Melvin Samuel
```

## Runner Codes

The runners act identical across all implementations. They all accept the same flags and show the same results.

Each local `poxh.sh` script relegating to a statically-typed language first compiles (or recompiles) the code before running it. The code is very light in gneral and compiling it should not take much time. These files are compiled insiide the `/tmp` folder and will be gone when you restart your computer.

### Flags

Runners accept a set of flag characters between two dashes. Each flag, save for one, is allowed to only appear once. Some flags may not be passed when other flags are passed.

You can pass `-?-` to view these flags. We will list them here.

| Flag Character | Function                       |
| -------------- | ------------------------------ |
| `e`            | Echo argument                  |
| `z`            | Don't print the header message |
| `^`            | Show the total time benchmark  |
| `+`            | Join the arguments with space  |
| `*`            | Print all digests              |
| `N`            | Print every non-decimal digest |
| `D`            | Print every decimal digest     |
| `8`            | Print bytes digest             |
| `4`            | Print words digest             |
| `2`            | Print doubles digest           |
| `1`            | Print quad digest              |
| `g`            | Print sexagesimal digest       |
| `v`            | Print vigesimal digest         |
| `h`            | Print hexadecimal digest       |
| `t`            | Print tetradecimal digest      |
| `d`            | Print duodecimal digest        |
| `o`            | Print octal digest             |
| `s`            | Print senary digest            |
| `b`            | Print binary digest            |
| `9`            | Print time delta in nanosecs   |
| `6`            | Print time delta in microsecs  |
| `3`            | Print time delta in millisecs  |
| `5`            | Print time delta in seconds    |
| `0`            | Print time delta in minutes    |
| `?`            | Print Help                     |

These rules apply to passing args:

- Any other digest flag may not be passed along with `*`
- Any Non-Decimal digest flag may not be passed along wih `*` and `N`
- Any decimal digest flag may not be passed along with `*` and `D`
- Neither both, nor either `N` and `E` may be passed along with `*`
- If two `^`s are passed, it will just print the benchmark time
- `?` cannot be passed along with any othe flag
- Dash, `-`, may not appear more than twice, once at the beginning, once at th eend
- Minimum length of the first argument must be 3 and maximum, 24
- No flag except `^` may appear twice
- `^` may not appeare more than twice

For example:

```
./poxh.sh nim -s -hDz- SmallPox
```

We get:

```
Bytes: U8[177, 142, 70, 24, 124, 79, 242, 172]
Words: U16[36529, 6214, 20348, 44274]
Doubles: U32[407277233, 2901561212]
Quad: U64[12462110513289399985]
Hexdigest: 8EB118464F7CACF2
----
```

```
bash ./poxh.sh p -g*- SmallPox
```

We get:

```
+Python

You may not pass a non-decimal digest flag when `*` or `N` is passed
Error occurred. Please pass -?- to show help
```

**Note**: Errors are written to `stderr`.

When we pass `^`, the benchmark flag, we can either pass one of the timestamp flags, or it will just print microseconds.

```
bash ./poxh.sh nim -gvz^- SmallPox
```

We get:

```
+Nim

| 1 Message(s) || 2.8651e+03us |
Sexdigest: A8n1hY5d8CHs
Vigdigest: E^GJA+@$C@;IF@*$
----
```

Or we can pass another timestamp flag:

```
bash ./poxh.sh nim -gvz30^- SmallPox
```

We get:

```
+Nim

| 1 Message(s) || 2.6528e+0ms | 4.2137e-01m |
Sexdigest: A8n1hY5d8CHs
Vigdigest: E^GJA+@$C@;IF@*$
----
```

If we pass two `^`s, we will get all the timestamps. We may not pass any other timestamp along, and also, it will exit and not print any digests.

```
./poxh.sh p -s -^^z- Cholera Tifus
```

We just get this:

```
| 2 Message(s) || 1.0783e+06ns | 1.0783e+05us | 1.0780e+02ms | 1.0783e-01s | 1.7972e-03m |
```

With this:

```
./poxh.sh j -s -^^3z- Cholera Tifus
```

We get:

```
When double benchmark (`^^`) is passed, you may not pass a timestamp flag
Error occurred. Please pass -?- to show help
```

This will pint sexgesimal and vigesimal digests.

### Hashing Files

You can easily hash files by passing them as arguments. Except you need to prefix that argument with `file=`. This prefix is ignored if `+` is passed. You can pass as many arguments with that prefix, and you can mix it in aalong with other arguments that don't have the notation.

To demonstrate, let's donload a file first:

```
wget https://www.ietf.org/rfc/rfc2616.txt -qO /tmp/rfc2616.txt
```

We then run:

```
for i in {1..6}; do ./poxh.sh $i -hz- filepath=/tmp/rfc2616.txt; done
```

We get:

```
+Python
+C
Hexdigest: 16FC00E867E82D47
----
+Go
Hexdigest: 16FC00E867E82D47
----
+Nim
Hexdigest: 16FC00E867E82D47
----
+Rust
Hexdigest: 16FC00E867E82D47
----
+Python
Hexdigest: 16FC00E867E82D47
----
+JavaScript
Hexdigest: 16FC00E867E82D47
----
----
```

### Hashing Integers

If the prefix `int=` is applied to a flag, it will then parse the comma-separated numbers in that argument with taking prefices `0b`, `0o`, and `0x` for binary, octal nd hexadecimal, respectively. No base prefix means decimal integer. Let's see.

```
for l in c g r n j p; do ./poxh.sh $l -hz- int=0b1001,0o123,0x1F,142,0b01 int=12,13,0b01; done
```

We get:

```
+C
Hexdigest: 90ADF79ACB0790F1
----
Hexdigest: 5F048F586D954E11
----
+Go
Hexdigest: 90ADF79ACB0790F1
----
Hexdigest: 5F048F586D954E11
----
+Rust
Hexdigest: 90ADF79ACB0790F1
----
Hexdigest: 5F048F586D954E11
----
+Nim
Hexdigest: 90ADF79ACB0790F1
----
Hexdigest: 5F048F586D954E11
----
+JavaScript
Hexdigest: 90ADF79ACB0790F1
----
Hexdigest: 5F048F586D954E11
----
+Python
Hexdigest: 90ADF79ACB0790F1
----
Hexdigest: 5F048F586D954E11
----


```

## Testing the Avalanche Effect

Avalanche effect of a hash happens when one bit is flipped, and the whole digest changes. Let's see how PoxHash fairs at this.

This webpage: https://web.archive.org/web/19990429155017/http://www.fanfiction.net/text/main.cfm

Let's save the page intact:

```
wget -qO- https://web.archive.org/web/19990429155017/http://www.fanfiction.net/text/main.cfm > /tmp/pg1
```

String `navigationBAR.gif` appears only once in this file. Let's change it to `navigationABR.gif`

```
wget -qO- https://web.archive.org/web/19990429155017/http://www.fanfiction.net/text/main.cfm | tr 'navigationBAR.gif' 'navigationABR.gif'  > /tmp/pg2
```

Now:

```
for l in j c n; do ./poxh.sh $l -hz- file=/tmp/pg1 file=/tmp/pg2; done
```

We get:

```
+JavaScript

Hexdigest: 56122442BB6C4CB8
----
Hexdigest: 3FB5B2E693A9B936
----
+C

Hexdigest: 56122442BB6C4CB8
----
Hexdigest: 3FB5B2E693A9B936
----
+Nim

Hexdigest: 56122442BB6C4CB8
----
Hexdigest: 3FB5B2E693A9B936
----

```

## Benchmarking the Implementations

Let's create a small-but-decently sized file, with random bytes. We set the size at 20KB. With it we are going to benchmark the implementations.

First, let's generate the bytes:

```
python3 -c "from random import choices;from string import printable;print(''.join(choices(printable, k=20000)))" > /tmp/20kb
```

Let's check the size of the file in bytes:

```
wc -c /tmp/20kb
```

We get:

```
20001 /tmp/20kb
```

Now let's hash it:

```
for l in c g r n j p; do ./poxh.sh $l -z^^- file=/tmp/20kb; done
```

```
+C

| 1 Message(s) || 1.5488e+06ns | 1.5488e+05us | 1.5480e+02ms | 1.5488e-01s | 2.5810e-03m |

+Go

| 1 Message(s) || 4.1634e+06ns | 4.1634e+05us | 4.1630e+02ms | 4.1634e-01s | 6.9390e-03m |

+Rust

| 1 Message(s) || 2.3192e+06ns | 2.3192e+05us | 2.3190e+02ms | 2.3192e-01s | 3.8653e-03m |

+Nim

| 1 Message(s) || 2.02476e+6ns | 2.02476e+05us | 2.0247e+2ms | 2.0247e-01s | 3.3746e-03m |

+JavaScript

| 1 Message(s) || 3.4634e+06ns | 3.4634e+05us | 3.4630e+02ms | 3.4634e-01s | 5.7724e-03m |

+Python

| 1 Message(s) || 1.6592e+08ns | 1.6592e+07us | 1.6592e+04ms | 1.6590e+01s | 2.7653e-01m |

```

## A Final Look

Finally, let's print all digests all at once for every implementation for two messages: `PoxHash` and `PoHxash`

```
for l in c g r n j p; do ./poxh.sh $l -*z- PoxHash PoHxash; done
```

We get:

```
+C

Bytes: U8[208, 7, 140, 75, 228, 210, 243, 123]
Words: U16[2000, 19340, 53988, 31731]
Doubles: U32[1267468240, 2079576804]
Quad: U64[8931714363967670224]
Sexdigest: 0XK5MKExm8mp
Vigdigest: AFAACIHAG$~ID~G^
Hexdigest: 07D04B8CD2E47BF3
Tetdigest: 00T2W07096159640E7W7
Duodigest: 011*80#238272#016443
Octdigest: 003720045614151344075763
Sendgiest: 0013132022531210535400402523
Bindigest: 0000011111010000010010111000110011010010111001000111101111110011
----
Bytes: U8[247, 15, 107, 190, 189, 227, 79, 159]
Words: U16[4087, 48747, 58301, 40783]
Doubles: U32[3194687479, 2672812989]
Quad: U64[11479644379273695223]
Sexdigest: 187DWRGBfBJh
Vigdigest: A@EHGB;HHF+BFB~D
Hexdigest: 0FF7BE6BE3BD9F4F
Tetdigest: 016ER13T9R1736510W11
Duodigest: 0244724263298*51#727
Octdigest: 007767137153161675117517
Sendgiest: 0030531101340311255250512451
Bindigest: 0000111111110111101111100110101111100011101111011001111101001111
----
+Go

Bytes: U8[208, 7, 140, 75, 228, 210, 243, 123]
Words: U16[2000, 19340, 53988, 31731]
Doubles: U32[1267468240, 2079576804]
Quad: U64[8931714363967670224]
Sexdigest: 0XK5MKExm8mp
Vigdigest: AFAACIHAG$~ID~G^
Hexdigest: 07D04B8CD2E47BF3
Tetdigest: 00T2W07096159640E7W7
Duodigest: 011*80#238272#016443
Octdigest: 003720045614151344075763
Sendgiest: 0013132022531210535400402523
Bindigest: 0000011111010000010010111000110011010010111001000111101111110011
----
Bytes: U8[247, 15, 107, 190, 189, 227, 79, 159]
Words: U16[4087, 48747, 58301, 40783]
Doubles: U32[3194687479, 2672812989]
Quad: U64[11479644379273695223]
Sexdigest: 187DWRGBfBJh
Vigdigest: A@EHGB;HHF+BFB~D
Hexdigest: 0FF7BE6BE3BD9F4F
Tetdigest: 016ER13T9R1736510W11
Duodigest: 0244724263298*51#727
Octdigest: 007767137153161675117517
Sendgiest: 0030531101340311255250512451
Bindigest: 0000111111110111101111100110101111100011101111011001111101001111
----
+Rust

Bytes: U8[208, 7, 140, 75, 228, 210, 243, 123]
Words: U16[2000, 19340, 53988, 31731]
Doubles: U32[1267468240, 2079576804]
Quad: U64[8931714363967670224]
Sexdigest: 0XK5MKExm8mp
Vigdigest: AFAACIHAG$~ID~G^
Hexdigest: 07D04B8CD2E47BF3
Tetdigest: 00T2W07096159640E7W7
Duodigest: 011*80#238272#016443
Octdigest: 003720045614151344075763
Sendgiest: 0013132022531210535400402523
Bindigest: 0000011111010000010010111000110011010010111001000111101111110011
----
Bytes: U8[247, 15, 107, 190, 189, 227, 79, 159]
Words: U16[4087, 48747, 58301, 40783]
Doubles: U32[3194687479, 2672812989]
Quad: U64[11479644379273695223]
Sexdigest: 187DWRGBfBJh
Vigdigest: A@EHGB;HHF+BFB~D
Hexdigest: 0FF7BE6BE3BD9F4F
Tetdigest: 016ER13T9R1736510W11
Duodigest: 0244724263298*51#727
Octdigest: 007767137153161675117517
Sendgiest: 0030531101340311255250512451
Bindigest: 0000111111110111101111100110101111100011101111011001111101001111
----
+Nim

Bytes: U8[208, 7, 140, 75, 228, 210, 243, 123]
Words: U16[2000, 19340, 53988, 31731]
Doubles: U32[1267468240, 2079576804]
Quad: U64[8931714363967670224]
Sexdigest: 0XK5MKExm8mp
Vigdigest: AFAACIHAG$~ID~G^
Hexdigest: 07D04B8CD2E47BF3
Tetdigest: 00T2W07096159640E7W7
Duodigest: 011*80#238272#016443
Octdigest: 003720045614151344075763
Sendgiest: 0013132022531210535400402523
Bindigest: 0000011111010000010010111000110011010010111001000111101111110011
----
Bytes: U8[247, 15, 107, 190, 189, 227, 79, 159]
Words: U16[4087, 48747, 58301, 40783]
Doubles: U32[3194687479, 2672812989]
Quad: U64[11479644379273695223]
Sexdigest: 187DWRGBfBJh
Vigdigest: A@EHGB;HHF+BFB~D
Hexdigest: 0FF7BE6BE3BD9F4F
Tetdigest: 016ER13T9R1736510W11
Duodigest: 0244724263298*51#727
Octdigest: 007767137153161675117517
Sendgiest: 0030531101340311255250512451
Bindigest: 0000111111110111101111100110101111100011101111011001111101001111
----
+JavaScript

Bytes: U8[208, 7, 140, 75, 228, 210, 243, 123]
Words: U16[2000, 19340, 53988, 31731]
Doubles: U32[1267468240, 2079576804]
Quad: U64[8931714363967670224]
Sexdigest: 0XK5MKExm8mp
Vigdigest: AFAACIHAG$~ID~G^
Hexdigest: 07D04B8CD2E47BF3
Tetdigest: 00T2W07096159640E7W7
Duodigest: 011*80#238272#016443
Octdigest: 003720045614151344075763
Sendgiest: 0013132022531210535400402523
Bindigest: 0000011111010000010010111000110011010010111001000111101111110011
----
Bytes: U8[247, 15, 107, 190, 189, 227, 79, 159]
Words: U16[4087, 48747, 58301, 40783]
Doubles: U32[3194687479, 2672812989]
Quad: U64[11479644379273695223]
Sexdigest: 187DWRGBfBJh
Vigdigest: A@EHGB;HHF+BFB~D
Hexdigest: 0FF7BE6BE3BD9F4F
Tetdigest: 016ER13T9R1736510W11
Duodigest: 0244724263298*51#727
Octdigest: 007767137153161675117517
Sendgiest: 0030531101340311255250512451
Bindigest: 0000111111110111101111100110101111100011101111011001111101001111
----
+Python

Bytes: U8[208, 7, 140, 75, 228, 210, 243, 123]
Words: U16[2000, 19340, 53988, 31731]
Doubles: U32[1267468240, 2079576804]
Quad: U64[8931714363967670224]
Sexdigest: 0XK5MKExm8mp
Vigdigest: AFAACIHAG$~ID~G^
Hexdigest: 07D04B8CD2E47BF3
Tetdigest: 00T2W07096159640E7W7
Duodigest: 011*80#238272#016443
Octdigest: 003720045614151344075763
Sendgiest: 0013132022531210535400402523
Bindigest: 0000011111010000010010111000110011010010111001000111101111110011
----
Bytes: U8[247, 15, 107, 190, 189, 227, 79, 159]
Words: U16[4087, 48747, 58301, 40783]
Doubles: U32[3194687479, 2672812989]
Quad: U64[11479644379273695223]
Sexdigest: 187DWRGBfBJh
Vigdigest: A@EHGB;HHF+BFB~D
Hexdigest: 0FF7BE6BE3BD9F4F
Tetdigest: 016ER13T9R1736510W11
Duodigest: 0244724263298*51#727
Octdigest: 007767137153161675117517
Sendgiest: 0030531101340311255250512451
Bindigest: 0000111111110111101111100110101111100011101111011001111101001111
----
```
