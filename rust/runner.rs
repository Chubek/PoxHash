/////////////////////////////////////////////////////////////////////////////////////
//                       Runner for libpoxh.rs (PoxHash Rust)                      //
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


extern crate libpoxh;
use libpoxh::{pox_hash, PoxDigest};

const MAX_FLAG_SIZE: usize = 24;
const MIN_FLAG_SIZE: usize = 3;
const MIN_ARG_NUM: usize = 2;

const FLAG_BENCHMARK: char = '^';
const FLAG_JOIN: char = '+';
const FLAG_EVERTHING: char = '*';
const FLAG_ALL_NON_DEC: char = 'N';
const FLAG_ALL_DECIMAL: char = 'D';
const FLAG_BYTES: char = '8';
const FLAG_WORDS: char = '4';
const FLAG_DOUBLES: char = '2';
const FLAG_QUAD: char = '1';
const FLAG_SEX: char = 'g';
const FLAG_VIG: char = 'v';
const FLAG_HEX: char = 'h';
const FLAG_TET: char = 't';
const FLAG_DUO: char = 'd';
const FLAG_OCT: char = 'o';
const FLAG_SEN: char = 's';
const FLAG_BIN: char = 'b';
const FLAG_NS: char = '9';
const FLAG_US: char = '6';
const FLAG_MS: char = '3';
const FLAG_SS: char = '5';
const FLAG_MM: char = '0';
const FLAG_HELP: char = '?';
const FLAG_DASH: char = '-';
const FLAG_NHEADER: char = 'z';
const FLAG_ECHO: char = 'e';

const FILE_PREFIX: &'static str = "file=";
const FILE_PREFIX_LEN: usize = 5;

const INT_PREFIX: &'static str = "int=";
const INT_PREFIX_LEN: usize = 4;

const MAX_BIN: usize = 8;
const MAX_OCT: usize = 5;
const MAX_HEX: usize = 2;

const HEX_PREFIX: &'static str = "0x";
const BIN_PREFIX: &'static str = "0b";
const OCT_PREFIX: &'static str = "0o";
const BASE_PREFIX_NUM: usize = 2;

const NS_TO_NS: u128 = 100;
const NS_TO_US: u128 = 1000;
const NS_TO_MS: u128 = 1000000;
const NS_TO_SS: u128 = 1000000000;
const NS_TO_MM: u128 = 60000000000;

const E_NOT_TRUNC_LEN: usize = 4;

const WRONG_FLAGS: &'static [(char, char)] = &[
    ('G', 'g'),
    ('V', 'v'),
    ('O', 'o'),
    ('T', 't'),
    ('S', 's'),
    ('H', 'h'),
    ('n', 'N'),
    ('W', '4'),
    ('w', '4'),
    ('q', '1'),
    ('Q', '1'),
    ('/', '?'),
    ('=', '+'),
    ('B', 'b'),
    ('E', '*'),
    ('A', '*'),
    ('>', '?'),
    ('&', '*'),
    ('r', 't'),
    ('y', 't'),
    ('f', 'g'),
    ('x', 'z'),
    ('Z', 'z'),
    ('a', 'z'),
    ('E', 'e'),
    ('w', 'e'),
    ('r', 'e'),
    ('i', 'e'),
];


fn to_e_notation(num_in: f64, places: usize) -> String {
    let num = num_in.abs();
    if num > 1.0 {
        let num_str = num.to_string();
        let index_of_period = num_str.find('.').unwrap();
        let e = index_of_period - 1;
        let first_digit = num_str.chars().nth(0usize).unwrap();
        let mut truncs = String::new();
        let str_sub = &num_str[1..places + 1].to_string();
        for c in str_sub.chars() {
            if c == '.' {
                continue;
            }
            truncs.push(c);
        }
        
        if truncs.len() < places {
            (truncs.len()..places).into_iter().for_each(|_| truncs.push('0'));
        }

        let e_str = if e > 9 { e.to_string() } else { format!("0{}", e) };
        format!("{}.{}e+{}", first_digit, truncs, e_str)
    } else if num > 0.0 && num < 1.0 {
        let num_str = num.to_string();
        let mut first_non_zero_index = 0;
        let mut truncs = String::new();
        let mut first_digit = '\0';
        for (i, c) in num_str.char_indices() {
            if c != '0' && c  != '.' && first_non_zero_index == 0 {
                first_non_zero_index = i;
                first_digit = c;
                continue;
            }
            if first_non_zero_index != 0 && truncs.len() < places {
                truncs.push(c);
            }
        }

        if truncs.len() < places {
            (truncs.len()..places).into_iter().for_each(|_| truncs.push('0'));
        }

        let e = first_non_zero_index - 1;            
        let e_str = if e > 9 { e.to_string() } else { format!("0{}", e) };
        format!("{}.{}e-{}", first_digit, truncs, e_str)
    } else {
        num.to_string()
    }
}


