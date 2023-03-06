extern crate libpox;
use libpox::pox_hash;

fn main() {
    let args: Vec<String> = std::env::args().collect();
    println!("Pox (Rust) hashes for passed strings in hexdigest form:");
    for (i, arg) in (&args[1..]).to_vec().into_iter().enumerate() {
        let hash = pox_hash(arg.as_bytes().to_vec());
        println!("\t{} -> {} for\n\t `{}`", i + 1, hash.hexdigest, arg);
    }
}