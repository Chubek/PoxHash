mod consts {
    pub const POX_PRIMES: &'static [u16] = &[
        0xe537, 0xbd71, 0x9ef9, 0xbbcf, 0xf8dd, 0xceb7, 0xbaa1, 0x8f9f, 0xb0ed,
        0xfc4f, 0x9787, 0xf01f, 0xe1d1, 0xbcb9, 0xd565, 0xc011, 0xc1e1, 0xb58d,
        0xd4e1, 0x9ea1, 0xee49, 0x97cd, 0xdac9, 0xe257, 0xa32b, 0xafbb, 0xa5e3,
        0xfc43, 0xbf71, 0xe401, 0x8ebd, 0xd549
    ];
    pub const POX_8B_PRIMES: &'static [u16] = &[
        0x2, 0x3, 0x5, 0x7, 0xb, 0xd, 0x11, 0x13, 0x17, 0x1d, 0x1f, 0x25, 0x29,
        0x2b, 0x2f, 0x35, 0x3b, 0x3d, 0x43, 0x47, 0x49, 0x4f, 0x53, 0x59, 0x61,
        0x65, 0x67, 0x6b, 0x6d, 0x71, 0x7f, 0x83, 0x89, 0x8b, 0x95, 0x97, 0x9d,
        0xa3, 0xa7, 0xad, 0xb3, 0xb5, 0xbf, 0xc1, 0xc5, 0xc7, 0xd3, 0xdf, 0xe3,
        0xe5, 0xe9, 0xef, 0xf1, 0xfb
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
    pub const POX_PORTION_NUM: usize = 16;
    pub const POX_ROUND_NUM: usize = 8;
    pub const POX_FACT_NUM: usize = 4;
    pub const POX_NUM_SD_PRIME: u16 = 3;

    pub const UINT16_MAX_U32: u32 = 65535;
    pub const UINT16_MAX_U16: u16 = 65535;
    pub const WORD_WIDTH_U32: u32 = 16;
    pub const WORD_WIDTH_U16: u16 = 16;
    pub const BYTE_WIDTH_U16: u16 = 8;
    pub const BYTE_WIDTH_U8: u8 = 8;
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

    pub const COMB_BIONOM: &'static [(usize, usize)] = &[(0, 1), (0, 2), (0, 3), (1, 2), (1, 3), (2, 3)];
    pub const RANGE_ZTF: &'static [usize] = &[0, 1, 2, 3];
    pub const HEX_CHARS: &'static [char] = &[
        '0',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        'A',
        'B',
        'C',
        'D',
        'E',
        'F',];        
}

mod tools {
    use super::consts;

    pub fn sum_array<T: Copy + std::ops::AddAssign>(arr: &[T], size_arr: usize) -> T {
        let mut sum: T = arr[0];
        for i in 1..size_arr {
            sum += arr[i];
        }
        sum
    }

    pub fn max_and_argmax<T: std::cmp::PartialOrd + Copy>(arr: &[T], size_arr: usize) -> (T, usize) {
        let mut curr_max: T = arr[0];
        let mut curr_index = 0usize;
        for i in 1..size_arr {
            if arr[i] > curr_max {
                curr_max = arr[i];
                curr_index = i;
            }
        }
        (curr_max, curr_index)
    }

    pub fn min_and_argmin<T: std::cmp::PartialOrd + Copy>(arr: &[T], size_arr: usize) -> (T, usize) {
        let mut curr_min: T = arr[0];
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
        if num > 1 { return (1 + log2n(num / 2)); } else { return 0; }
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

    pub fn add_with_overflow(a: &mut u16, b: &u16) {
        let mut a_plus_b: u32 = (*a + *b) as u32;
        if a_plus_b > consts::UINT16_MAX_U32 {
            a_plus_b &= consts::ONE_LOWER16;
        }
        *a = a_plus_b as u16;
    }

    pub fn weighted_average(arr: &[u16], weights: &[u16]) -> u16 {
        let mut wavg = 0u32;
        for i in 0..consts::POX_FACT_NUM {
            wavg += (arr[i] * weights[i]) as u32;
        }
        wavg /= consts::POX_FACT_NUM as u32;
        if wavg > consts::UINT16_MAX_U32 {
            wavg = (wavg & consts::ONE_UPPER16) >> consts::WORD_WIDTH_U32;
        }
        wavg as u16
    }

    pub fn weighted_median(arr: &[u16], weights: &[u16]) -> u16 {
        let mut wmed = 0u32;
        for i in 0..consts::POX_FACT_NUM {
            wmed += (arr[i] * weights[i]) as u32;
        }
        wmed /= 2;
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
        let mut hex = vec!['\0'; consts::HEX_SIZE];
        let mut dec_mut = dec;
        hex
            .iter_mut()
            .rev()
            .for_each(|c| {
                *c = consts::HEX_CHARS[(dec_mut % consts::WORD_WIDTH_U16) as usize];
                dec_mut /= consts::WORD_WIDTH_U16;
            });
        String::from_iter(hex.into_iter())
    }
    
    pub fn word_array_to_byte_array(word_array: &[u16]) -> Vec<u8> {
        let (b0, b1) = single_word_to_byte(word_array[0]);
        let (b2, b3) = single_word_to_byte(word_array[1]);
        let (b4, b5) = single_word_to_byte(word_array[2]);
        let (b6, b7) = single_word_to_byte(word_array[3]);
        vec![b0, b1, b2, b3, b4, b5, b6, b7]
    }

    pub fn word_array_to_hex_digest(word_array: &[u16]) -> String {
        let hex_a = decimal_to_hex(word_array[0]);
        let hex_b = decimal_to_hex(word_array[1]);
        let hex_c = decimal_to_hex(word_array[2]);
        let hex_d = decimal_to_hex(word_array[3]);
        format!("{}{}{}{}", hex_a, hex_b, hex_c, hex_d)
    }

    pub fn byte_vec_to_word_vec_and_pad(byte_array: Vec<u8>) -> Vec<u16> {
        let mut word_vec = byte_array.into_iter().map(|b| b as u16).collect::<Vec<u16>>();
        while word_vec.len() % consts::POX_BLOCK_NUM != 0 {
            word_vec.push(0);
        }
        word_vec
    }
}

mod alphabet {
    use super::{consts, tools, bits};