macro_rules! error_out {
    ($message: literal) => {{
        eprintln!();
        eprint!($message);
        eprintln!();
        eprint!("\x1b[1;31mError occurred\x1b[0m. Please pass \x1b[1;34m-?-\x1b[0m to show help\n");
        std::process::exit(1);
    }};
}

fn print_help(exec: String) {
    print!("\x1b[1;30;42mHelp | Chubak#7400 (Discord) | @bidpaafx (Telegram) | Chubakbidpaa[at]gmail[dot]com\x1b[0m\n");
    println!();
    print!("Examples \x1b[1m(flags go between two dashes!)\x1b[0m:\n");
    print!("{} -N82- myword1\n", exec);
    print!("{} -*+^- mywod to be joined\n", exec);
    print!("{} -Dhob- word1 word 2\n", exec);
    print!("{} -^^+- large seq  to join and  benchmark\n", exec);
    print!(
        "wget -qO- www.example.com | xargs bash -c '{} -h+- $@'\n",
        exec
    );
    print!("If an argument stats with `{}`, it will lead to file read attempt, unless `{}` is passed\n", FILE_PREFIX, FLAG_JOIN);
    print!("If an argument stats with `{}`, it will parse the int, prefixes 0b, 0o and 0x for bin, oct and hex and none for decimal apply\n", INT_PREFIX);
    println!();
    print!("\x1b[1;32mFlags:\x1b[0m\n");
    print!("\x1b[1;33m\t`{}`\x1b[0m: Echo argument\n", FLAG_ECHO);
    print!(
        "\x1b[1;33m\t`{}`\x1b[0m: Don't print header message\n",
        FLAG_NHEADER
    );
    print!(
        "\x1b[1;33m\t`{}`\x1b[0m: Benchmark run (pass two to only show benchmark)\n",
        FLAG_BENCHMARK
    );
    print!(
        "\x1b[1;33m\t`{}`\x1b[0m: Join arguments with space (byte 32)\n",
        FLAG_JOIN
    );
    print!(
        "\x1b[1;33m\t`{}`\x1b[0m: Print every digest\n",
        FLAG_EVERTHING
    );
    print!(
        "\x1b[1;33m\t`{}`\x1b[0m: Print every non-decimal digest\n",
        FLAG_ALL_NON_DEC
    );
    print!(
        "\x1b[1;33m\t`{}`\x1b[0m: Print every decimal digest\n",
        FLAG_ALL_DECIMAL
    );
    print!(
        "\x1b[1;33m\t`{}`\x1b[0m: Print bytes digest (eight unsigned 8-bit integers)\n",
        FLAG_BYTES
    );
    print!(
        "\x1b[1;33m\t`{}`\x1b[0m: Print words digest (four unsigned 16-bit integers)\n",
        FLAG_WORDS
    );
    print!(
        "\x1b[1;33m\t`{}`\x1b[0m: Print doubles digest (two unsigned 32-bit integers)\n",
        FLAG_DOUBLES
    );
    print!(
        "\x1b[1;33m\t`{}`\x1b[0m: Print quad digest (one unsigned 64-bit integer)\n",
        FLAG_QUAD
    );
    print!(
        "\x1b[1;33m\t`{}`\x1b[0m: Print sexagesimal digest (base sixty)\n",
        FLAG_SEX
    );
    print!(
        "\x1b[1;33m\t`{}`\x1b[0m: Print vigesimal digest (base twenty)\n",
        FLAG_VIG
    );
    print!(
        "\x1b[1;33m\t`{}`\x1b[0m: Print hexadecimal digest (base sixteen)\n",
        FLAG_HEX
    );
    print!(
        "\x1b[1;33m\t`{}`\x1b[0m: Print tetradecimal digest (base fourteen)\n",
        FLAG_TET
    );
    print!(
        "\x1b[1;33m\t`{}`\x1b[0m: Print duodecimal digest (base twelve)\n",
        FLAG_DUO
    );
    print!(
        "\x1b[1;33m\t`{}`\x1b[0m: Print octal digest (base eight)\n",
        FLAG_OCT
    );
    print!(
        "\x1b[1;33m\t`{}`\x1b[0m: Print senary digest (base six)\n",
        FLAG_SEN
    );
    print!(
        "\x1b[1;33m\t`{}`\x1b[0m: Print binary digest (base two)\n",
        FLAG_BIN
    );
    print!("\x1b[1;33m\t`{}`\x1b[0m: Print total time in nanoseconds\n", FLAG_NS);
    print!("\x1b[1;33m\t`{}`\x1b[0m: Print total time in mictoseconds\n", FLAG_US);
    print!("\x1b[1;33m\t`{}`\x1b[0m: Print total time in milliseconds\n", FLAG_MS);
    print!("\x1b[1;33m\t`{}`\x1b[0m: Print total time in seconds\n", FLAG_SS);
    print!("\x1b[1;33m\t`{}`\x1b[0m: Print total time in minutes\n", FLAG_MM);
    print!("\x1b[1;33m\t`{}`\x1b[0m: Print Help\n\n", FLAG_HELP);
    std::process::exit(1);
}

