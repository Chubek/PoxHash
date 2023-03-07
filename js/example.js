const libpoxh = require("./libpoxh.js");

console.log("Pox (JS) hashes for passed strings in hexdigest, byte and word form:");
const args = process.argv.slice(2);
args.forEach((val, index) => {
    const hash = libpoxh.poxHash(val);
    const hexdigest = hash.hexdigest;
    const bytes = hash.bytes;
    const words = hash.words;
    console.log("\n");
    console.log(`\tArg #${index + 1} as follows`);
    console.log(`\t\thexdigest: ${hexdigest}`);
    console.log(`\t\tbytes: uint8(${bytes[0]}, ${bytes[1]}, ${bytes[2]}, ${bytes[3]}, ${bytes[4]}, ${bytes[5]}, ${bytes[6]}, ${bytes[7]})`);
    console.log(`\t\twords: uint16(${words[0]}, ${words[1]}, ${words[2]}, ${words[3]})`);
    console.log("\n");
});