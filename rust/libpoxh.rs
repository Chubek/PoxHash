/////////////////////////////////////////////////////////////////////////////////////
//                       PoxHash implementation in Rust                            //
//                          March 2023 - Chubak Bidpaa                             //
/////////////////////////////////////////////////////////////////////////////////////
// MIT License                                                                     //
//                                                                                 //
// Copyright (c) 2023 Chubak Bidpaa                                                //
//                                                                                 //
// Permission is hereby granted, free of charge, to any person obtaining a copy    //
// of this software and associated documentation files (the "Software"), to deal   //
// in the Software without restriction, including without limitation the rights    //
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       //
// copies of the Software, and to permit persons to whom the Software is           //
// furnished to do so, subject to the following conditions:                        //
//                                                                                 //
// The above copyright notice and this permission notice shall be included in all  //
// copies or substantial portions of the Software.                                 //
//                                                                                 //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     //
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   //
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   //
// SOFTWARE.                                                                       //
/////////////////////////////////////////////////////////////////////////////////////

mod consts {

    // https://github.com/Chubek/PoxHash/blob/master/SPEC.md#initial-prime-numbers
    pub mod initial_primes {
        pub const PRIME_INIT_A: u16 = 0x17cb;
        pub const PRIME_INIT_B: u16 = 0x0371;
        pub const PRIME_INIT_C: u16 = 0x2419;
        pub const PRIME_INIT_D: u16 = 0xf223;
    }

    // https://github.com/Chubek/PoxHash/blob/master/SPEC.md#size-constants
    pub mod size_values {
        pub const ROUND_PRIME_NUM: usize = 90;
        pub const BLOCK_NUM: usize = 64;
        pub const BYTE_PRIME_NUM: usize = 54;
        pub const ROUND_NUM: usize = 31;
        pub const CHUNK_NUM: usize = 16;
        pub const PORTION_NUM: usize = 4;
        pub const MASKS_ARRAY_NUM: usize = 4;
        pub const SD_PRIME_NUM: usize = 3;
        pub const MAGIC_PRIME_NUM: usize = 2;
    }

    // https://github.com/Chubek/PoxHash/blob/master/SPEC.md#masks
    pub mod masks {
        pub const MASK_QWORD_14Z2F: usize = 0x00000000000000ff;
        pub const DWORD_4F4Z: u32 = 0xffff0000;
        pub const DWORD_4Z4F: u32 = 0x0000ffff;
        pub const WORD_FZFZ: u16 = 0xf0f0;
        pub const WORD_ZFZF: u16 = 0x0f0f;
        pub const WORD_FZZZ: u16 = 0xf000;
        pub const WORD_ZZFZ: u16 = 0x00f0;
        pub const WORD_ZZZF: u16 = 0x000f;
        pub const WORD_ZZFF: u16 = 0x00ff;
        pub const WORD_FFZZ: u16 = 0xff00;
        pub const WORD_FZZF: u16 = 0xf00f;
        pub const WORD_FFFZ: u16 = 0xfff0;
        pub const WORD_ZFFF: u16 = 0x0fff;
        pub const NIBBLET_01: usize = 0b01;
        pub const NIBBLET_10: usize = 0b10;
        pub const NIBBLET_11: usize = 0b11;
        pub const NIBBLET_00: usize = 0b00;
        pub const MASKS_ARRAY: &'static [u16] = &[WORD_FFZZ, WORD_ZFFF, WORD_FFFZ, WORD_ZZFF];
    }

    // https://github.com/Chubek/PoxHash/blob/master/SPEC.md#prime-arrays
    pub mod prime_arrays {
        pub const ROUND_PRIMES: [u16; super::size_values::ROUND_PRIME_NUM] = [
            0x0377, 0x0683, 0x05fb, 0x05fb, 0x0665, 0x074b, 0x036d, 0x033d, 0x0115, 0x07cf, 0x0e59,
            0x0e75, 0x0a75, 0x119b, 0x1073, 0x12b3, 0x0fd1, 0x0a75, 0x0de7, 0x10bb, 0x18d1, 0x1c99,
            0x1723, 0x1cc9, 0x20c3, 0x2327, 0x2063, 0x215b, 0x17e1, 0x22bd, 0xf2ff, 0xf50b, 0xf4af,
            0xf2b3, 0xf5fb, 0xf4af, 0xf2b9, 0xf38b, 0xf4c3, 0xf5db, 0x1039, 0x1003, 0x0fa1, 0x0fa3,
            0x0fa7, 0x8687, 0x80db, 0x86d1, 0x7fcd, 0x7f43, 0xa10b, 0x9e81, 0x9d15, 0xa289, 0xa279,
            0x3e11, 0x3aa5, 0x3be3, 0x3daf, 0x3bff, 0xff8f, 0xff71, 0xfe03, 0xfe41, 0xfe05, 0xff2f,
            0xfe7b, 0xfeb3, 0x0409, 0x0481, 0x1d7b, 0x1c4f, 0x1e6d, 0x1b7f, 0x1e71, 0xe875, 0xe2cd,
            0xe351, 0xe363, 0xe329, 0x049d, 0x0427, 0xcbb3, 0x184d, 0x2ce1, 0x8861, 0x59b3, 0x2077,
            0xff9d, 0xff2f,
        ];
        pub const BYTE_PRIMES: [u16; super::size_values::BYTE_PRIME_NUM] = [
            0x2, 0x3, 0x5, 0x7, 0xb, 0xd, 0x11, 0x13, 0x17, 0x1d, 0x1f, 0x25, 0x29, 0x2b, 0x2f,
            0x35, 0x3b, 0x3d, 0x43, 0x47, 0x49, 0x4f, 0x53, 0x59, 0x61, 0x65, 0x67, 0x6b, 0x6d,
            0x71, 0x7f, 0x83, 0x89, 0x8b, 0x95, 0x97, 0x9d, 0xa3, 0xa7, 0xad, 0xb3, 0xb5, 0xbf,
            0xc1, 0xc5, 0xc7, 0xd3, 0xdf, 0xe3, 0xe5, 0xe9, 0xef, 0xf1, 0xfb,
        ];
        pub const SINGLE_DIGIT_PRIMES: [u16; super::size_values::SD_PRIME_NUM] = [0x3, 0x5, 0x7];
        pub const MAGIC_PRIMES: [u16; super::size_values::MAGIC_PRIME_NUM] = [0x33, 0x65];
    }

