const libpox = require("./libpox.js");

console.log("Pox (JS) hashes for passed strings in hexdigest form:");
const args = process.argv.slice(2);
args.forEach((val, index) => {
    const hash = libpox.poxHash(val);
    console.log(`\t${index + 1} -> ${hash.hexdigest} for\n\t \`${val}\``);
});