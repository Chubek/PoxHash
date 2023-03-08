const libpoxh = require("./libpoxh.js");

console.log("Pox (JS) hashes for passed strings in various forms:");
const args = process.argv.slice(2);
args.forEach((val, index) => {
    const hash = libpoxh.poxHash(val);
    const hexdigest = hash.hexdigest;
    const bytes = hash.bytes;
    const words = hash.words;
    const doubles = hash.doubles;
    const quad = hash.quad;
    console.log("\n");
    console.log(`\tArg #${index + 1} as follows`);
    console.log(`\t\thexdigest: ${hexdigest}`);
    console.log(`\t\tbytes: uint8(${bytes[0]}, ${bytes[1]}, ${bytes[2]}, ${bytes[3]}, ${bytes[4]}, ${bytes[5]}, ${bytes[6]}, ${bytes[7]})`);
    console.log(`\t\twords: uint16(${words[0]}, ${words[1]}, ${words[2]}, ${words[3]})`);
    console.log(`\t\tdoubles: uint32(${doubles[0]}, ${doubles[1]})`)
    console.log(`\t\tquad: uint64(${quad})`);
    console.log("\n");
});