    // https://github.com/Chubek/PoxHash/blob/master/SPEC.md#bit-related-constants
    pub mod bit_values {
        pub const UINT16_MAX_U32: u32 = 65535;
        pub const UINT16_MAX_U16: u16 = 65535;
        pub const WORD_WIDTH_U32: u32 = 16;
        pub const WORD_WIDTH_U16: u16 = 16;
        pub const BYTE_WIDTH_U16: u16 = 8;
    }

    // https://github.com/Chubek/PoxHash/blob/master/SPEC.md#misc
    pub mod misc {
        pub const COMB_BIONOM: &'static [(usize, usize)] =
            &[(0, 1), (0, 2), (0, 3), (1, 2), (1, 3), (2, 3)];
        pub const SIZE_BIONOM: usize = 6;
    }

    pub mod conversion {
        pub const SEX_SIZE: usize = 3;
        pub const VIG_SIZE: usize = 4;
        pub const HEX_SIZE: usize = 4;
        pub const TET_SIZE: usize = 5;
        pub const DUO_SIZE: usize = 5;
        pub const OCT_SIZE: usize = 6;
        pub const SEN_SIZE: usize = 7;
        pub const BIN_SIZE: usize = 16;
        pub const SEX_BASE: u16 = 60;
        pub const VIG_BASE: u16 = 20;
        pub const HEX_BASE: u16 = 16;
        pub const TET_BASE: u16 = 14;
        pub const DUO_BASE: u16 = 12;
        pub const OCT_BASE: u16 = 8;
        pub const SEN_BASE: u16 = 6;
        pub const BIN_BASE: u16 = 2;

        pub const SEX_CHARS: [char; 60] = [
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
            'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
            'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
            'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
        ];
        pub const VIG_CHARS: [char; 20] = [
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', '@', '^', '&', '*', '$', '+', '!',
            ';', ':', '~',
        ];
        pub const HEX_CHARS: [char; 16] = [
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
        ];
        pub const TET_CHARS: [char; 14] = [
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'E', 'W', 'R',
        ];
        pub const DUO_CHARS: [char; 12] =
            ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '#'];
        pub const OCT_CHARS: [char; 8] = ['0', '1', '2', '3', '4', '5', '6', '7'];
        pub const SEN_CHARS: [char; 6] = ['0', '1', '2', '3', '4', '5'];
        pub const BIN_CHARS: [char; 2] = ['0', '1'];
    }
}

mod types {
    pub type ArrTypeRef<'a> = &'a [u16];
    pub type ArrType = [u16; 4];
}

mod tools {
    use super::types;

    pub fn max_and_argmax(arr: types::ArrTypeRef, size_arr: usize) -> (u16, usize) {
        let mut curr_max = arr[0];
        let mut curr_index = 0usize;
        for i in 1..size_arr {
            if arr[i] > curr_max {
                curr_max = arr[i];
                curr_index = i;
            }
        }
        (curr_max, curr_index)
    }

    pub fn min_and_argmin(arr: types::ArrTypeRef, size_arr: usize) -> (u16, usize) {
        let mut curr_min = arr[0];
        let mut curr_index = 0usize;
        for i in 1..size_arr {
            if arr[i] < curr_min {
                curr_min = arr[i];
                curr_index = i;
            }
        }
        (curr_min, curr_index)
    }

    pub fn log2n(num: u16) -> u16 {
        if num > 1 {
            return 1 + log2n(num / 2);
        } else {
            return 0;
        }
    }

    pub fn copy_array(arr: types::ArrTypeRef) -> types::ArrType {
        let ret: types::ArrType = [arr[0], arr[1], arr[2], arr[3]];
        ret
    }
}

// https://github.com/Chubek/PoxHash/blob/master/SPEC.md#part-g-conversion--preparation-prep-methods
mod convert {
    use super::{consts::*, types};