    pub fn alpha(temp_array_ptr: *mut &mut [u16]) {
        let temp_array = unsafe { temp_array_ptr.as_ref().unwrap() };

        let aleph: u16 = (temp_array[0] ^ temp_array[1]) & consts::MASK_ZZFF;
        let theh: u16 = (temp_array[2] ^ temp_array[3]) & consts::MASK_FFZZ;
        let daal: u16 = (aleph | theh) % consts::POX_8B_PRIMES[0];
        let gaaf: u16 = (aleph ^ theh) % consts::POX_8B_PRIMES[1];

        unsafe {
            (*temp_array_ptr)[0] >>= daal;
            (*temp_array_ptr)[1] >>= ((daal + gaaf) % 2) + 1;
            (*temp_array_ptr)[2] >>= gaaf;
        }
    }
    
    pub fn delta(temp_array_ptr: *mut &mut [u16]) {
        let temp_array = unsafe { temp_array_ptr.as_ref().unwrap() };

        let mut alaf: u16 = (temp_array[0] ^ consts::MASK_FFFZ) % tools::get_8b_prime(temp_array[0]);
        let mut dalat: u16 = (temp_array[1] ^ consts::MASK_FZZF) % tools::get_8b_prime(temp_array[1]);
        let mut tit: u16 = (temp_array[2] & consts::MASK_ZFFF) % tools::get_8b_prime(temp_array[2]);
        let mut gaman: u16 = (temp_array[3] & consts::MASK_FFZZ) % tools::get_8b_prime(temp_array[3]);

        for _ in 0..consts::POX_FACT_NUM {
            alaf >>= consts::POX_SINGLE_DIGIT_PRIMES[(dalat % consts::POX_NUM_SD_PRIME) as usize];
            dalat = bits::rotate_left(dalat, 2);
            tit >>= consts::POX_SINGLE_DIGIT_PRIMES[(gaman % consts::POX_NUM_SD_PRIME) as usize];
            gaman ^= (alaf ^ consts::MASK_ZZFF) >> consts::POX_SINGLE_DIGIT_PRIMES[(tit % consts::POX_NUM_SD_PRIME) as usize];
        }

        unsafe {
            (*temp_array_ptr)[1] ^= temp_array[2] % consts::POX_MAGIC_PRIMES[1];
            (*temp_array_ptr)[2] ^= alaf + tit;
            (*temp_array_ptr)[3] ^= tit + gaman;
        }        
    }

    pub fn theta(temp_array_ptr: *mut &mut [u16]) {
        let temp_array = unsafe { temp_array_ptr.as_ref().unwrap() };

        let alef: u16 = temp_array[0] % 2;
        let dalet: u16 = temp_array[1] % 2;
        let tet: u16 = temp_array[2] % 2;
        let gimmel: u16 = temp_array[3] % 2;

        let weighted_avg: u16 = bits::weighted_average(&temp_array, &[alef, dalet, tet, gimmel]);
        let weighted_med: u16 = bits::weighted_median(&temp_array, &[alef, dalet, tet, gimmel]);

        unsafe {
            (*temp_array_ptr)[0] ^= ((weighted_avg >> gimmel) ^ consts::MASK_ZZFF) & consts::MASK_ZZZF;
            (*temp_array_ptr)[3] ^= ((weighted_med << alef) ^ consts::MASK_FZFZ) & consts::MASK_FZZZ;
        }        
    }

    pub fn gamma(temp_array_ptr: *mut &mut [u16]) {
        let temp_array = unsafe { temp_array_ptr.as_ref().unwrap() };

        let (mmin, argmin) = tools::min_and_argmin(&temp_array, consts::POX_FACT_NUM);
        let (mmax, argmax) = tools::max_and_argmax(&temp_array, consts::POX_FACT_NUM);
        let (aside, beside) = {
            let mut aside_beside: &mut [usize] = &mut [0, 0];
            let mut j = 0usize;
            for i in 0..consts::POX_FACT_NUM {
                if consts::RANGE_ZTF[i] != argmin && consts::RANGE_ZTF[i] != argmax {
                    aside_beside[j] = consts::RANGE_ZTF[i];
                    j += 1;
                }
            }
            (aside_beside[0], aside_beside[1])
        };

        let alaph: u16 = temp_array[aside] % tools::get_8b_prime(temp_array[aside]);
        let dalath: u16 = (tools::get_8b_prime(mmax) ^ consts::MASK_ZFZF) % tools::get_8b_prime(mmin);
        let teth: u16 = mmax % tools::get_8b_prime(mmax);
        let gamal: u16 = temp_array[beside] % tools::get_8b_prime((mmin + mmax) / 2);

        unsafe {
            (*temp_array_ptr)[aside] >>= (alaph ^ consts::MASK_ZZFZ) % consts::WORD_WIDTH_U16;
            (*temp_array_ptr)[argmin] >>= (gamal ^ consts::MASK_FZZZ) % ((mmax % 2) + 1);
            (*temp_array_ptr)[argmax] ^= tools::log2n(dalath) & consts::MASK_ZFFF;
            (*temp_array_ptr)[beside] ^= tools::log2n(teth) >> ((gamal % 2) + 1);
        }        
    }
}