fn check_for_wrong_flags(flags: &String) {
    for flag in flags.chars() {
        for (wrong_flag, right_flag) in WRONG_FLAGS {
            if flag == *wrong_flag {
                print!(
                    "No flag for `{}`, perhaps you meant `{}`?",
                    flag, right_flag
                );
                error_out!("Flag errror");
            }
        }
    }
}

fn get_exec_name(argv0: &String) -> String {
    let argv0_split = argv0.split("/");
    argv0_split.last().unwrap().to_string()
}

fn arg_has_flag(flags_arg: &String, must_have: char) -> bool {
    flags_arg[0..flags_arg.len() - 1]
        .chars()
        .filter(|c| *c == must_have)
        .count()
        > 0
}

fn search_for_flag_reocurrance(flags_arg: &String) -> char {
    let count_benchmark = flags_arg.chars().filter(|c| *c == FLAG_BENCHMARK).count();
    if count_benchmark == 2 {
        return FLAG_BENCHMARK;
    } else if count_benchmark > 2 {
        error_out!("`^` can appear at most twice");
    }

    for ch in flags_arg.chars() {
        if flags_arg.chars().filter(|c| *c == ch).count() > 1 {
            return ch;
        }
    }

    '\0'
}

fn validate_flags(argv: &Vec<String>) {
    let num_argv = argv.len();

    if num_argv < MIN_ARG_NUM - 1 {
        error_out!("No flags passed");
    }

    let exec = argv.get(0).unwrap();
    let flags_arg = argv.get(1).unwrap();

    let len_flags = flags_arg.len();
    if len_flags < MIN_FLAG_SIZE || len_flags > MAX_FLAG_SIZE {
        error_out!("Length of the first argument must at least be 3 and at most 24");
    }

    if flags_arg.chars().next() != Some(FLAG_DASH) || flags_arg.chars().last() != Some(FLAG_DASH) {
        error_out!("The flag argument must begin and end with `-`");
    }

    check_for_wrong_flags(flags_arg);

    let exec_name = get_exec_name(exec);
    if flags_arg == "-?-" {
        print_help(exec_name);
    }

    let help_passed = arg_has_flag(flags_arg, FLAG_HELP);
    if help_passed && len_flags > MIN_FLAG_SIZE {
        error_out!("You may not pass the `?` flag along with other flags");
    }

    let reoccurrance = search_for_flag_reocurrance(&flags_arg[1..flags_arg.len() - 1].to_string());
    if reoccurrance != '\0' && reoccurrance != FLAG_BENCHMARK {
        print!("Flag `{}` appears twice", reoccurrance);
        error_out!("Only `^` can appear twice");
    }
    let double_benchmark = reoccurrance == FLAG_BENCHMARK;

    if num_argv < MIN_ARG_NUM {
        error_out!("You must pass at least one argument to hash");
    }

    let all_flags_passed = arg_has_flag(flags_arg, FLAG_EVERTHING);
    let all_flags_dec_passed = arg_has_flag(flags_arg, FLAG_ALL_DECIMAL);
    let all_flags_nondec_passed = arg_has_flag(flags_arg, FLAG_ALL_NON_DEC);
    let benchmark_has_passed = arg_has_flag(flags_arg, FLAG_BENCHMARK);

    for flag in flags_arg[1..len_flags - 1].chars() {
        match flag {
            FLAG_BENCHMARK | FLAG_JOIN | FLAG_NHEADER | FLAG_ECHO => continue,
            FLAG_NS | FLAG_US | FLAG_MS | FLAG_SS | FLAG_MM => {
                if !benchmark_has_passed {
                    error_out!("When a timestamp flag has passed, `^` must be passed as well");
                } else if double_benchmark {
                    error_out!("When double benchmark (`^^`) is passed, you may not pass a timestamp flag")
                }
                continue;
            }
            FLAG_EVERTHING => {
                if all_flags_dec_passed || all_flags_nondec_passed{
                    error_out!("You may not pass `*` when you have passed `N` or `D`");
                }
                continue;
            },
            FLAG_ALL_NON_DEC => {
                if all_flags_passed {
                    error_out!("You may not pass `N` when `*` is passed");
                }
                continue;
            },
            FLAG_ALL_DECIMAL => {
                if all_flags_passed {
                    error_out!("You may not pass `D` when `*` is passed");
                }
                continue;
            },
            FLAG_BYTES => {
                if all_flags_dec_passed || all_flags_passed {
                    error_out!("You may not pass a decimal digest flag when `*` or `D` is passed");
                }
                continue;
            },
            FLAG_WORDS => {
                if all_flags_dec_passed || all_flags_passed {
                    error_out!("You may not pass a decimal digest flag when `*` or `D` is passed");
                }
                continue;
            },
            FLAG_DOUBLES => {
                if all_flags_dec_passed || all_flags_passed  {
                    error_out!("You may not pass a decimal digest flag when `*` or `D` is passed");
                }
                continue;
            },
            FLAG_QUAD => {
                if all_flags_dec_passed || all_flags_passed  {
                    error_out!("You may not pass a decimal digest flag when `*` or `D` is passed");
                }
                continue;
            },
            FLAG_SEX => {
                if all_flags_nondec_passed || all_flags_passed {
                    error_out!("You may not pass a non-decimal digest flag when `*` or `N` is passed");
                }
                continue;
            },
            FLAG_VIG => {
                if all_flags_nondec_passed || all_flags_passed {
                    error_out!("You may not pass a non-decimal digest flag when `*` or `N` is passed");
                }
                continue;
            },
            FLAG_HEX => {
                if all_flags_nondec_passed || all_flags_passed {
                    error_out!("You may not pass a non-decimal digest flag when `*` or `N` is passed");
                }
                continue;
            },
            FLAG_TET => {
                if all_flags_nondec_passed || all_flags_passed {
                    error_out!("You may not pass a non-decimal digest flag when `*` or `N` is passed");
                }
                continue;
            },
            FLAG_DUO => {
                if all_flags_nondec_passed || all_flags_passed {
                    error_out!("You may not pass a non-decimal digest flag when `*` or `N` is passed");
                }
                continue;
            },
            FLAG_OCT => {
                if all_flags_nondec_passed || all_flags_passed {
                    error_out!("You may not pass a non-decimal digest flag when `*` or `N` is passed");
                }
                continue;
            },
            FLAG_SEN => {
                if all_flags_nondec_passed || all_flags_passed {
                    error_out!("You may not pass a non-decimal digest flag when `*` or `N` is passed");
                }
                continue;
            },
            FLAG_BIN => {
                if all_flags_nondec_passed || all_flags_passed {
                    error_out!("You may not pass a non-decimal digest flag when `*` or `N` is passed");
                }
                continue;
            },
            FLAG_HELP => {
                if len_flags > MIN_FLAG_SIZE {
                    error_out!("You may not pass the `?` flag along with other flags");
                }
            },
            FLAG_DASH => error_out!("You may not use `-` in the first argument other than in the first, and the last letter"),
            _ => error_out!("Unknown flag detected!"),
        }
    }
}