    macro_rules! convert_decimal_to_base {
        ($base: path, $size: path, $chars: path, $res: ident, $dec: ident, $offset: ident) => {{
            let mut decimal = $dec;
            for i in ($offset * $size..($offset * $size) + $size).rev() {
                $res[i] = $chars[(decimal % $base) as usize];
                decimal /= $base;
            }
        }};
    }

    fn single_word_to_byte(word: u16) -> (u8, u8) {
        let lower: u8 = (word & masks::WORD_ZZFF) as u8;
        let upper: u8 = ((word & masks::WORD_FFZZ) >> bit_values::BYTE_WIDTH_U16) as u8;
        (lower, upper)
    }

    fn word_to_double(w1: u16, w2: u16) -> u32 {
        let mut res = 0u32;
        res |= w1 as u32;
        res |= (w2 as u32) << 16;

        res
    }

    pub fn word_array_to_byte_array(word_array: types::ArrTypeRef) -> [u8; 8] {
        let (b0, b1) = single_word_to_byte(word_array[0]);
        let (b2, b3) = single_word_to_byte(word_array[1]);
        let (b4, b5) = single_word_to_byte(word_array[2]);
        let (b6, b7) = single_word_to_byte(word_array[3]);
        [b0, b1, b2, b3, b4, b5, b6, b7]
    }

    pub fn word_array_to_double_array(word_array: types::ArrTypeRef) -> [u32; 2] {
        let lower = word_to_double(word_array[0], word_array[1]);
        let upper = word_to_double(word_array[2], word_array[3]);
        [lower, upper]
    }

    pub fn word_array_to_sex_digest(word_array: types::ArrTypeRef) -> String {
        let mut digest = vec!['0'; conversion::SEX_SIZE * size_values::PORTION_NUM];
        for i in 0..size_values::PORTION_NUM {
            let word = word_array[i];
            convert_decimal_to_base! {
                conversion::SEX_BASE,
                conversion::SEX_SIZE,
                conversion::SEX_CHARS,
                digest,
                word,
                i
            }
        }
        return digest.into_iter().collect();
    }

    pub fn word_array_to_vig_digest(word_array: types::ArrTypeRef) -> String {
        let mut digest = vec!['0'; conversion::VIG_SIZE * size_values::PORTION_NUM];
        for i in 0..size_values::PORTION_NUM {
            let word = word_array[i];
            convert_decimal_to_base! {
                conversion::VIG_BASE,
                conversion::VIG_SIZE,
                conversion::VIG_CHARS,
                digest,
                word,
                i
            }
        }
        return digest.into_iter().collect();
    }

    pub fn word_array_to_hex_digest(word_array: types::ArrTypeRef) -> String {
        let mut digest = vec!['0'; conversion::HEX_SIZE * size_values::PORTION_NUM];
        for i in 0..size_values::PORTION_NUM {
            let word = word_array[i];
            convert_decimal_to_base! {
                conversion::HEX_BASE,
                conversion::HEX_SIZE,
                conversion::HEX_CHARS,
                digest,
                word,
                i
            }
        }
        return digest.into_iter().collect();
    }

    pub fn word_array_to_tet_digest(word_array: types::ArrTypeRef) -> String {
        let mut digest = vec!['0'; conversion::TET_SIZE * size_values::PORTION_NUM];
        for i in 0..size_values::PORTION_NUM {
            let word = word_array[i];
            convert_decimal_to_base! {
                conversion::TET_BASE,
                conversion::TET_SIZE,
                conversion::TET_CHARS,
                digest,
                word,
                i
            }
        }
        return digest.into_iter().collect();
    }

    pub fn word_array_to_duo_digest(word_array: types::ArrTypeRef) -> String {
        let mut digest = vec!['0'; conversion::DUO_SIZE * size_values::PORTION_NUM];
        for i in 0..size_values::PORTION_NUM {
            let word = word_array[i];
            convert_decimal_to_base! {
                conversion::DUO_BASE,
                conversion::DUO_SIZE,
                conversion::DUO_CHARS,
                digest,
                word,
                i
            }
        }
        return digest.into_iter().collect();
    }

    pub fn word_array_to_oct_digest(word_array: types::ArrTypeRef) -> String {
        let mut digest = vec!['0'; conversion::OCT_SIZE * size_values::PORTION_NUM];
        for i in 0..size_values::PORTION_NUM {
            let word = word_array[i];
            convert_decimal_to_base! {
                conversion::OCT_BASE,
                conversion::OCT_SIZE,
                conversion::OCT_CHARS,
                digest,
                word,
                i
            }
        }
        return digest.into_iter().collect();
    }

    pub fn word_array_to_sen_digest(word_array: types::ArrTypeRef) -> String {
        let mut digest = vec!['0'; conversion::SEN_SIZE * size_values::PORTION_NUM];
        for i in 0..size_values::PORTION_NUM {
            let word = word_array[i];
            convert_decimal_to_base! {
                conversion::SEN_BASE,
                conversion::SEN_SIZE,
                conversion::SEN_CHARS,
                digest,
                word,
                i
            }
        }
        return digest.into_iter().collect();
    }

