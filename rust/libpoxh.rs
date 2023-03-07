//////////////////////////////////////////////
//      PoxHash v1                          //
//      Implementation in Rust 			    //
//      By Chubak Bidpaa - March 2023       //
//      License: GPLv3                      //
//////////////////////////////////////////////

mod consts {
    pub const POX_PRIMES: &'static [u16] = &[
        0xe537, 0xbd71, 0x9ef9, 0xbbcf, 0xf8dd, 0xceb7, 0xbaa1, 0x8f9f, 0xb0ed, 0xfc4f, 0x9787,
        0xf01f, 0xe1d1, 0xbcb9, 0xd565, 0xc011, 0xc1e1, 0xb58d, 0xd4e1, 0x9ea1, 0xee49, 0x97cd,
        0xdac9, 0xe257, 0xa32b, 0xafbb, 0xa5e3, 0xfc43, 0xbf71, 0xe401, 0x8ebd, 0xd549,
    ];
    pub const POX_8B_PRIMES: &'static [u16] = &[
        0x2, 0x3, 0x5, 0x7, 0xb, 0xd, 0x11, 0x13, 0x17, 0x1d, 0x1f, 0x25, 0x29, 0x2b, 0x2f, 0x35,
        0x3b, 0x3d, 0x43, 0x47, 0x49, 0x4f, 0x53, 0x59, 0x61, 0x65, 0x67, 0x6b, 0x6d, 0x71, 0x7f,
        0x83, 0x89, 0x8b, 0x95, 0x97, 0x9d, 0xa3, 0xa7, 0xad, 0xb3, 0xb5, 0xbf, 0xc1, 0xc5, 0xc7,
        0xd3, 0xdf, 0xe3, 0xe5, 0xe9, 0xef, 0xf1, 0xfb,
    ];
    pub const POX_MAGIC_PRIMES: &'static [u16] = &[0x33, 0x65];
    pub const POX_SINGLE_DIGIT_PRIMES: &'static [u16] = &[0x3, 0x5, 0x7];

    pub const POX_PRIME_A: u16 = 0x9f91;
    pub const POX_PRIME_B: u16 = 0xdb3b;
    pub const POX_PRIME_C: u16 = 0xc091;
    pub const POX_PRIME_D: u16 = 0xac8b;

    pub const POX_BLOCK_NUM: usize = 64;
    pub const POX_8B_PRIME_NUM: usize = 54;
    pub const POX_PRIME_NUM: usize = 32;
    pub const POX_CHUNK_NUM: usize = 16;
    pub const POX_ROUND_NUM: usize = 8;
    pub const POX_PORTION_NUM: usize = 4;
    pub const POX_NUM_SD_PRIME: u16 = 3;
    pub const POX_NUM_MAGIC_PRIME: u16 = 2;

    pub const UINT16_MAX_U32: u32 = 65535;
    pub const UINT16_MAX_U16: u16 = 65535;
    pub const WORD_WIDTH_U32: u32 = 16;
    pub const WORD_WIDTH_U16: u16 = 16;
    pub const BYTE_WIDTH_U16: u16 = 8;
    pub const HEX_SIZE: usize = 4;
    pub const ONE_UPPER16: u32 = 0xffff0000;
    pub const ONE_LOWER16: u32 = 0x0000ffff;

    pub const MASK_FZFZ: u16 = 0xf0f0;
    pub const MASK_ZFZF: u16 = 0x0f0f;
    pub const MASK_FZZZ: u16 = 0xf000;
    pub const MASK_ZZFZ: u16 = 0x00f0;
    pub const MASK_ZZZF: u16 = 0x000f;
    pub const MASK_ZZFF: u16 = 0x00ff;
    pub const MASK_FFZZ: u16 = 0xff00;
    pub const MASK_FZZF: u16 = 0xf00f;
    pub const MASK_FFFZ: u16 = 0xfff0;
    pub const MASK_ZFFF: u16 = 0x0fff;
    pub const MASK_01: usize = 0b01;
    pub const MASK_10: usize = 0b10;
    pub const MASK_11: usize = 0b11;
    pub const MASK_00: usize = 0b00;

    pub const COMB_BIONOM: &'static [(usize, usize)] =
        &[(0, 1), (0, 2), (0, 3), (1, 2), (1, 3), (2, 3)];
    pub const SIZE_BIONOM: usize = 6;
    pub const HEX_CHARS: &'static [char] = &[
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    ];

    pub type ArrTypeRef<'a> = &'a [u16];
    pub type ArrType = [u16; 4];
}