fn get_time_in_ns() -> u128 {
    std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .expect("Error getting time")
        .as_nanos()
}

fn convert_time(time: u128, divisor: u128) -> String {
    to_e_notation(time as f64 / divisor as f64, E_NOT_TRUNC_LEN)
}


fn all_are_false(bools: Vec<bool>) -> bool {
    bools.into_iter().filter(|b| *b).count() == 0
}

fn print_hashes(hashes: &Vec<PoxDigest>, flags: &String, total_time: u128) {
    let reoccurrance = search_for_flag_reocurrance(&flags[1..flags.len() - 1].to_string());
    let double_benchmark = reoccurrance == FLAG_BENCHMARK;
    
    if arg_has_flag(flags, FLAG_BENCHMARK) {
        print!("| {} Message(s) ||", hashes.len());
        let mut has_printed = false;
        if arg_has_flag(flags, FLAG_NS) || double_benchmark {
            print!(" {}ns |", convert_time(total_time, NS_TO_NS));
            has_printed = true;
        }
        if arg_has_flag(flags, FLAG_US) || double_benchmark {
            print!(" {}us |", convert_time(total_time, NS_TO_US));
            has_printed = true;
        }
        if arg_has_flag(flags, FLAG_MS) || double_benchmark {
            print!(" {}ms |", convert_time(total_time, NS_TO_MS));
            has_printed = true;
        }
        if arg_has_flag(flags, FLAG_SS) || double_benchmark {
            print!(" {}s |", convert_time(total_time, NS_TO_SS));
            has_printed = true;
        }
        if arg_has_flag(flags, FLAG_MM) || double_benchmark {
            print!(" {}m |", convert_time(total_time, NS_TO_MM));
            has_printed = true;
        }
        if !has_printed {
            print!(" {}us |", convert_time(total_time, NS_TO_US));
        }
        println!();
    }

    if double_benchmark {
        println!();
        std::process::exit(0);
    }

    let everything = arg_has_flag(flags, FLAG_EVERTHING);
    let all_flags_decimal = arg_has_flag(flags, FLAG_ALL_DECIMAL);
    let all_flags_non_decimal = arg_has_flag(flags, FLAG_ALL_NON_DEC);
    let byte = arg_has_flag(flags, FLAG_BYTES);
    let word = arg_has_flag(flags, FLAG_WORDS);
    let dub = arg_has_flag(flags, FLAG_DOUBLES);
    let quad = arg_has_flag(flags, FLAG_QUAD);
    let sex = arg_has_flag(flags, FLAG_SEX);
    let vig = arg_has_flag(flags, FLAG_VIG);
    let hex = arg_has_flag(flags, FLAG_HEX);
    let tet = arg_has_flag(flags, FLAG_TET);
    let duo = arg_has_flag(flags, FLAG_DUO);
    let oct = arg_has_flag(flags, FLAG_OCT);
    let sen = arg_has_flag(flags, FLAG_SEN);
    let bin = arg_has_flag(flags, FLAG_BIN);

    let all_false = all_are_false(vec![
        everything,
        all_flags_decimal,
        all_flags_non_decimal,
        byte,
        word,
        dub,
        quad,
        sex,
        vig,
        hex,
        tet,
        duo,
        oct,
        sen,
        bin,
    ]);

    if all_false {
        print!("You had not specfied any digests to be printed\n");
        std::process::exit(0);
    }

    for hash in hashes.into_iter() {
        if everything || all_flags_decimal || byte {
            print!(
                "Bytes: U8[{}, {}, {}, {}, {}, {}, {}, {}]\n",
                hash.bytes[0],
                hash.bytes[1],
                hash.bytes[2],
                hash.bytes[3],
                hash.bytes[4],
                hash.bytes[5],
                hash.bytes[6],
                hash.bytes[7]
            );
        }
        if everything || all_flags_decimal || word {
            print!(
                "Words: U16[{}, {}, {}, {}]\n",
                hash.words[0], hash.words[1], hash.words[2], hash.words[3]
            );
        }
        if everything || all_flags_decimal || dub {
            print!("Doubles: U32[{}, {}]\n", hash.doubles[0], hash.doubles[1]);
        }
        if everything || all_flags_decimal || quad {
            print!("Quad: U64[{}]\n", hash.quad);
        }
        if everything || all_flags_non_decimal || sex {
            print!("Sexdigest: {}\n", hash.sexdigest);
        }
        if everything || all_flags_non_decimal || vig {
            print!("Vigdigest: {}\n", hash.vigdigest);
        }
        if everything || all_flags_non_decimal || hex {
            print!("Hexdigest: {}\n", hash.hexdigest);
        }
        if everything || all_flags_non_decimal || tet {
            print!("Tetdigest: {}\n", hash.tetdigest);
        }
        if everything || all_flags_non_decimal || duo {
            print!("Duodigest: {}\n", hash.duodigest);
        }
        if everything || all_flags_non_decimal || oct {
            print!("Octdigest: {}\n", hash.octdigest);
        }
        if everything || all_flags_non_decimal || sen {
            print!("Sendgiest: {}\n", hash.sendigest);
        }
        if everything || all_flags_non_decimal || bin {
            print!("Bindigest: {}\n", hash.bindigest);
        }
        print!("----\n");
    }
}

