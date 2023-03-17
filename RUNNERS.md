# Guide for Runners and `poxh.sh`

## The Bash Files (`./poxh.sh`)

This repository contains 7 files named `poxh.sh`. 6 of them are inside implementation directories and the one that ties them together is in the top level folder.

The job of `poxh.sh` at the root folder is to accept a language name, alias or code as an argument and execute the corresponding `poxh.sh` file inside the implementation directories. The Shell scripts inside these directories, in turn either compile the application. They compile the application every time, keep that in mind --- the time it takes to run the commnand is not an indicator of the performance of the implementation. As for interpreted languages, the script just runs that file.

The argument syntax for the main `poxh.sh` at the top level directory is:

```
./poxh.sh <language name, alias or code> [silent?] <runner flags> [arguments passed to runner]
```

**Note**: if you run `./poxh.sh -h` it will print help.

For example:

```
./poxh.sh c -h- Masud Masvd
```

This command will call the C implementation, hash `Masud` and `Masvd` separately, and output the hexdigest --- as determined by the flag `h` between the two dashes. The result is:

```
+C
PoxHashRunner   |   Header-Only C   |  March 2023 - Chubak Bidpaa  |  GPLv3
Hexdigest: 46390453961EACD8
----
Hexdigest: 11FC08868E62B7AE
----
```

You may notice that the script echoes the name of our selected implementation. We can easily turn that off by passing an `-s` after our language argument:

```
./poxh.sh c -s -h- Masud Masvd
```

This script accepts the following arguments as language names, aliases or codese:

**Note**: all UPPERCASE lettrs in the language argument will be converted to lowercase so it does not matter if you passs the language name with different letter casing.

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

```bassh
bash ./poxh.sh nim -hD- SmallPox
```

We get:

```
PoxHashRunner   |   Nim    |  March 2023 - Chubak Bidpaa  |  GPLv3
Bytes: U8[189, 6, 78, 99, 252, 193, 58, 168]
Words: U16[1725, 25422, 49660, 43066]
Doubles: U32[1666057917, 2822423036]
Quad: U64[12122214636763088573]
Hexdigest: 06BD634EC1FCA83A
----
```

```bassh
bash ./poxh.sh nim -g*- SmallPox
```

We get:

```
You may not pass a non-decimal digest flag when `*` or `N` is passed
Error occurred. Please pass -?- to show help
```

```bassh
bash ./poxh.sh nim -gv^- SmallPox
```

We get:

```
Total time for hashing 1 unsigned bytearrays(s): 332us
Sexdigest: 0Sj73gDleBvk
Vigdigest: AEGFDD^CGEDAFH*G
----
```

Will print all decimal digests, plus hexadecimal.

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
Hexdigest: 1ECEA38E525A6D8D
----
+Rust
Hexdigest: 1ECEA38E525A6D8D
----
+C
Hexdigest: 1ECEA38E525A6D8D
----
+JavaScript
Hexdigest: 1ECEA38E525A6D8D
----
+Go
Hexdigest: 1ECEA38E525A6D8D
----
+Nim
Hexdigest: 1ECEA38E525A6D8D
----
```

## Testing the Universality

Let's test the universality of the file by swapping one character with the other.

This webpage: https://web.archive.org/web/19990429155017/http://www.fanfiction.net/text/main.cfm

Let's save the page intace:

```
wget -qO- https://web.archive.org/web/19990429155017/http://www.fanfiction.net/text/main.cfm > /tmp/pg1
```

String `navigationBAR.gif` appears only once in this file. Let's change it to `navigationABR.gif`

```
wget -qO- https://web.archive.org/web/19990429155017/http://www.fanfiction.net/text/main.cfm | tr 'navigationBAR.gif' 'navigationABR.gif'  > /tmp/pg2
```

Now:

```
for i in {1..6}; do ./poxh.sh $i -hz- file=/tmp/pg1 file=/tmp/pg2; done
```

We get:

```
+Python
Hexdigest: DCD082987D176E9F
----
Hexdigest: 9D3EA829202B936E
----
+Rust
Hexdigest: DCD082987D176E9F
----
Hexdigest: 9D3EA829202B936E
----
+C
Hexdigest: DCD082987D176E9F
----
Hexdigest: 9D3EA829202B936E
----
+JavaScript
Hexdigest: DCD082987D176E9F
----
Hexdigest: 9D3EA829202B936E
----
+Go
Hexdigest: DCD082987D176E9F
----
Hexdigest: 9D3EA829202B936E
----
+Nim
Hexdigest: DCD082987D176E9F
----
Hexdigest: 9D3EA829202B936E
----
```

## Benchmarking the Implementations

The runners provide us with tools to benchmark the time it takes for the hasher functions to scramble a message. So let's do that.

But it is rather unfair to compare Python and JS to statically typed, compiled languages. So we will do a grouped benchmark. One group is Python and JS, the other group is C, Rust, Go and Nim.

First, let's generate a large enough file for the first group, say, 2.5MB:

```
python3 -c "from string import ascii_lowercase;from random import choices;print(''.join(choices(ascii_lowercase, k=int(2.5e6))))" > /tmp/lrg
```

Now let's hash this file in statically-typed implementations:

```
for i in {1..6}; do ./poxh.sh $i -^^z- file=/tmp/lrg; done
```

We get:

```
+C
Total time for hashing 1 unsigned bytearrays(s): 12686060us

+Rust
Total time for hashing 1 unsigned bytearrays(s): 10346094us

+Go
Total time for hashing 1 unsigned bytearrays(s): 12508965us

