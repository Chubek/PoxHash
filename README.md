# PoxHash: A Block Hashing Algorithm

**Focuses on Universality and Irreversibility**

PoxHash is a block hashing algorithm created for performative and educational means and can be used for various tasks. It was originally developed for one of my active C| | projects as a checksum for HTTP body data. But I decided to take it out, polish it, make it more intricate, and implement it in C, Rust, Go, Nim, Python and JavaScript. Along with the implementations in every language, I have also written a runner code.

Length of the hash is 64 bits. It is comprised of 4 unsigned 16-bit integers. Size of each block is 64 as well. The size of hexadecimal digest of the hash is 16. Besides base 16, the implementations offer non-decimal digests in bases 60, 20, 14, 12, 8, 6 and 2. The 4 16-bit factors are also offered as 8 unsigned 8-bit integers, 2 unsigned 32-bit integers and a single unsigned 64-bit integer.

## Here, You Can...

- Learn how to make use of the `libpoxh.*` files -> [USAGE.md](/USAGE.md)
- Learn how to use the runners and `poxh.sh` script -> [RUNNERS.md](/RUNNERS.md)
- Learn more about the hashing algorithm in general -> [SPEC.md](/SPEC.md)
- Learn about ways to contribute to this project -> [CONTRIB.md](/CONTRIB.md)

## Demonstrations

### Universality

To demonstate the universality of PoxHash, look at the table below. The strings are encoded with UTF-8 and passed to the hasher function.

#### Shorter Messages

| Message | Hexdigest        | Message  | Hexdigest        |
| ------- | ---------------- | -------- | ---------------- |
| PoxHash | C28BDAFFCCD40526 | Felix    | 93D9E06FD6CFC2DA |
| QoxHash | FB441271DAED0056 | Feliz    | 8A11D66FC211E7BE |
| oPxHash | C7E81CB4D222EBE5 | Estella  | DA1DDD889834AC42 |
| Tanami  | 3641F6F8D849AA04 | Ertella  | 758ED778D01D50A4 |
| Tanammi | 99F13D64CE45153E | Lilly    | FC12E356CA142CED |
| Sanami  | E4062809A44AA672 | Likmy    | ADD6F7C17E101365 |
| 001298  | B6181CC3E2E5A0F1 | SmallPox | 06BD634EC1FCA83A |
| 001398  | 6FF92D0C82A6AFA2 | SmallPoy | 030156BFD808CB9C |

#### Some Longer Messages

First, let hash the static HTML page of example.com.

We get -> **5D485D326ADE5220**

Now, let's replace the string `<html>`, which only appears once on top, with `<htlm>`.

Now, we get -> **C6AE61C95D0C481F**

The RFC for HTTP 1.1 text file: https://www.ietf.org/rfc/rfc2616.txt

Intact -> **E8F8CCD71D80CEA4**

With 'must-revalidate' which appears only once change to 'must-ervalidate'

We get -> **E3C6E071187CD358**

With the same string changed to 'must-revalidbte':

We get -> **F3E8D234423EBE97**

#### Unsigned Byte Arrays

- [0x00, 0x00] -> **1474F230D6D1CDEC**
- [0x01, 0x00] -> **E4170016DBDE32E5**
- [0x00, 0x01] -> **E5B5FC44DCA9CF55**
- [0...255]    -> **810432FC3F471642**

### Irreversibility

Althoug I cannot guarantee that PoxHash is ever-unbreakable, what I know is, it relies on several serially accumulative operations to scramle the message. These operations apply average, median, product, sum (and the weighted counterparts of these) to the 4 word factors during the operations. Walking backwards from this without knowing the original message is a hard task. You can notice some patterns with the digest, but these patterns are fairly faint and untracable. I still refuse to make a promise.

If you manage to break the universality or irreversibility of PoxHash, please let me know so I can relase a stronger version.

### Speed

Speed is not th focus of PoxHash. It does some very heavy operations on the messag that requires time and resource expense. But the implementations in static languages are fast enough, with Nim being the fastest. Python is the slowest of them all.

In the table below you will see the time it takes to hash 25KBs of data, in microseconds (1e-6 of a second).

| Language   | Time Taken |
| ---------- | ---------- |
| C          | 127435us   |
| Go         | 127397us   |
| Rust       | 103081us   |
| Nim        | 110692us   |
| JavaScript | 323601us   |
| Python     | 7726792us  |

## Future Plans

I am currently done with this version of PoxHash. What I wanna do is to use the experience I earned from designing my first hash to come up with a faster, more universal hash.

I may add a Kotlin version very soon. Java is out of the question. Functional programming is impossible with Java and I want all the formal implementations to remain unifromly functional in paradigm.

I also wish to design PoxCipher. That is why the files are all called `libpoxh.*` and not just `libpox.*`, because I wish there to be `libpoxc.*` one day.

Another interest of mine are EC ciphers. Curve-based ciphers fascinate me. I have previously implemented Edwards-512 in my [mathcord](https://github.com/chbuek/mathcord) project and I wish to come up with one of myself.

Currently I am running out of money, and I want to work on a photo color manipulation utility which I will again implement cross-language. If you find this project useful, please consider a small [donation](https://github.com/Chubek/chubek/blob/master/README.md#donation-%E2%80%BF) my way. It will go a long way!

If you have any questions or requests, my contact details are inside my Github profile. Thank you.