fn assert_file(arg: &String) -> bool {
    arg.len() > FILE_PREFIX_LEN && arg.starts_with(FILE_PREFIX)
}

fn assert_int(arg: &String) -> bool {
    arg.len() > INT_PREFIX_LEN && arg.starts_with(INT_PREFIX)
}

fn to_int(arg: &String) -> Vec<u8> {
    arg.split(",")
        .map(|n| {
            let sans_prefix = &n[BASE_PREFIX_NUM..];
            match &n[..BASE_PREFIX_NUM] {
                BIN_PREFIX => {
                    if sans_prefix.len() > MAX_BIN {
                        error_out!("Size of binary number should not exceed 8");
                    }
                    
                    u8::from_str_radix(sans_prefix, 2).expect("Bad binary number")
                },
                OCT_PREFIX => {
                    if sans_prefix.len() > MAX_OCT {
                        error_out!("Size of octal number should not exceed 5");
                    }

                    u8::from_str_radix(sans_prefix, 8).expect("Bad octal number")
                },
                HEX_PREFIX => {
                    if sans_prefix.len() > MAX_HEX {
                        error_out!("Size of hexadecimal number should not exceed 2");
                    }

                    u8::from_str_radix(sans_prefix, 16).expect("Bad hexadecimal number")
                },
                _ => {
                    if n.to_string().chars().any(|c| !c.is_numeric()) {
                        error_out!("With 'int=' prefix you must pass byte-sized integers in base 16, 8, 10 and 2");
                    }

                    u8::from_str_radix(n, 10).expect("Given integer must be byte-sized (0-255)")
                }
            }
        })
        .collect()
}

