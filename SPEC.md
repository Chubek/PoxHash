```
/ Pseudo-Code for PoxHash Block Algorithm by Chubak Bidpaa; March 2023
/ Licensed Under GPLv3
/ NOTE: This document is best viewable with a monospace font

// PREFACE: CONVENTIONS
// In thise document we define the following as:

    /// Integer Operations
    a add b -> integer addition
    a sub b-> integer subtraction
    a idiv b -> integer integer division
    a mul b -> integer multiplication
    a mod b-> modulo
    log2 n -> Natural logarithm of the number
    
    /// Bitwise Operations
    a shl b -> shift bits left
    a shr b -> shift bits right
    a and b -> bitwie and
    a or b -> bitwise or
    a xor b -> bitwise xor

    /// Conditionals
    a gt b -> greater than
    a lt b -> lesser than
    if (condition) -> if conditional
    for n in (iterator) -> for loop
    a...b -> iterate from num a to num b
    a...b step n -> iterate from num a to num b but step by n

    /// Variable Operation
    assign-with -> Assign value to already existing, mutable variable
    instantiate-with -> Instantiate a new [mutable] variable
    ret -> return a variable from operation
    mutarg -> This argument is mutable 
    '->' -> This is an expression variable
    swap a with b -> swap variable a with variable b

    /// Literals
    is -> This means this is a constant literal

    /// Bit Units
    byte -> 8bits
    word -> 16bits
    double -> 32bits
    quad -> 64bits

    /// Integer Types
    i8 -> signed byte-sized integer
    u8 -> unsigned byte-sized integer
    u16 -> usigned word-sized integer
    u32 -> unsigned double-sized integer
    u64 -> unsigned quad-sized integer
    int -> default integer type
    '<type> -> instantiate-withantiate a new integer with this type
    as <type> -> conversion to

    /// Array Operations
    index -> The position of a value inside an array, we define as ra$ngi$ng from [0, +inf), indices are accessed by [] notation after an array name   
    max, argmax -> maximum value inside an array, and its correspondi$ng index
    min, argmin -> minimum value inside an array, and its correspondi$ng index
    range a, b -> Select a range of memebers from the array from indices a to b
    {ty, size} -> size and type

    /// Message Types
    char -> ASCII-encoded characters, for example 'char' in C or 'rune' in Go
            +---------------------------+
            | Ra$nge/Char | Ra$nge/Byte | 
            | '0'...'9'  |    48..57    |
            | 'A'...'Z'  |    65...90   | 
            | 'a'...'z'  |    97...122  |
            |    '*'     |      42      |
            |    '#'     |      35      |
            +---------------------------+
    bytearray -> A collection of i8 bytes, for example '[]bytes' in Go or 'bytearray' in Python

    /// Terminology
    message -> the byte array passed to the hasher
    digest -> the resulti$ng hash, which can be in various forms
    decimal-digests -> 
        bytes -> Each u16 factor divided in 2 u8s, a total of 8 bytes
        words -> The raw factors, all 4 of them
        doubles -> The 4 words put together into 2 doubles
        quad -> The 4 words put together into 1 quad
    non-decimal-digest ->
        sexdigest -> The 4 factors in sexadecimal or base 60
        hexdigest -> The 4 factors in hexadecimal or base 16
        duodigest -> The 4 factors in duoadecimal or base 12
        octdigest -> The 4 factors in octal or base 8
        bindigest -> The 4 factors in binary or base 2
    base-factors -> The array of size 4 containi$ng our 4 main word-size primes, of type u16
    temp-factors -> A temporary version of our factors
    input-raw -> The raw input sequence of variable size.
    input-padded -> The raw input converted to u16 and padded with z$eores until its size is divisible by 64
    block, block-size, block-array -> Block the hash operations on (which input-padded is divided to) and its size, which is 64
    $chunk, $chunk-size -> Chunk the block is divided to and its size, which is 16
    portion, portion-size, portion-array -> Portion the $chunk is divided to and its size, which is 4
    round-num -> Number of rounds each portion is applied to the base-factors
    round -> Applyi$ng scrambli$ng operations on base-factors    
    input-type -> The type of input to the hasher    
    bespoke-operations -> Vanilla numeric operations with custom overflow handli$ng
    bitwise-operations -> Basic bitwise operations
    alphabet-operations -> Scrambli$ng operations
    round-methods -> Methods for applyi$ng input to factors, and scrambli$ng the factors
    block-methods -> The main method that processes the block and applies the bytes
    byte-application -> Applyi$ng the portion to base-factors
    decimal-to-basen-conversion -> Converti$ng decimal base to base N
    word-to-byte-conversion -> Converti$ng 4 words to 8 bytes
    word-to-double0-conversion -> Converti$ng 4 words to 2 doubles
    word-to-quad-conversion -> Converti$ng 4 words to 1 quad
    endian-ness -> Whether the most significant bit of the integr is the leftmost bit, or the rightmost bit. As a simple example, let's take a look at '01'. In little-endian it's 1, but in big-endian it's 2. In PoxHash we treat everything as little-endian. 

// PART A: CONSTANTS
// In this document we will refer to several constant values, which are defined as below.

    /// PRIME ARRAYS
    /// WARNING: INDEX-SENSITIVE!
    []ROUND_PRIMES {u16, 32}[
        0xe537, 0xbd71, 0x9ef9, 0xbbcf, 0xf8dd, 0xceb7, 0xbaa1, 0x8f9f, 0xb0ed,
        0xfc4f, 0x9787, 0xf01f, 0xe1d1, 0xbcb9, 0xd565, 0xc011, 0xc1e1, 0xb58d,
        0xd4e1, 0x9ea1, 0xee49, 0x97cd, 0xdac9, 0xe257, 0xa32b, 0xafbb, 0xa5e3,
        0xfc43, 0xbf71, 0xe401, 0x8ebd, 0xd549
    ]
    []8B_PRIMES {u16, 54}[
        0x2, 0x3, 0x5, 0x7, 0xb, 0xd, 0x11, 0x13, 0x17, 0x1d, 0x1f, 0x25, 0x29,
        0x2b, 0x2f, 0x35, 0x3b, 0x3d, 0x43, 0x47, 0x49, 0x4f, 0x53, 0x59, 0x61,
        0x65, 0x67, 0x6b, 0x6d, 0x71, 0x7f, 0x83, 0x89, 0x8b, 0x95, 0x97, 0x9d,
        0xa3, 0xa7, 0xad, 0xb3, 0xb5, 0xbf, 0xc1, 0xc5, 0xc7, 0xd3, 0xdf, 0xe3,
        0xe5, 0xe9, 0xef, 0xf1, 0xfb
    ]
    []MAGIC_PRIMES {u16, 2}[
        0x33, 0x65
    ]
    []SINGLE_DIGIT_PRIMES {u16, 3}[
        0x3, 0x5, 0x7,
    ]

    /// INITIAL PRIME NUMBERS
    #PRIME_INIT_A is 0x9f91
    #PRIME_INIT_B is 0xdb3b
    #PRIME_INIT_C is 0xc091
    #PRIME_INIT_D is 0xac8b

    /// SIZE CONSTANTS
    #BLOCK_NUM is 64
    #8B_PRIME_NUM is 54
    #ROUND_PRIME_NUM is 32
    #CHUNK_NUM is 16
    #ROUND_NUM is 8
    #PORTION_NUM is 4
    ##SD_PRIME_NUM is 3
    #MAGIC_PRIME_NUM is 2

    /// BIT-RELATED CONSTANTS
    #WORD_WIDTH is 16
    #BYTE_WIDTH is 8
    #MAX_UINT16 is 65535

    /// MASKS
    xMASK_DWORD_4F4Z is 0xffff0000
    xMASK_DWORD_4Z4F is 0x0000ffff
    xMASK_WORD_FZFZ is 0xf0f0
    xMASK_WORD_ZFZF is 0x0f0f
    xMASK_WORD_FZZZ is 0xf000
    xMASK_WORD_ZZFZ is 0x00f0
    xMASK_WORD_ZZZF is 0x000f
    xMASK_WORD_ZZFF is 0x00ff
    xMASK_WORD_FFZZ is 0xff00
    xMASK_WORD_FZZF is 0xf00f
    xMASK_WORD_FFFZ is 0xfff0
    xMASK_WORD_ZFFF is 0x0fff
    xMASK_NIBBLET_01 is 0b01
    xMASK_NIBBLET_10 is 0b10
    xMASK_NIBBLET_11 is 0b11
    xMASK_NIBBLET_00 is 0b00

    /// MISC
    //// These are the result of bionomial coefficients of 4 to 2
    []COMB_BIONOM {(int, int), 32}[(0, 1), (0, 2), (0, 3), (1, 2), (1, 3), (2, 3)]
    #COMB_BIONOM_SIZE is 6

// PART B: BITWISE OPERATIONS
// We use several bitwise operations in our bitwise operations, as follows:

    @OMEGA(u32 $num) -> ret ($num and xMASK_DWORD_4F4Z) shr #WORD_WIDTH
    @EPSILON(u32 $num) -> ret $num and xMASK_DWORD_4Z4F
    /// Basically, bitwise rotation of 16bits
    @LAMED(u32 $num, $by) -> ret ($num shl $by) or ($num shr (#WORD_WIDTH sub $by))

// PART C: BESPOKE OPERATIONS
// These are bespoke operations for various run-off-the-mill numeric and bitwise operations

    /// rotate left with overflow in mind
    @GORDA(u16 $num, $by) ->
        $var instantiate-with $num as u32
        $var assign-with @LAMED($var, $by)
        $var assign-with @OMEGA($var) if $var gt #MAX_UINT16
        ret $var as u16

    /// add with overflow in mind
    @TASU( $a, $b) ->
        $vara, $varb instantiate-with $a and $b as u32
        $varc instantiate-with $vara add $varb
        $var assign-with @EPSILON($varc) if $varc gt #MAX_UINT16
        ret $varc as u16
    
    /// weighted average with overflow in mind
    @CENTUM(word-array $arr, $weights) ->
        $ctm instantiate-with 0'u32
        for i in 0...#PORTION_NUM: $ctm assign-with $ctm add ($arr[i] mul $weights[i])
        $ctm assign-with $ctm idiv #PORTION_NUM
        $ctm assign-with @OMEGA($ctm) if $ctm gt #MAX_UINT16
        ret $ctm as u16

    /// weighted median with overflow in mind
    @SATEM(word-array $arr, $weights) ->
        $stm instantiate-with 0'u32
        for i in 0...#PORTION_NUM: $stm assign-with $stm add ($arr[i] mul $weights[i])
        $stm assign-with ($stm add 1) idiv 2
        $stm = @EPSILON($stm) if $stm gt #MAX_UINT16
        ret $stm as u16

    /// normal average with overflow in mind
    @TAMAAM(word-array $arr) ->
        $tmt instantiate-with 0'u32
        for i in 0...#PORTION_NUM: $tmt assign-with $tmt add $arr[i]
        $tmt assign-with $tmt idiv #PORTION_NUM
        $tmt = @OMEGA($tmt) if $tmt gt #MAX_UINT16
        ret $tmt as u16

    /// normal median with overflow in mind
    @DECA(word-array $arr) ->
        $$dca instantiate-with 0'u32
        for i in 0...#PORTION_NUM: $$dca assign-with $$dca add $arr[i]
        $$dca assign-with ($$dca add 1) idiv 2
        $dca = @EPSILON($$dca) if $$dca gt #MAX_UINT16
        ret $$dca as u16

    /// getti$ng the 8-bit prime at index
    @GET-8B-PRIME(u16 $num) -> ret []8B_PRIEMS[$num mod #8B_PRIME_NUM]

// PART D: ALPHABET OPERATIONS
// These are the bread and butter operations of PoxHash. Implement them carefully!

    /// Operation ALPHA is focused on bitwise operations to scramble the message
    @ALPHA(mutarg temp-factors $tmp) ->
        $aleph instantiate-with ($tmp[0] xor $tmp[1]) and xMASK_WORD_ZZFF
        $daal instantiate-with ($tmp[2] xor $tmp[3]) and xMASK_WORD_FFZZ
        $theh instantiate-with (aleph or daal) mod []8B_PRIMES[0]
        $gaaf instantiate-with (aleph xor daal) mod []8B_PRIMES[1]

        $tmp[0] assign-with $tmp[0] shr $theh
        $tmp[1] assign-with $tmp[1] shr (($theh mod 2) add 1)
        $tmp[2] assign-with $tmp[2] shr gaaf

    /// Operation DELTA is also focused on bitwise ops, but in a more involved way plus a loop
    @DELTA(mutarg temp-factors $tmp) ->
        $alaf instantiate-with ($tmp[0] mod xMASK_WORD_FFFZ) mod GET-8B-PRIME($tmp[0])
        $dalat instantiate-with ($tmp[1] xor xMASK_WORD_FZZF) mod GET-8B-PRIME($tmp[1])
        $tit instantiate-with ($tmp[2] and xMASK_WORD_ZFFF) mod GET-8B-PRIME($tmp[2])
        $gaman instantiate-with ($tmp[3] and xMASK_WORD_FFZZ) mod GET-8B-PRIME($tmp[3])

        for #PORTION_NUM times loop:
            $alaf assign-with $alaf shr []SINGLE_DIGIT_PRIMES[$dalat mod #SD_PRIME_NUM]
            $dalat assign-with GORDA($dalat, 2)
            $tit assign-with $tit shr []SINGLE_DIGIT_PRIMES[$gaman mod #SD_PRIME_NUM]
            $gaman assign-with $gaman xor (($alaf ^ xMASK_WORD_ZZFF) shr []SINGLE_DIGIT_PRIMES[$tit mod #SD_PRIME_NUM])

        $tmp[1] assign-with $tmp[1] xor ($tmp[2] mod []MAGIC_PRIMES[$alaf mod #MAGIC_PRIME_NUM])
        $tmp[2] assign-with $tmp[2] xor ($alaf add $tit)
        $tmp[3] assign-with $tmp[3] xor ($tit add $gaman)
    
    /// Operation THETA leverages weighted operations to scamble the message
    @THETA(mutarg temp-factors $tmp) ->
        $alef instantiate-with $tmp[0] mod 2
        $dalet instantiate-with $tmp[1] mod 2
        $tet instantiate-with $tmp[2] mod 2
        $gimmel instantiate-with $tmp[3] mod 2

        $ctm instantiate-with CENTUM($tmp, word-array([$alef, $dalet, $tet, $gimmel]))
        $stm instantiate-with SATEM($tmp, word-array([$alef, $dalet, $tet, $gimmel]))

        $tmp[0] assign-with $tmp[0] xor ((($ctm shr $gimmel) xor xMASK_WORD_ZZFF) and xMASK_WORD_ZZZF)
        $tmp[3] assign-with $tmp[3 xor ((($stm shl $alef) xor xMASK_WORD_FZFZ) and xMASK_WORD_FZZZ)

    /// Operation GAMMA uses $argmin and $argmax to scramble the message
    @GAMMA(mutarg temp-factors $tmp) ->
        $argmin, $min instantiate-with argmin and min of $tmp
        $argmax, $max instantiate-with argmax and max of $tmp

        $ay instantiate-with $argmin and xMASK_NIBBLET_01
        $dee instantiate-with  $argmax xor xMASK_NIBBLET_10
        $thorn instantiate-with $argmin and xMASK_NIBBLET_11
        $gee instantiate-with $argmax xor xMASK_NIBBLET_00

        $alaph instantiate-with $tmp[$ay] mod GET-8B-PRIME($tmp[$thorn])
        $dalath instantiate-with (GET-8B-PRIME($max) xor xMASK_WORD_ZFZF) mod GET-8B-PRIME($min)
        $teth instantiate-with $max mod GET-8B-PRIME($max)
        $gamal instantiate-with $tmp[$dee] mod GET-8B-PRIME(($min add $max) idiv 2)

        $tmp[$ay] assign-with $tmp[$ay] xhr (($alaph xor xMASK_WORD_ZZFZ) mod WORD_WIDTH)
        $tmp[$dee] assign-with $tmp[$dee] shr (($gamal xor xMASK_WORD_FZZZ) mod (($max mod 2) add 1))
        $tmp[$thorn] assign-with $tmp[$thorn] xor (log2N($dalath) and xMASK_WORD_ZFFF)
        $tmp[$gee] assign-with  $tmp[$gee] xor (log2N($teth) shr (($gamal mod 2) add 1))

// PART E: ROUND METHODS
// In round methods we apply the ALPHABET OPERATIONS, shuffle around the factors, and apply the []ROUND_PRIMES. We do all these on temporary copy of the factors array, and then use the TASU method to add them up.

    /// Apply ALPBET OPERATIONS in order
    @APPLY_ALPHABET(mutarg temp-factors $tmp) ->
        @ALPHA($tmp)
        @DELTA($tmp)
        @THETA($tmp)
        @GAMMA($tmp)

    /// Apply the []ROUND_PRIMES array to temporary factors
    @APPLY_ROUND_PRIME(mutarg temp-factors $tmp) ->
        for i in 0...#ROUND_PRIME_NUM:
            $tmp[0] assign-with $tmp[0] xor []ROUND_PRIMES[i]
            $tmp[1] assign-with $tmp[1] and []ROUND_PRIMES[i]
            $tmp[2] assign-with $tmp[2] xor []ROUND_PRIMES[i]
            $tmp[3] assign-with $tmp[3] and []ROUND_PRIMES[i]

    /// Shuffle the array around a bit with C(4, 2) as indices
    @APPLY_SHUFFLE(mutarg temp-array $tmp) ->
        for i in 0...#COMB_BIONOM_SIZE: 
            $iof, $iwith assign-with expand []COMB_BIONOM[i]
            swap $tmp[$iof] with $tmp[$iwith]

    /// Add temporary factors to base-factors
    @APPLY_ADD_TEMPORARY(mutarg base-array $base, temp-array $tmp) ->
        for i in 0...#PORTION_NUM:
            $base[i] assign-with TASU($base[i], $tmp[i])

    /// The final roud operation
    @APPLY_ROUND(mutarg base-array $base) ->
        $tmp assign-with copy of $base
        @APPLY_ALPHABET($tmp)
        @APPLY_ROUND_PRIME($tmp)
        @APPLY_SHUFFLE($tmp)
        @APPLY_ADD_TEMPORARY($base, $tmp)

// PART F: BLOCK METHODS
// Below are the methods that happen at every block
    
    /// This operation applies the portion to the base factors at every round
    @APPLY_BYTES_TO_FACTORS(base-factors $base, portion-array $portion, word index) ->
        $tmt instantiate-with TAMAAM($portion)
        $dca instantiate-with DECA($portion)
        $tmtOddFactor instantiate-with #MAX_UINT16 mul ($tmt mod 2)
        $dcaOddFactor instantiate-with #MAX_UINT16 mul ($dca mod 2)

        $ng assign-with ($portion[0] add index) mod #PORTION_NUM)
        $chu assign-with ($portion[1] add index) mod #PORTION_NUM)
        $yo assign-with ($portion[2] add index) mod #PORTION_NUM)
        $eo assign-with ($portion[3] add index) mod #PORTION_NUM)

        $base[$ng] assign-with $base[$ng] xor (($portion[$eo] or $tmt) xor $dca_odd_factor)
        $base[$chu] assign-with $base[$chu] xor (($portion[$yo] and $dca) xor $tmt_odd_factor)
        $base[$yo] assign-with $base[$yo] xor (($portion[$chu] xor $tmt) xor $dca_odd_factor)
        $base[$eo] assign-with $base[$eo] xor (($portion[$ng] or $dca) xor $tmt_odd_factor)

    /// The main process block loop
    @PROCESS_BLOCK(base-factors $base, block-array $block) ->
        for i in 0...#BLOCK_NUM step #CHUNK_SIZE:
            for j in i...i + #CHUNK_SIZE step #PORTION_NUM:
                $portion instantiate-with $block range j, j + #PORTION_NUM
                for m in 0...#ROUND_NUM:
                    @APPLY_BYTES_TO_FACTORS($base, $portion)
                    @APPLY_ROUND($base)

// PART G: CONVERSION & PREPARATION PREP METHODS
// In this part we'll take a look at methods of converting from decimal to other bases, preparing the input, changing bit unit, and so on

    /// PART G.1: DATA PREPARATION
    
    /// CASTING
    /// Blocks must be in form of u16. Message has to be either passed as a string of bytes, an array of
    /// bytes, or both. Bytes can be either signed (i8) or unsigned (u8). PoxHash operates on u8s that are
    /// cast into u16s. So if your byte value is in signed form, you first need to cast it to unsigned
    /// byte, and then cast it into unsigned word. This especially comees into play with strings! As a
    /// rule of thumb, PoxHash's hasher algorithm must only accept numbers in form of u8, but in case of
    /// some languages, especially untyped languages, it will become rather difficult to enforce. So if you
    /// are also accepting strings as your input, make sure that they are encoded to 8-bit ASCII and not
    /// 7bit-ASCII, or just encode with Unicode. If you are in C, follow the formal implementations to
    /// convert i8 to u16. That's because C casts with 1s flipped on.
    /// So whereas '0b11100010' (-30'i8, 226'u8) is usually casted with leading bits flipped off, as in,
    /// '0b0000000011100010', in C they are flipped on! As in '0b1111111111100010'. So be careful with that.
    
    /// PADDING
    /// The resulting u'16 sequence must be padded until its size is divislbe by #BLOCK_SIZE. After you
    /// have gained that size using modulo operation, you have to pad it with 0s. Be careful that your
    /// data container is not smashing a loose pointer, has enough size, etc.

    /// PART G.2: WORD DIGEST CONVERSION METHODS
    /// The final 4 uint16 factors are the digest of PoxHash. They can be converted into as many 
    /// representations as the field of numerical studiges allows, or even beyond! But let's focus 
    /// on the representations that we have in the formal implementations. They are sexdigest, hexdigest, 
    /// duodigest, octdigest, bindgiest, bytes, doubles, quad and finally the words which are the factors 
    /// themselves. You can follow these basic rules to convert between them:

        //// Run once on all factors
        @WORD_TO_BYTE(u16 $word) ->
            $lowerByte, $upperByte instantiate-with 0'u8
            $lowerByte assign-with $word and xMASK_WORD_ZZFF
            $upperByte assign-with ($word and xMASK_WORD_FFZZ) shr #BYTE_WIDTH

            ret $lowerByte, $upperByte

        //// Run twice across all factors, once on 1 and 2, once on 3 and 4
        @WORDS_TO_DOUBLE(u16 $word1, $word2) ->
            $w1dbl, $w2dbl instantiate-with $word1, $word2 as  u32
            $dbl instantiate-with 0'u32
            
            $dbl assign-with $dbl or $w1dbl
            $dbl assign-with $db or ($w2dbl shl 16)

            ret $dbl

        //// Run twice on all factors
        @WORD_TO_QUAD(u16 $word1, $word2, $word3, $word4) ->
            $w1qd, $w2qd, $w3qd, $w4qd instantiate-with $word1, $word2, $word3, $word4 as u64
            $qd instantiate-with 0'u64
            
            $qd assign-with $qd or w1qd
            $qd assign-with $qd or ($w2qd shl 16)
            $qd assign-with $qd or ($w3qd shl 32)
            $qd assign-with $qd or ($w4qd shl 64)

            ret $qd

    /// PART G.3: NON-DECIMAL DIGESTS LETTER CONVENTION
    /// The process for converting the factors to non-decimal digests is impleemnted in all of
    /// the formal implementations so it does not seem there to be a need for repeating as it is 
    /// basic algebra. However we should mention the letter naming convention for sexdigest, hexdigest
    /// and duodigest as they may differ across different standards.

        //// SEXADECIMAL LETTERS 
            +----------------------------------------------------------+
            | n  |  c  || n  |  c  || n  |  c  || n  |  c  || n  |  c  |
            |====|=====||===|======|=====|=====|=====|=====|=====|=====|
            | 0  | '0' || 1  | '1' || 2  | '2' || 3  | '3' || 4  | '4' | 
            | 5  | '5' || 6  | '6' || 7  | '7' || 8  | '8' || 9  | '9' | 
            | 10 | 'A' || 11 | 'B' || 12 | 'C' || 13 | 'D' || 14 | 'E' | 
            | 15 | 'F' || 16 | 'G' || 17 | 'H' || 18 | 'I' || 19 | 'J' | 
            | 20 | 'K' || 21 | 'L' || 22 | 'M' || 23 | 'N' || 24 | 'O' | 
            | 25 | 'P' || 26 | 'Q' || 27 | 'R' || 28 | 'S' || 29 | 'T' | 
            | 30 | 'U' || 31 | 'V' || 32 | 'W' || 33 | 'X' || 34 | 'Y' | 
            | 35 | 'Z' || 36 | 'a' || 37 | 'b' || 38 | 'c' || 39 | 'd' | 
            | 40 | 'e' || 41 | 'f' || 42 | 'g' || 43 | 'h' || 44 | 'i' | 
            | 45 | 'j' || 46 | 'k' || 47 | 'l' || 48 | 'm' || 49 | 'n' | 
            | 50 | 'o' || 51 | 'p' || 52 | 'q' || 53 | 'r' || 54 | 's' | 
            | 55 | 't' || 56 | 'u' || 57 | 'v' || 58 | 'w' || 59 | 'x' | 
            +----------------------------------------------------------+

        //// HEXADECIMAL LETTERS
            +----------------------------------------------+
            | n  |  c  || n  |  c  || n  |  c  || n  |  c  |
            |====|=====||====|=====||====|=====||====|=====|
            | 0  | '0' || 1  | '1' || 2  | '2' || 3  | '3' | 
            | 4  | '4' || 5  | '5' || 6  | '6' || 7  | '7' | 
            | 8  | '8' || 9  | '9' || 10 | 'A' || 11 | 'B' | 
            | 12 | 'C' || 13 | 'D' || 14 | 'E' || 15 | 'F' |
            +----------------------------------------------+

        //// DUODECIMAL LETTERS
            +---------------------------------+
            | n |  c  || n  |  c  || n  |  c  |
            |===|=====||====|=====||====|=====|
            | 0 | '0' || 1  | '1' || 2  | '2' | 
            | 3 | '3' || 4  | '4' || 5  | '5' | 
            | 6 | '6' || 7  | '7' || 8  | '8' | 
            | 9 | '9' || 10 | '*' || 11 | '#' |
            +---------------------------------+ 
```