    pub fn word_array_to_bin_digest(word_array: types::ArrTypeRef) -> String {
        let mut digest = vec!['0'; conversion::BIN_SIZE * size_values::PORTION_NUM];
        for i in 0..size_values::PORTION_NUM {
            let word = word_array[i];
            convert_decimal_to_base! {
                conversion::BIN_BASE,
                conversion::BIN_SIZE,
                conversion::BIN_CHARS,
                digest,
                word,
                i
            }
        }
        return digest.into_iter().collect();
    }

    pub fn word_array_to_quad(word_array: types::ArrTypeRef) -> u64 {
        let mut quad = 0u64;

        quad |= word_array[0] as u64;
        quad |= (word_array[1] as u64) << 16;
        quad |= (word_array[2] as u64) << 32;
        quad |= (word_array[3] as u64) << 48;

        quad
    }
}

mod operations {
    use super::{consts::*, types};

    // https://github.com/Chubek/PoxHash/blob/master/SPEC.md#part-b-bitwise-operations
    pub mod bitwise {
        use super::*;

        pub fn octopad(byte_array: &Vec<u8>) -> Vec<u16> {
            let original_len = byte_array.len();
            let mut n = original_len;
            let mut word_vec = byte_array
                .into_iter()
                .map(|b| *b as u16)
                .collect::<Vec<u16>>();
            while word_vec.len() % size_values::BLOCK_NUM != 0 {
                word_vec.push(word_vec[n % original_len] ^ (n & masks::MASK_QWORD_14Z2F) as u16);
                n += word_vec[n % original_len] as usize;
            }
            word_vec
        }

        pub fn omega(num: u32) -> u32 {
            (num & masks::DWORD_4F4Z) >> bit_values::WORD_WIDTH_U32
        }

        pub fn epsilon(num: u32) -> u32 {
            num & masks::DWORD_4Z4F
        }

        pub fn lamed(num: u32, by: u32) -> u32 {
            (num << by) | (num >> (bit_values::WORD_WIDTH_U32 - by))
        }
    }

    // https://github.com/Chubek/PoxHash/blob/master/SPEC.md#part-c-bespoke-operations
    pub mod bespoke {
        use super::*;

        pub fn gorda(num: u16, by: u32) -> u16 {
            let mut res = num as u32;
            res = bitwise::lamed(res, by);
            if res > bit_values::UINT16_MAX_U32 {
                res = bitwise::omega(res);
            }
            res as u16
        }

        pub fn tasu(a: u16, b: u16) -> u16 {
            let (aa, bb) = (a as u32, b as u32);

            let mut a_plus_b: u32 = (aa + bb) as u32;
            if a_plus_b > bit_values::UINT16_MAX_U32 {
                a_plus_b = bitwise::epsilon(a_plus_b);
            }
            a_plus_b as u16
        }

        pub fn centum(arr: types::ArrTypeRef, weights: &[u16]) -> u16 {
            let mut wtmt = 0u32;
            for i in 0..size_values::PORTION_NUM {
                wtmt += (arr[i] * weights[i]) as u32;
            }
            wtmt /= size_values::PORTION_NUM as u32;
            if wtmt > bit_values::UINT16_MAX_U32 {
                wtmt = bitwise::omega(wtmt);
            }
            wtmt as u16
        }

        pub fn satum(arr: types::ArrTypeRef, weights: &[u16]) -> u16 {
            let mut wdca = 0u32;
            for i in 0..size_values::PORTION_NUM {
                wdca += (arr[i] * weights[i]) as u32;
            }
            wdca = (wdca + 1) / 2;
            if wdca > bit_values::UINT16_MAX_U32 {
                wdca = bitwise::epsilon(wdca);
            }
            wdca as u16
        }

        pub fn tamaam(arr: &[u16]) -> u16 {
            let mut wtmt = 0u32;
            for i in 0..size_values::PORTION_NUM {
                wtmt += arr[i] as u32;
            }
            wtmt /= size_values::PORTION_NUM as u32;
            if wtmt > bit_values::UINT16_MAX_U32 {
                wtmt = bitwise::omega(wtmt);
            }
            wtmt as u16
        }

        pub fn deca(arr: &[u16]) -> u16 {
            let mut wdca = 0u32;
            for i in 0..size_values::PORTION_NUM {
                wdca += arr[i] as u32;
            }
            wdca = (wdca + 1) / 2;
            if wdca > bit_values::UINT16_MAX_U32 {
                wdca = bitwise::epsilon(wdca);
            }
            wdca as u16
        }

        pub fn get_8b_prime(num: u16) -> u16 {
            prime_arrays::BYTE_PRIMES[(num as usize) % size_values::BYTE_PRIME_NUM]
        }
    }
}

// https://github.com/Chubek/PoxHash/blob/master/SPEC.md#part-d-alphabet-operations
mod alphabet {
    use super::{consts::*, operations::*, tools, types};