fn join_args(argv: &Vec<String>) -> String {
    let mut joined = String::new();
    let mut warned = false;
    for arg in argv {
        if assert_file(&arg) && !warned {
            print!("\x1b[1;33mWarning:\x1b[0m: The `filepath=` prefix is ignored in join mode\n");
            warned = true;
        }
        joined.push_str(&arg);
        joined.push(' ');
    }
    joined.trim().to_string()
}

fn is_regular_file(fpath: &String) {
    let path = std::path::PathBuf::from(fpath);
    if !path.exists() || path.is_dir() {
        error_out!("Specfied file does not exist or is a directory. Pass `+` with only one argument to ignore");
    }
}

fn read_given_file(fpath: &String) -> String {
    is_regular_file(fpath);
    std::fs::read_to_string(fpath).expect("Unkown error occrurred reading file")
}

fn process_arg(arg: &String) -> Vec<u8> {
    if !assert_file(arg) && !assert_int(arg) {
        return arg.clone().as_bytes().to_vec();
    } else if assert_int(arg) {
        return to_int(&arg[INT_PREFIX_LEN..].to_string());
    }
    read_given_file(&arg[FILE_PREFIX_LEN..].to_string()).as_bytes().to_vec()
}

#[allow(unused_assignments)]
fn main() {
    let argv: Vec<String> = std::env::args().collect();
    validate_flags(&argv);
    let flags_arg = argv.get(1).unwrap();

    if !arg_has_flag(flags_arg, FLAG_NHEADER) {
        print!(
            "\x1b[1;30;47m   PoxHashRunner   |  Rust  |  March 2023 - Chubak Bidpaa  |  MIT  \x1b[0m\n"
        );
    }
    let echo_arg = arg_has_flag(flags_arg, FLAG_ECHO);
    let mut hashes = vec![PoxDigest::default(); argv.len() - 2];
    match arg_has_flag(&flags_arg, FLAG_JOIN) {
        true => {
            let args_joined = join_args(&argv[2..].to_vec());
            if echo_arg {
                print!("Joined Args: \n`{}`\n", args_joined);
            }
            let t1 = get_time_in_ns();
            hashes[0] = pox_hash(&args_joined.as_bytes().to_vec());
            let t2 = get_time_in_ns();
            print_hashes(&hashes[..1].to_vec(), &flags_arg, t2 - t1);
        }
        false => {
            let (mut t1, mut t2, mut total_time) = (0u128, 0u128, 0u128);
            for (i, arg) in argv[2..].into_iter().enumerate() {
                if echo_arg {
                    print!("Arg {}: {}\n", i + 1, arg);
                }
                let processed_arg = process_arg(arg);
                t1 = get_time_in_ns();
                hashes[i] = pox_hash(&processed_arg);
                t2 = get_time_in_ns();
                total_time += t2 - t1;
            }
            print_hashes(&hashes, &flags_arg, total_time);
        }
    }
}
