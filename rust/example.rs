extern crate libpoxh;
use libpoxh::pox_hash;

fn main() {
    let args: Vec<String> = std::env::args().collect();
    println!("Pox (Rust) hashes for passed strings in various forms:");
    for (i, arg) in (&args[1..]).to_vec().into_iter().enumerate() {
        let hash = pox_hash(arg.as_bytes().to_vec());
        let sexdigest = hash.sexdigest;
        let hexdigest = hash.hexdigest;
        let duodigest = hash.duodigest;
        let octdigest = hash.octdigest;
        let bindigest = hash.bindigest;
        let bytes = hash.bytes;
        let words = hash.words;
        let doubles = hash.doubles;
        let quad = hash.quad;
        println!("\n");
        println!("\tArg #{i} as follows");
        println!("\t\tsexdigest: {sexdigest}");
        println!("\t\thexdigest: {hexdigest}");
        println!("\t\tduodigest: {duodigest}");
        println!("\t\toctdigest: {octdigest}");
        println!("\t\tbindigest: {bindigest}");
        println!("\t\tbytes: uint8({}, {}, {}, {}, {}, {}, {}, {})", bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5], bytes[6], bytes[7]);
        println!("\t\twords: uint16({}, {}, {}, {})", words[0], words[1], words[2], words[3],);
        println!("\t\tdouble: uint32({}, {})", doubles[0], doubles[1]);
        println!("\t\tquad: uint64({})", quad);
        println!("\n")
    }
}