    pub fn alpha(temp_array: types::ArrTypeRef) -> types::ArrType {
        let aleph: u16 = (temp_array[0] ^ temp_array[1]) & masks::WORD_ZZFF;
        let daal: u16 = (temp_array[2] ^ temp_array[3]) & masks::WORD_FFZZ;
        let theh: u16 = (aleph | daal) % prime_arrays::BYTE_PRIMES[0];
        let gaaf: u16 = (aleph ^ daal) % prime_arrays::BYTE_PRIMES[1];

        let mut temp_array_cpy = tools::copy_array(temp_array);

        temp_array_cpy[0] >>= theh;
        temp_array_cpy[1] >>= ((theh + gaaf) % 2) + 1;
        temp_array_cpy[2] >>= gaaf;

        temp_array_cpy
    }

    pub fn delta(temp_array: types::ArrTypeRef) -> types::ArrType {
        let mut alaf: u16 =
            (temp_array[0] ^ masks::WORD_FFFZ) % bespoke::get_8b_prime(temp_array[0]);
        let mut dalat: u16 =
            (temp_array[1] ^ masks::WORD_FZZF) % bespoke::get_8b_prime(temp_array[1]);
        let mut tit: u16 =
            (temp_array[2] & masks::WORD_ZFFF) % bespoke::get_8b_prime(temp_array[2]);
        let mut gaman: u16 =
            (temp_array[3] & masks::WORD_FFZZ) % bespoke::get_8b_prime(temp_array[3]);

        for _ in 0..size_values::PORTION_NUM {
            alaf >>= prime_arrays::SINGLE_DIGIT_PRIMES
                [(dalat % (size_values::SD_PRIME_NUM as u16)) as usize];
            dalat = bespoke::gorda(dalat, 2);
            tit >>= prime_arrays::SINGLE_DIGIT_PRIMES
                [(gaman % (size_values::SD_PRIME_NUM as u16)) as usize];
            gaman ^= (alaf ^ masks::WORD_ZZFF)
                >> prime_arrays::SINGLE_DIGIT_PRIMES
                    [(tit % (size_values::SD_PRIME_NUM as u16)) as usize];
        }

        let mut temp_array_cpy = tools::copy_array(temp_array);

        temp_array_cpy[1] ^= temp_array[2]
            % prime_arrays::MAGIC_PRIMES[(alaf % (size_values::MAGIC_PRIME_NUM as u16)) as usize];
        temp_array_cpy[2] ^= alaf + tit;
        temp_array_cpy[3] ^= tit + gaman;

        temp_array_cpy
    }

    pub fn theta(temp_array: types::ArrTypeRef) -> types::ArrType {
        let alef: u16 = temp_array[0] % 2;
        let dalet: u16 = temp_array[1] % 2;
        let tet: u16 = temp_array[2] % 2;
        let gimmel: u16 = temp_array[3] % 2;

        let ctm: u16 = bespoke::centum(temp_array, &[alef, dalet, tet, gimmel]);
        let stm: u16 = bespoke::satum(temp_array, &[alef, dalet, tet, gimmel]);

        let mut temp_array_cpy = tools::copy_array(temp_array);

        temp_array_cpy[0] ^= ((ctm >> gimmel) ^ masks::WORD_ZZFF) & masks::WORD_ZZZF;
        temp_array_cpy[3] ^= ((stm << alef) ^ masks::WORD_FZFZ) & masks::WORD_FZZZ;

        temp_array_cpy
    }

    pub fn gamma(temp_array: types::ArrTypeRef) -> types::ArrType {
        let (mmin, argmin) = tools::min_and_argmin(temp_array, size_values::PORTION_NUM);
        let (mmax, argmax) = tools::max_and_argmax(temp_array, size_values::PORTION_NUM);
        let ay = argmin & masks::NIBBLET_01;
        let dee = argmax ^ masks::NIBBLET_10;
        let thorn = argmin & masks::NIBBLET_11;
        let gee = argmax ^ masks::NIBBLET_00;

        let alaph: u16 = temp_array[ay] % bespoke::get_8b_prime(temp_array[thorn]);
        let dalath: u16 =
            (bespoke::get_8b_prime(mmax) ^ masks::WORD_ZFZF) % bespoke::get_8b_prime(mmin);
        let teth: u16 = mmax % bespoke::get_8b_prime(mmax);
        let gamal: u16 =
            temp_array[dee] % bespoke::get_8b_prime((((mmin as u32) + (mmax as u32)) / 2) as u16);
        let mut temp_array_cpy = tools::copy_array(temp_array);

        temp_array_cpy[ay] >>= (alaph ^ masks::WORD_ZZFZ) % bit_values::WORD_WIDTH_U16;
        temp_array_cpy[dee] >>= (gamal ^ masks::WORD_FZZZ) % ((mmax % 2) + 1);
        temp_array_cpy[thorn] ^= tools::log2n(dalath) & masks::WORD_ZFFF;
        temp_array_cpy[gee] ^= tools::log2n(teth) >> ((gamal % 2) + 1);

        temp_array_cpy
    }
}

// https://github.com/Chubek/PoxHash/blob/master/SPEC.md#part-e-round-methods
mod round {
    use super::{alphabet, consts::*, operations::*, tools, types};

