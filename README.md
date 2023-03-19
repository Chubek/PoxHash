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

#### Short String Messages

| Message | Hexdigest        |
| ------- | ---------------- |
| PoxHash | 07D04B8CD2E47BF3 |
| oPxHash | 8F7D20ECC51F3285 |
| PoxaHsh | 547CBDB0CB569320 |
| PxoHash | D92204E8D1C90376 |
| PoxHahs | 8D1DFF6A365C6E1A |
| QoxHash | 74B3D0533F14145B |
| PpxHash | E6DD8876150D0CBA |
| PoyHash | 337BA5F968A3927E |
| PoxIash | 61C8B88057481B42 |

#### Long String Messages

- Message A: HTML for `www.example.com`
- Message A': Message A, with <html>`changes to`<htlm>`
- Message B: contents of `https://www.ietf.org/rfc/rfc2616.txt`
- Message B': Message B, with `[Page 116]` changed to `[Page 161]`

| Message | Hexdigest        |
| ------- | ---------------- |
| A       | 53F0C77BD979E4C9 |
| A'      | DC1119514D34DD17 |
| B       | 43B9A61E5A9A5441 |
| B'      | 332BF0AEDC518AF1 |

#### Byte Arrays

- Message A; [0b00100010, 0b01100101, 0b10110101, 0b10110101, 0b01011101, 0b1111110, 0b1111101]
- Message B: [0b00100010, 0b01100100, 0b10110101, 0b10110101, 0b01011101, 0b1111110, 0b1111101]
- Message C: [0b00100010, 0b01100101, 0b10110101, 0b10110101, 0b01011101, 0b1101110, 0b1111101]
- Message D: [0b00100010, 0b01100101, 0b10110101, 0b10111101, 0b01011101, 0b1111110, 0b1111101]

| Message | Hexdigest        |
| ------- | ---------------- |
| A       | 41BA2FB4D6421610 |
| B       | 1568B6F5F5948EF6 |
| C       | 6293E59B2064CD28 |
| D       | 8D91A7BC753E223D |

### Irreversibility

Althoug I cannot guarantee that PoxHash is ever-unbreakable, what I know is, it relies on several serially accumulative operations to scramle the message. These operations apply average, median, product, sum (and the weighted counterparts of these) to the 4 word factors during the operations. Walking backwards from this without knowing the original message is a hard task. You can notice some patterns with the digest, but these patterns are fairly faint and untracable. I still refuse to make a promise.

If you manage to break the universality or irreversibility of PoxHash, please let me know so I can relase a stronger version.

### Speed

The following table shows the speed with which the implementations hash a message of 20KBs.

| Language | Nanos        | Micros        | Millis       | Secs        | Mins        |
| -------- | ------------ | ------------- | ------------ | ----------- | ----------- |
| C        | 1.5488e+06ns | 1.5488e+05us  | 1.5480e+02ms | 1.5488e-01s | 2.5810e-03m |
| Go       | 4.1634e+06ns | 4.1634e+05us  | 4.1630e+02ms | 4.1634e-01s | 6.9390e-03m |
| Rust     | 2.3192e+06ns | 2.3192e+05us  | 2.3190e+02ms | 2.3192e-01s | 3.8653e-03m |
| Nim      | 2.02476e+6ns | 2.02476e+05us | 2.0247e+2ms  | 2.0247e-01s | 3.3746e-03m |
| JS       | 3.4634e+06ns | 3.4634e+05us  | 3.4630e+02ms | 3.4634e-01s | 5.7724e-03m |
| Python   | 1.6592e+08ns | 1.6592e+07us  | 1.6592e+04ms | 1.6590e+01s | 2.7653e-01m |

## Future Plans

I wish to make Pox into a series of cryptographical tools and algorithms. I wish to make a PoxCipher, a PoxECC (Elliptic Curve Ciper), and my next target is PoxChain, a BlockChain implementation in Go using libp2p. If you find this project useful, please consider a small [donation](https://github.com/Chubek/chubek/blob/master/README.md#donation-%E2%80%BF) my way. It will go a long way!

If you have any questions or requests, my contact details are inside my Github profile. Thank you.