mod tools {
    use super::consts;

    pub fn sum_portion(arr: consts::ArrTypeRef) -> u16 {
        let mut sum = arr[0];
        for i in 1..consts::POX_PORTION_NUM {
            sum += arr[i];
        }
        sum
    }

    pub fn max_and_argmax(arr: consts::ArrTypeRef, size_arr: usize) -> (u16, usize) {
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

    pub fn min_and_argmin(arr: consts::ArrTypeRef, size_arr: usize) -> (u16, usize) {
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

    pub fn get_8b_prime(num: u16) -> u16 {
        consts::POX_8B_PRIMES[(num as usize) % consts::POX_8B_PRIME_NUM]
    }

    pub fn log2n(num: u16) -> u16 {
        if num > 1 {
            return 1 + log2n(num / 2);
        } else {
            return 0;
        }
    }

    pub fn copy_array(arr: consts::ArrTypeRef) -> consts::ArrType {
        let ret: consts::ArrType = [arr[0], arr[1], arr[2], arr[3]];
        ret
    }
}

mod bits {
    use super::consts;

    pub fn rotate_left(num: u16, by: u32) -> u16 {
        let mut res = num as u32;
        res = (res << by) | (res >> (consts::WORD_WIDTH_U32 - by));
        if res > consts::UINT16_MAX_U32 {
            res = (res & consts::ONE_UPPER16) >> consts::WORD_WIDTH_U32;
        }
        res as u16
    }

    pub fn add_with_overflow(a: u16, b: u16) -> u16 {
        let (aa, bb) = (a as u32, b as u32);

        let mut a_plus_b: u32 = (aa + bb) as u32;
        if a_plus_b > consts::UINT16_MAX_U32 {
            a_plus_b &= consts::ONE_LOWER16;
        }
        a_plus_b as u16
    }

    pub fn weighted_average(arr: consts::ArrTypeRef, weights: &[u16]) -> u16 {
        let mut wavg = 0u32;
        for i in 0..consts::POX_PORTION_NUM {
            wavg += (arr[i] * weights[i]) as u32;
        }
        wavg /= consts::POX_PORTION_NUM as u32;
        if wavg > consts::UINT16_MAX_U32 {
            wavg = (wavg & consts::ONE_UPPER16) >> consts::WORD_WIDTH_U32;
        }
        wavg as u16
    }

    pub fn weighted_median(arr: consts::ArrTypeRef, weights: &[u16]) -> u16 {
        let mut wmed = 0u32;
        for i in 0..consts::POX_PORTION_NUM {
            wmed += (arr[i] * weights[i]) as u32;
        }
        wmed = (wmed + 1) / 2;
        if wmed > consts::UINT16_MAX_U32 {
            wmed &= consts::ONE_LOWER16;
        }
        wmed as u16
    }
}

mod convert {
    use super::consts;

    fn single_word_to_byte(word: u16) -> (u8, u8) {
        let lower: u8 = (word & consts::MASK_ZZFF) as u8;
        let upper: u8 = ((word & consts::MASK_FFZZ) >> consts::BYTE_WIDTH_U16) as u8;
        (lower, upper)
    }

    fn decimal_to_hex(dec: u16) -> String {
        let mut hex = vec!['0' as u8; consts::HEX_SIZE];
        let mut dec_mut = dec;
        hex.iter_mut().rev().for_each(|c| {
            *c = consts::HEX_CHARS[(dec_mut % consts::WORD_WIDTH_U16) as usize] as u8;
            dec_mut /= consts::WORD_WIDTH_U16;
        });
        String::from_utf8(hex).unwrap()
    }

    pub fn word_array_to_byte_array(word_array: consts::ArrTypeRef) -> Vec<u8> {
        let (b0, b1) = single_word_to_byte(word_array[0]);
        let (b2, b3) = single_word_to_byte(word_array[1]);
        let (b4, b5) = single_word_to_byte(word_array[2]);
        let (b6, b7) = single_word_to_byte(word_array[3]);
        vec![b0, b1, b2, b3, b4, b5, b6, b7]
    }

    pub fn word_array_to_hex_digest(word_array: consts::ArrTypeRef) -> String {
        let hex_a = decimal_to_hex(word_array[0]);
        let hex_b = decimal_to_hex(word_array[1]);
        let hex_c = decimal_to_hex(word_array[2]);
        let hex_d = decimal_to_hex(word_array[3]);
        format!("{}{}{}{}", hex_a, hex_b, hex_c, hex_d)
    }

    pub fn byte_vec_to_word_vec_and_pad(byte_array: Vec<u8>) -> Vec<u16> {
        let mut word_vec = byte_array
            .into_iter()
            .map(|b| b as u16)
            .collect::<Vec<u16>>();
        while word_vec.len() % consts::POX_BLOCK_NUM != 0 {
            word_vec.push(0);
        }
        word_vec
    }
}

mod alphabet {
    use super::{bits, consts, tools};

    pub fn alpha(temp_array: consts::ArrTypeRef) -> consts::ArrType {
        let aleph: u16 = (temp_array[0] ^ temp_array[1]) & consts::MASK_ZZFF;
        let theh: u16 = (temp_array[2] ^ temp_array[3]) & consts::MASK_FFZZ;
        let daal: u16 = (aleph | theh) % consts::POX_8B_PRIMES[0];
        let gaaf: u16 = (aleph ^ theh) % consts::POX_8B_PRIMES[1];

        let mut temp_array_cpy = tools::copy_array(temp_array);

        temp_array_cpy[0] >>= daal;
        temp_array_cpy[1] >>= ((daal + gaaf) % 2) + 1;
        temp_array_cpy[2] >>= gaaf;

        temp_array_cpy
    }

    pub fn delta(temp_array: consts::ArrTypeRef) -> consts::ArrType {
        let mut alaf: u16 =
            (temp_array[0] ^ consts::MASK_FFFZ) % tools::get_8b_prime(temp_array[0]);
        let mut dalat: u16 =
            (temp_array[1] ^ consts::MASK_FZZF) % tools::get_8b_prime(temp_array[1]);
        let mut tit: u16 = (temp_array[2] & consts::MASK_ZFFF) % tools::get_8b_prime(temp_array[2]);
        let mut gaman: u16 =
            (temp_array[3] & consts::MASK_FFZZ) % tools::get_8b_prime(temp_array[3]);

        for _ in 0..consts::POX_PORTION_NUM {
            alaf >>= consts::POX_SINGLE_DIGIT_PRIMES[(dalat % consts::POX_NUM_SD_PRIME) as usize];
            dalat = bits::rotate_left(dalat, 2);
            tit >>= consts::POX_SINGLE_DIGIT_PRIMES[(gaman % consts::POX_NUM_SD_PRIME) as usize];
            gaman ^= (alaf ^ consts::MASK_ZZFF)
                >> consts::POX_SINGLE_DIGIT_PRIMES[(tit % consts::POX_NUM_SD_PRIME) as usize];
        }

        let mut temp_array_cpy = tools::copy_array(temp_array);

        temp_array_cpy[1] ^=
            temp_array[2] % consts::POX_MAGIC_PRIMES[alaf % consts::POX_NUM_MAGIC_PRIME];
        temp_array_cpy[2] ^= alaf + tit;
        temp_array_cpy[3] ^= tit + gaman;

        temp_array_cpy
    }

    pub fn theta(temp_array: consts::ArrTypeRef) -> consts::ArrType {
        let alef: u16 = temp_array[0] % 2;
        let dalet: u16 = temp_array[1] % 2;
        let tet: u16 = temp_array[2] % 2;
        let gimmel: u16 = temp_array[3] % 2;

        let weighted_avg: u16 = bits::weighted_average(temp_array, &[alef, dalet, tet, gimmel]);
        let weighted_med: u16 = bits::weighted_median(temp_array, &[alef, dalet, tet, gimmel]);

        let mut temp_array_cpy = tools::copy_array(temp_array);

        temp_array_cpy[0] ^= ((weighted_avg >> gimmel) ^ consts::MASK_ZZFF) & consts::MASK_ZZZF;
        temp_array_cpy[3] ^= ((weighted_med << alef) ^ consts::MASK_FZFZ) & consts::MASK_FZZZ;

        temp_array_cpy
    }

    pub fn gamma(temp_array: consts::ArrTypeRef) -> consts::ArrType {
        let (mmin, argmin) = tools::min_and_argmin(temp_array, consts::POX_PORTION_NUM);
        let (mmax, argmax) = tools::max_and_argmax(temp_array, consts::POX_PORTION_NUM);
        let ay = argmin & consts::MASK_01;
        let dee = argmax ^ consts::MASK_10;
        let thorn = argmin & consts::MASK_11;
        let gee = argmax ^ consts::MASK_00;

        let alaph: u16 = temp_array[ay] % tools::get_8b_prime(temp_array[thorn]);
        let dalath: u16 =
            (tools::get_8b_prime(mmax) ^ consts::MASK_ZFZF) % tools::get_8b_prime(mmin);
        let teth: u16 = mmax % tools::get_8b_prime(mmax);
        let gamal: u16 =
            temp_array[dee] % tools::get_8b_prime((((mmin as u32) + (mmax as u32)) / 2) as u16);
        let mut temp_array_cpy = tools::copy_array(temp_array);

        temp_array_cpy[ay] >>= (alaph ^ consts::MASK_ZZFZ) % consts::WORD_WIDTH_U16;
        temp_array_cpy[dee] >>= (gamal ^ consts::MASK_FZZZ) % ((mmax % 2) + 1);
        temp_array_cpy[thorn] ^= tools::log2n(dalath) & consts::MASK_ZFFF;
        temp_array_cpy[gee] ^= tools::log2n(teth) >> ((gamal % 2) + 1);

        temp_array_cpy
    }
}

mod round {
    use super::{alphabet, bits, consts, tools};

    macro_rules! swap {
        ($arr: ident, $indexof: ident, $indexwith: ident) => {{
            let tmp = $arr[$indexof];
            $arr[$indexof] = $arr[$indexwith];
            $arr[$indexwith] = tmp;
        }};
    }

    fn apply_alphabet_operation(temp_array: consts::ArrTypeRef) -> consts::ArrType {
        let mut temp_array_cpy = tools::copy_array(temp_array);
        temp_array_cpy = alphabet::alpha(&temp_array_cpy);
        temp_array_cpy = alphabet::delta(&temp_array_cpy);
        temp_array_cpy = alphabet::theta(&temp_array_cpy);
        temp_array_cpy = alphabet::gamma(&temp_array_cpy);
        temp_array_cpy
    }

    fn apply_prime(temp_array: consts::ArrTypeRef) -> consts::ArrType {
        let mut temp_array_cpy = tools::copy_array(temp_array);
        for i in 0..consts::POX_PRIME_NUM {
            temp_array_cpy[0] ^= consts::POX_PRIMES[i];
            temp_array_cpy[1] &= consts::POX_PRIMES[i];
            temp_array_cpy[2] ^= consts::POX_PRIMES[i];
            temp_array_cpy[3] &= consts::POX_PRIMES[i];
        }
        temp_array_cpy
    }

    fn apply_add_temp_to_facts(
        factor_array: consts::ArrTypeRef,
        temp_array: consts::ArrTypeRef,
    ) -> consts::ArrType {
        let mut factor_array_cpy = tools::copy_array(factor_array);
        factor_array_cpy[0] = bits::add_with_overflow(factor_array_cpy[0], temp_array[0]);
        factor_array_cpy[1] = bits::add_with_overflow(factor_array_cpy[1], temp_array[1]);
        factor_array_cpy[2] = bits::add_with_overflow(factor_array_cpy[2], temp_array[2]);
        factor_array_cpy[3] = bits::add_with_overflow(factor_array_cpy[3], temp_array[3]);
        factor_array_cpy
    }

    fn apply_shuffle(temp_array: consts::ArrTypeRef) -> consts::ArrType {
        let mut temp_array_cpy = tools::copy_array(temp_array);
        for i in 0..consts::SIZE_BIONOM {
            let (iof, iwith) = consts::COMB_BIONOM[i];
            swap!(temp_array_cpy, iof, iwith);
        }
        temp_array_cpy
    }

    pub fn one_round(factor_array: consts::ArrTypeRef) -> consts::ArrType {
        let mut temp_array: consts::ArrType = [
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

mod block {
    use super::{consts, round, tools};

    fn apply_bytes(factor_array: consts::ArrTypeRef, portion: &[u16]) -> consts::ArrType {
        let sum = tools::sum_portion(portion);
        let avg = sum / (consts::POX_PORTION_NUM as u16);
        let med = (sum + 1) / 2;
        let avg_odd_factor = consts::UINT16_MAX_U16 * (avg % 2);
        let med_odd_factor = consts::UINT16_MAX_U16 * (med % 2);

        let mut factor_array_cpy = tools::copy_array(factor_array);
        factor_array_cpy[0] ^= (portion[0] + avg) ^ med_odd_factor;
        factor_array_cpy[1] ^= (portion[1] + med) ^ avg_odd_factor;
        factor_array_cpy[2] ^= (portion[2] + avg) ^ med_odd_factor;
        factor_array_cpy[3] ^= (portion[3] + med) ^ avg_odd_factor;

        factor_array_cpy
    }

    pub fn process_block(factor_array: consts::ArrTypeRef, block: &[u16]) -> consts::ArrType {
        let mut factor_array_cpy = tools::copy_array(factor_array);
        for i in (0..consts::POX_BLOCK_NUM).step_by(consts::POX_CHUNK_NUM) {
            for j in (i..i + consts::POX_CHUNK_NUM).step_by(consts::POX_PORTION_NUM) {
                let portion: &[u16] = &[block[j], block[j + 1], block[j + 2], block[j + 3]];
                for _ in 0..consts::POX_ROUND_NUM {
                    factor_array_cpy = apply_bytes(&factor_array_cpy, portion);
                    factor_array_cpy = round::one_round(&factor_array_cpy);
                }
            }
        }
        factor_array_cpy
    }
}

pub struct PoxHashTy {
    pub hexdigest: String,
    pub bytes: [u8; 8],
    pub words: [u16; 4],
}

#[allow(unused_doc_comments)]
pub fn pox_hash(data: Vec<u8>) -> PoxHashTy {
    /// Converts the given data into a PoxHashTy object
    /// Parameters:
    ///     data: Vec<u8>
    ///
    /// Returns:
    ///     PoxHashTy
    ///         PoxHashTy.hexdigest: String
    ///         PoxHashTy.bytes: [u8; 8]
    ///         PoxHashTy.words: [u16; 4]
    let padded_u16 = convert::byte_vec_to_word_vec_and_pad(data);
    let mut factor_array: consts::ArrType = [
        consts::POX_PRIME_A,
        consts::POX_PRIME_B,
        consts::POX_PRIME_C,
        consts::POX_PRIME_D,
    ];

    for i in (0..padded_u16.len()).step_by(consts::POX_BLOCK_NUM) {
        factor_array =
            block::process_block(&factor_array, &padded_u16[i..i + consts::POX_BLOCK_NUM]);
    }

    let hexdigest = convert::word_array_to_hex_digest(&factor_array);
    let bytes_vec = convert::word_array_to_byte_array(&factor_array);
    let bytes: [u8; 8] = [
        bytes_vec[0],
        bytes_vec[1],
        bytes_vec[2],
        bytes_vec[3],
        bytes_vec[4],
        bytes_vec[5],
        bytes_vec[6],
        bytes_vec[7],
    ];
    let words: [u16; 4] = [
        factor_array[0],
        factor_array[1],
        factor_array[2],
        factor_array[3],
    ];

    PoxHashTy {
        hexdigest,
        bytes,
        words,
    }
}