    macro_rules! swap {
        ($arr: ident, $indexof: ident, $indexwith: ident) => {{
            let tmp = $arr[$indexof];
            $arr[$indexof] = $arr[$indexwith];
            $arr[$indexwith] = tmp;
        }};
    }

    fn apply_alphabet_operation(temp_array: types::ArrTypeRef) -> types::ArrType {
        let mut temp_array_cpy = tools::copy_array(temp_array);
        temp_array_cpy = alphabet::alpha(&temp_array_cpy);
        temp_array_cpy = alphabet::delta(&temp_array_cpy);
        temp_array_cpy = alphabet::theta(&temp_array_cpy);
        temp_array_cpy = alphabet::gamma(&temp_array_cpy);
        temp_array_cpy
    }

    fn apply_bahman(temp_array: types::ArrTypeRef, pnum: u16) -> types::ArrType {
        let mut temp_array_cpy = tools::copy_array(temp_array);
        let cica = (pnum % (size_values::PORTION_NUM as u16)) as usize;
        let mica = ((cica + 1) % size_values::PORTION_NUM) as usize;
        let nica = ((mica + 2) % size_values::PORTION_NUM) as usize;
        let wica = ((nica + 3) % size_values::PORTION_NUM) as usize;

        let mianju = (temp_array[cica] % (size_values::MASKS_ARRAY_NUM as u16)) as usize;
        let mianja = (temp_array[mica] % (size_values::MASKS_ARRAY_NUM as u16)) as usize;

        let sosu = (temp_array[nica] % (size_values::ROUND_PRIME_NUM as u16)) as usize;
        let sosa = (temp_array[wica] % (size_values::ROUND_PRIME_NUM as u16)) as usize;

        temp_array_cpy[cica] ^= (temp_array_cpy[mica] << cica) & masks::MASKS_ARRAY[mianju];
        temp_array_cpy[wica] &= temp_array_cpy[wica] ^ prime_arrays::ROUND_PRIMES[sosu];
        temp_array_cpy[nica] ^= (temp_array_cpy[cica] << (wica * 2)) & masks::MASKS_ARRAY[mianja];
        temp_array_cpy[mica] |= temp_array_cpy[nica] | prime_arrays::ROUND_PRIMES[sosa];

        temp_array_cpy
    }

    fn apply_prime(temp_array: types::ArrTypeRef) -> types::ArrType {
        let mut temp_array_cpy = tools::copy_array(temp_array);
        for i in 0..size_values::ROUND_PRIME_NUM {
            temp_array_cpy[0] %= prime_arrays::ROUND_PRIMES[i];
            temp_array_cpy[1] %= prime_arrays::ROUND_PRIMES[i];
            temp_array_cpy[2] %= prime_arrays::ROUND_PRIMES[i];
            temp_array_cpy[3] %= prime_arrays::ROUND_PRIMES[i];
            temp_array_cpy = apply_bahman(&temp_array_cpy, prime_arrays::ROUND_PRIMES[i]);
        }
        temp_array_cpy
    }

    fn apply_add_temp_to_facts(
        factor_array: types::ArrTypeRef,
        temp_array: types::ArrTypeRef,
    ) -> types::ArrType {
        let mut factor_array_cpy = tools::copy_array(factor_array);
        factor_array_cpy[0] = bespoke::tasu(factor_array_cpy[0], temp_array[0]);
        factor_array_cpy[1] = bespoke::tasu(factor_array_cpy[1], temp_array[1]);
        factor_array_cpy[2] = bespoke::tasu(factor_array_cpy[2], temp_array[2]);
        factor_array_cpy[3] = bespoke::tasu(factor_array_cpy[3], temp_array[3]);
        factor_array_cpy
    }

    fn apply_shuffle(temp_array: types::ArrTypeRef) -> types::ArrType {
        let mut temp_array_cpy = tools::copy_array(temp_array);
        for i in 0..misc::SIZE_BIONOM {
            let (iof, iwith) = misc::COMB_BIONOM[i];
            swap!(temp_array_cpy, iof, iwith);
        }
        temp_array_cpy
    }

    pub fn one_round(factor_array: types::ArrTypeRef) -> types::ArrType {
        let mut temp_array: types::ArrType = [
            factor_array[0],
            factor_array[1],
            factor_array[2],
            factor_array[3],
        ];

        temp_array = apply_alphabet_operation(&temp_array);
        temp_array = apply_prime(&temp_array);
        temp_array = apply_shuffle(&temp_array);
        let add_result = apply_add_temp_to_facts(factor_array, &temp_array);
        add_result
    }
}

// https://github.com/Chubek/PoxHash/blob/master/SPEC.md#part-f-block-methods
mod block {
    use super::{consts::*, operations::*, round, tools, types};

