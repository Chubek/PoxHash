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
const FLAG_HELP: char = '?';
const FLAG_DASH: char = '-';
const FLAG_NHEADER: char = 'z';
const FLAG_ECHO: char = 'e';

const FILE_PREFIX: &'static str = "file=";
const FLE_PREFIX_LEN: usize = 5;

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
    ('3', '2'),
    ('5', '4'),
    ('6', '^'),
    ('7', '8'),
    ('9', '8'),
    ('0', '1'),
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

macro_rules! error_out {
    ($message: literal) => {{
        println!();
        print!($message);
        println!();
        print!("\x1b[1;31mError occurred\x1b[0m. Please pass \x1b[1;34m-?-\x1b[0m to show help\n");
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
    print!("If an argument stats with `{}`, it will lead to file read attempt, unles `{}` is passed\n", FILE_PREFIX, FLAG_JOIN);
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
                error_out!("Flag erreror");
            }
        }
    }
}

fn get_exec_name(argv0: &String) -> String {
    let argv0_split = argv0.split("/");
    argv0_split.last().unwrap().to_string()
}

fn arg_has_flag(flag_arg: &String, must_have: char) -> bool {
    flag_arg[0..flag_arg.len() - 1]
        .chars()
        .filter(|c| *c == must_have)
        .count()
        > 0
}

fn search_for_flag_reocurrance(flag_arg: &String) -> char {
    let count_benchmark = flag_arg.chars().filter(|c| *c == FLAG_BENCHMARK).count();
    if count_benchmark == 2 {
        return FLAG_BENCHMARK;
    } else if count_benchmark > 2 {
        error_out!("`^` can appear at most twice");
    }

    for ch in flag_arg.chars() {
        if flag_arg.chars().filter(|c| *c == ch).count() > 1 {
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
    let flag_arg = argv.get(1).unwrap();

    let len_flags = flag_arg.len();
    if len_flags < MIN_FLAG_SIZE || len_flags > MAX_FLAG_SIZE {
        error_out!("Length of the first argument must at least be 3 and at most 24");
    }

    if flag_arg.chars().next() != Some(FLAG_DASH) || flag_arg.chars().last() != Some(FLAG_DASH) {
        error_out!("The flag argument must begin and end with `-`");
    }

    check_for_wrong_flags(flag_arg);

    let exec_name = get_exec_name(exec);
    if flag_arg == "-?-" {
        print_help(exec_name);
    }

    let help_passed = arg_has_flag(flag_arg, FLAG_HELP);
    if help_passed && len_flags > MIN_FLAG_SIZE {
        error_out!("You may not pass the `?` flag along with other flags");
    }

    let reoccurrance = search_for_flag_reocurrance(&flag_arg[1..flag_arg.len() - 1].to_string());
    if reoccurrance != '\0' && reoccurrance != FLAG_BENCHMARK {
        print!("Flag `{}` appears twice", reoccurrance);
        error_out!("Only `^` can appear twice");
    }

    if num_argv < MIN_ARG_NUM {
        error_out!("You must pass at least one argument to hash");
    }

    let all_flags_passed = arg_has_flag(flag_arg, FLAG_EVERTHING);
    let all_flags_dec_passed = arg_has_flag(flag_arg, FLAG_ALL_DECIMAL);
    let all_flags_nondec_passed = arg_has_flag(flag_arg, FLAG_ALL_NON_DEC);

    for flag in flag_arg[1..len_flags - 1].chars() {
        match flag {
            FLAG_BENCHMARK | FLAG_JOIN | FLAG_NHEADER | FLAG_ECHO => continue,
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

fn get_time_in_us() -> u128 {
    std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .expect("Error getting time")
        .as_micros()
}

fn all_are_false(bools: Vec<bool>) -> bool {
    bools.into_iter().filter(|b| *b).count() == 0
}

fn print_hashes(hashes: &Vec<PoxDigest>, flags: &String, total_time: u128) {
    if arg_has_flag(flags, FLAG_BENCHMARK) {
        print!(
            "Total time for hashing {} unsigned bytearrays(s): {}us\n",
            hashes.len(),
            total_time
        );
    }
    let reoccurrance = search_for_flag_reocurrance(&flags[1..flags.len() - 1].to_string());
    if reoccurrance == FLAG_BENCHMARK {
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
    arg.len() > FLE_PREFIX_LEN && arg.starts_with(FILE_PREFIX)
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

fn process_arg(arg: &String) -> String {
    if !assert_file(arg) {
        return arg.clone();
    }
    read_given_file(&arg[FLE_PREFIX_LEN..].to_string())
}

#[allow(unused_assignments)]
fn main() {
    let argv: Vec<String> = std::env::args().collect();
    validate_flags(&argv);
    let flag_arg = argv.get(1).unwrap();

    if !arg_has_flag(flag_arg, FLAG_NHEADER) {
        print!(
            "\x1b[1;30;47mPoxHashRunner   |  Rust  |  March 2023 - Chubak Bidpaa  |  GPLv3  \x1b[0m\n"
        );
    }
    let echo_arg = arg_has_flag(flag_arg, FLAG_ECHO);
    let mut hashes = vec![PoxDigest::default(); argv.len() - 2];
    match arg_has_flag(&flag_arg, FLAG_JOIN) {
        true => {
            let args_joined = join_args(&argv[2..].to_vec());
            if echo_arg {
                print!("Joined Args: \n`{}`\n", args_joined);
            }
            let t1 = get_time_in_us();
            hashes[0] = pox_hash(&args_joined.as_bytes().to_vec());
            let t2 = get_time_in_us();
            print_hashes(&hashes[..1].to_vec(), &flag_arg, t2 - t1);
        }
        false => {
            let (mut t1, mut t2, mut total_time) = (0u128, 0u128, 0u128);
            for (i, arg) in argv[2..].into_iter().enumerate() {
                if echo_arg {
                    print!("Arg {}: {}\n", i + 1, arg);
                }
                let processed_arg = process_arg(arg).as_bytes().to_vec();
                t1 = get_time_in_us();
                hashes[i] = pox_hash(&processed_arg);
                t2 = get_time_in_us();
                total_time += t2 - t1;
            }
            print_hashes(&hashes, &flag_arg, total_time);
        }
    }
}