+Nim
Total time for hashing 1 unsigned bytearrays(s): 59180us
```

Now let us put Python and JS to the test.

```
for i in j p; do ./poxh.sh $i -^^z- file=/tmp/lrg; done
```

We get:

```
+JavaScript
Total time for hashing 1 unsigned bytearrays(s): 28218256us

```

Now here is where I have to break the obvious news, Python is slow. So slow that I had to kill the process after 10 minutes! It's a lot of loops and the such and the so. Let's create a new, smaller file to test Python in tandeme with the other 5 implementations.

If we change the expontent in the `python -c` command that generates a file from 6 to 4 and pipe it to `/tmp/slrg`, according to `wc -c /tmp/slrg` we end up with `25001` bytes.

Let beunchmark it.

```
for i in c g r n j p; do ./poxh.sh $i -^^z- file=/tmp/slrg; done
```

We end up with:

```
+C
Total time for hashing 1 unsigned bytearrays(s): 127435us

+Go
Total time for hashing 1 unsigned bytearrays(s): 127397us

+Rust
Total time for hashing 1 unsigned bytearrays(s): 103081us

+Nim
Total time for hashing 1 unsigned bytearrays(s): 110692us

+JavaScript
Total time for hashing 1 unsigned bytearrays(s): 323601us

+Python
Total time for hashing 1 unsigned bytearrays(s): 7726792us
```

Python is clearly slower thna all of them combined. Now it may depend on the implementation of Python, be it Cython or CPython etc. It differs. I bet JIT will help a lot. Alass, Python is a great utility and scrpting language, but it's just not made for performance. I am surprised at ho fast JS is.

And finally, let's have every digest printed, in every implementation, for string `SmallPox`.

```
for i in c g r n j p; do ./poxh.sh $i -z*- SmallPox; done
```

We get:

```
+C
Bytes: U8[189, 6, 78, 99, 252, 193, 58, 168]
Words: U16[1725, 25422, 49660, 43066]
Doubles: U32[1666057917, 2822423036]
Quad: U64[12122214636763088573]
Sexdigest: 0Sj73gDleBvk
Vigdigest: AEGFDD^CGEDAFH*G
Hexdigest: 06BD634EC1FCA83A
Tetdigest: 008E30939W14152119T2
Duodigest: 00##912866248*420#0*
Octdigest: 003275061516140774124072
Sendgiest: 0011553031341010215240531214
Bindigest: 0000011010111101011000110100111011000001111111001010100000111010
----
+Go
Bytes: U8[189, 6, 78, 99, 252, 193, 58, 168]
Words: U16[1725, 25422, 49660, 43066]
Doubles: U32[1666057917, 2822423036]
Quad: U64[12122214636763088573]
Sexdigest: 0Sj73gDleBvk
Vigdigest: AEGFDD^CGEDAFH*G
Hexdigest: 06BD634EC1FCA83A
Tetdigest: 008E30939W14152119T2
Duodigest: 00##912866248*420#0*
Octdigest: 003275061516140774124072
Sendgiest: 0011553031341010215240531214
Bindigest: 0000011010111101011000110100111011000001111111001010100000111010
----
+Rust
Bytes: U8[189, 6, 78, 99, 252, 193, 58, 168]
Words: U16[1725, 25422, 49660, 43066]
Doubles: U32[1666057917, 2822423036]
Quad: U64[12122214636763088573]
Sexdigest: 0Sj73gDleBvk
Vigdigest: AEGFDD^CGEDAFH*G
Hexdigest: 06BD634EC1FCA83A
Tetdigest: 008E30939W14152119T2
Duodigest: 00##912866248*420#0*
Octdigest: 003275061516140774124072
Sendgiest: 0011553031341010215240531214
Bindigest: 0000011010111101011000110100111011000001111111001010100000111010
----
+Nim
Bytes: U8[189, 6, 78, 99, 252, 193, 58, 168]
Words: U16[1725, 25422, 49660, 43066]
Doubles: U32[1666057917, 2822423036]
Quad: U64[12122214636763088573]
Sexdigest: 0Sj73gDleBvk
Vigdigest: AEGFDD^CGEDAFH*G
Hexdigest: 06BD634EC1FCA83A
Tetdigest: 008E30939W14152119T2
Duodigest: 00##912866248*420#0*
Octdigest: 003275061516140774124072
Sendgiest: 0011553031341010215240531214
Bindigest: 0000011010111101011000110100111011000001111111001010100000111010
----
+JavaScript
Bytes: U8[189, 6, 78, 99, 252, 193, 58, 168]
Words: U16[1725, 25422, 49660, 43066]
Doubles: U32[1666057917, 2822423036]
Quad: U64[12122214636763088573]
Sexdigest: 0Sj73gDleBvk
Vigdigest: AEGFDD^CGEDAFH*G
Hexdigest: 06BD634EC1FCA83A
Tetdigest: 008E30939W14152119T2
Duodigest: 00##912866248*420#0*
Octdigest: 003275061516140774124072
Sendgiest: 0011553031341010215240531214
Bindigest: 0000011010111101011000110100111011000001111111001010100000111010
----
+Python
Bytes: U8[189, 6, 78, 99, 252, 193, 58, 168]
Words: U16[1725, 25422, 49660, 43066]
Doubles: U32[1666057917, 2822423036]
Quad: U64[12122214636763088573]
Sexdigest: 0Sj73gDleBvk
Vigdigest: AEGFDD^CGEDAFH*G
Hexdigest: 06BD634EC1FCA83A
Tetdigest: 008E30939W14152119T2
Duodigest: 00##912866248*420#0*
Octdigest: 003275061516140774124072
Sendgiest: 0011553031341010215240531214
Bindigest: 0000011010111101011000110100111011000001111111001010100000111010
----
```