    fn apply_bytes(factor_array: types::ArrTypeRef, portion: &[u16], index: u16) -> types::ArrType {
        let tmt = bespoke::tamaam(portion);
        let dca = bespoke::deca(portion);
        let tmt_odd_factor = bit_values::UINT16_MAX_U16 ^ (tmt % (dca + 2));
        let dca_odd_factor = bit_values::UINT16_MAX_U16 ^ (dca % (tmt + 3));

        let ng = ((portion[0] + index) % (size_values::PORTION_NUM as u16)) as usize;
        let chu = ((portion[1] + index) % (size_values::PORTION_NUM as u16)) as usize;
        let yo = ((portion[2] + index) % (size_values::PORTION_NUM as u16)) as usize;
        let eo = ((portion[3] + index) % (size_values::PORTION_NUM as u16)) as usize;

        let zam = portion[0]
            % prime_arrays::BYTE_PRIMES[(portion[chu] as usize) % size_values::BYTE_PRIME_NUM];
        let pez = portion[1]
            % prime_arrays::BYTE_PRIMES[(portion[yo] as usize) % size_values::BYTE_PRIME_NUM];
        let dit = portion[2]
            % prime_arrays::BYTE_PRIMES[(portion[eo] as usize) % size_values::BYTE_PRIME_NUM];
        let kit = portion[3]
            % prime_arrays::BYTE_PRIMES[(portion[ng] as usize) % size_values::BYTE_PRIME_NUM];

        let mut factor_array_cpy = tools::copy_array(factor_array);
        factor_array_cpy[ng] ^= (((portion[eo] >> chu) | tmt) ^ dca_odd_factor) | zam;
        factor_array_cpy[chu] ^= ((portion[yo] & dca) ^ tmt_odd_factor) ^ pez;
        factor_array_cpy[yo] ^= ((portion[chu] ^ tmt) ^ dca_odd_factor) | dit;
        factor_array_cpy[eo] ^= (((portion[ng] >> yo) | dca) ^ tmt_odd_factor) ^ kit;

        factor_array_cpy[0] >>= portion[3] % ((ng + 1) as u16);
        factor_array_cpy[1] >>= portion[2] % ((chu + 1) as u16);
        factor_array_cpy[2] ^= portion[1] >> ((dca % 2) as u16);
        factor_array_cpy[3] >>= portion[0] % ((eo + 1) as u16);

        factor_array_cpy
    }

    pub fn process_block(factor_array: types::ArrTypeRef, block: &[u16]) -> types::ArrType {
        let mut factor_array_cpy = tools::copy_array(factor_array);
        for i in (0..size_values::BLOCK_NUM).step_by(size_values::CHUNK_NUM) {
            for j in (i..i + size_values::CHUNK_NUM).step_by(size_values::PORTION_NUM) {
                let portion: &[u16] = &[block[j], block[j + 1], block[j + 2], block[j + 3]];
                for m in 0..size_values::ROUND_NUM {
                    factor_array_cpy = apply_bytes(&factor_array_cpy, portion, m as u16);
                    factor_array_cpy = round::one_round(&factor_array_cpy);
                }
            }
        }
        factor_array_cpy
    }
}

pub struct PoxDigest {
    pub sexdigest: String,
    pub vigdigest: String,
    pub hexdigest: String,
    pub tetdigest: String,
    pub duodigest: String,
    pub octdigest: String,
    pub sendigest: String,
    pub bindigest: String,
    pub bytes: [u8; 8],
    pub words: [u16; 4],
    pub doubles: [u32; 2],
    pub quad: u64,
}

impl std::default::Default for PoxDigest {
    fn default() -> Self {
        Self {
            sexdigest: String::from("unset"),
            vigdigest: String::from("unset"),
            hexdigest: String::from("unset"),
            tetdigest: String::from("unset"),
            duodigest: String::from("unset"),
            octdigest: String::from("unset"),
            sendigest: String::from("unset"),
            bindigest: String::from("unset"),
            bytes: [0u8; 8],
            words: [0u16; 4],
            doubles: [0u32; 2],
            quad: 0u64,
        }
    }
}

impl Clone for PoxDigest {
    fn clone(&self) -> Self {
        let mut bytes_clone = [0u8; 8];
        let mut words_clone = [0u16; 4];
        let mut doubles_clone = [0u32; 2];
        let quad_clone = self.quad;

        for i in 0..8 {
            bytes_clone[i] = self.bytes[i];
        }
        for i in 0..4 {
            words_clone[i] = self.words[i];
        }
        for i in 0..2 {
            doubles_clone[i] = self.doubles[i];
        }
        let sexdigest_clone = self.sexdigest.clone();
        let vigdigest_clone = self.vigdigest.clone();
        let hexdigest_clone = self.hexdigest.clone();
        let tetdigest_clone = self.tetdigest.clone();
        let duodigest_clone = self.duodigest.clone();
        let octdigest_clone = self.octdigest.clone();
        let sendigest_clone = self.sendigest.clone();
        let bindigest_clone = self.bindigest.clone();

        Self {
            sexdigest: sexdigest_clone,
            vigdigest: vigdigest_clone,
            hexdigest: hexdigest_clone,
            tetdigest: tetdigest_clone,
            duodigest: duodigest_clone,
            octdigest: octdigest_clone,
            sendigest: sendigest_clone,
            bindigest: bindigest_clone,
            bytes: bytes_clone,
            words: words_clone,
            doubles: doubles_clone,
            quad: quad_clone,
        }
    }
}

impl std::fmt::Display for PoxDigest {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let mut _result = write!(f, "Sexdigest: {}\n", &self.sexdigest);
        _result = write!(f, "Vigdigest: {}\n", &self.vigdigest);
        _result = write!(f, "Hexdigest: {}\n", &self.hexdigest);
        _result = write!(f, "Tetdigest: {}\n", &self.tetdigest);
        _result = write!(f, "Duodigest: {}\n", &self.duodigest);
        _result = write!(f, "Octdigest: {}\n", &self.octdigest);
        _result = write!(f, "Sendigest: {}\n", &self.sendigest);
        _result = write!(f, "Bindigest: {}\n", &self.bindigest);
        _result = write!(f, "Sexdigest: {}\n", &self.sexdigest);
        _result = write!(
            f,
            "Bytes:    U8[{}, {}, {}, {}, {}, {}, {}, {}]\n",
            &self.bytes[0],
            &self.bytes[1],
            &self.bytes[2],
            &self.bytes[3],
            &self.bytes[4],
            &self.bytes[5],
            &self.bytes[6],
            &self.bytes[7]
        );
        _result = write!(
            f,
            "Words:    U16[{}, {}, {}, {}]\n",
            &self.words[0], &self.words[1], &self.words[2], &self.words[3]
        );
        _result = write!(
            f,
            "Doubles:  U32[{}, {}]\n",
            &self.doubles[0], &self.doubles[1]
        );
        write!(f, "Quad:     U64[{}]", &self.quad)
    }
}

impl std::fmt::Debug for PoxDigest {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("PoxDigest")
            .field("Sexdigest", &self.sexdigest)
            .field("Vigdigest", &self.vigdigest)
            .field("Hexdigest", &self.hexdigest)
            .field("Tetdigest", &self.tetdigest)
            .field("Duodigest", &self.duodigest)
            .field("Octdigest", &self.octdigest)
            .field("Sendigest", &self.sendigest)
            .field("Bindigest", &self.bindigest)
            .field("Bytes", &self.bytes)
            .field("Words", &self.words)
            .field("Doubles", &self.doubles)
            .field("Quad", &self.quad)
            .finish()
    }
}

impl std::fmt::Binary for PoxDigest {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.bindigest)
    }
}

impl std::fmt::Octal for PoxDigest {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.octdigest)
    }
}

impl std::fmt::UpperHex for PoxDigest {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.hexdigest)
    }
}

impl std::fmt::LowerHex for PoxDigest {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.hexdigest.to_lowercase())
    }
}

impl std::fmt::Pointer for PoxDigest {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let ptr = self as *const Self;
        std::fmt::Pointer::fmt(&ptr, f)
    }
}

#[allow(unused_doc_comments)]
pub fn pox_hash(message: &Vec<u8>) -> PoxDigest {
    /// Converts the given message into a PoxDigest object
    /// Parameters:
    ///     message: Vec<u8>
    ///
    /// Returns:
    ///     PoxDigest
    ///         PoxDigest.sexdigest: String
    ///         PoxDigest.vigdigest: String
    ///         PoxDigest.hexdigest: String
    ///         PoxDigest.tetdigest: String
    ///         PoxDigest.duodigest: String
    ///         PoxDigest.octdigest: String
    ///         PoxDigest.sendigest: String
    ///         PoxDigest.bindigest: String
    ///         PoxDigest.bytes: [u8; 8]
    ///         PoxDigest.words: [u16; 4]
    ///         PoxDigest.doubles: [u32, 2]
    ///         PoxDigest.quad: u64
    let padded_u16 = operations::bitwise::octopad(message);
    let mut factor_array: types::ArrType = [
        consts::initial_primes::PRIME_INIT_A,
        consts::initial_primes::PRIME_INIT_B,
        consts::initial_primes::PRIME_INIT_C,
        consts::initial_primes::PRIME_INIT_D,
    ];

    for i in (0..padded_u16.len()).step_by(consts::size_values::BLOCK_NUM) {
        factor_array = block::process_block(
            &factor_array,
            &padded_u16[i..i + consts::size_values::BLOCK_NUM],
        );
    }

    let sexdigest = convert::word_array_to_sex_digest(&factor_array);
    let vigdigest = convert::word_array_to_vig_digest(&factor_array);
    let hexdigest = convert::word_array_to_hex_digest(&factor_array);
    let tetdigest = convert::word_array_to_tet_digest(&factor_array);
    let duodigest = convert::word_array_to_duo_digest(&factor_array);
    let octdigest = convert::word_array_to_oct_digest(&factor_array);
    let sendigest = convert::word_array_to_sen_digest(&factor_array);
    let bindigest = convert::word_array_to_bin_digest(&factor_array);
    let bytes = convert::word_array_to_byte_array(&factor_array);
    let words: [u16; 4] = [
        factor_array[0],
        factor_array[1],
        factor_array[2],
        factor_array[3],
    ];
    let doubles = convert::word_array_to_double_array(&factor_array);
    let quad = convert::word_array_to_quad(&factor_array);

    PoxDigest {
        sexdigest,
        vigdigest,
        hexdigest,
        tetdigest,
        duodigest,
        octdigest,
        sendigest,
        bindigest,
        bytes,
        words,
        doubles,
        quad,
    }
}
