//////////////////////////////////////////////
//      PoxHash v1                          //
//      Implementation in JavaScript			  //
//      By Chubak Bidpaa - March 2023       //
//      License: GPLv3                      //
//////////////////////////////////////////////

const cPOX_ROUND_PRIMES = new Uint16Array([
  0x0377, 0x0683, 0x05fb, 0x05fb, 0x0665, 0x074b, 0x036d, 0x033d, 0x0115,
  0x07cf, 0x0e59, 0x0e75, 0x0a75, 0x119b, 0x1073, 0x12b3, 0x0fd1, 0x0a75,
  0x0de7, 0x10bb, 0x18d1, 0x1c99, 0x1723, 0x1cc9, 0x20c3, 0x2327, 0x2063,
  0x215b, 0x17e1, 0x22bd, 0xf2ff, 0xf50b, 0xf4af, 0xf2b3, 0xf5fb, 0xf4af,
  0xf2b9, 0xf38b, 0xf4c3, 0xf5db, 0x1039, 0x1003, 0x0fa1, 0x0fa3, 0x0fa7,
  0x8687, 0x80db, 0x86d1, 0x7fcd, 0x7f43, 0xa10b, 0x9e81, 0x9d15, 0xa289,
  0xa279, 0x3e11, 0x3aa5, 0x3be3, 0x3daf, 0x3bff, 0xff8f, 0xff71, 0xfe03,
  0xfe41, 0xfe05, 0xff2f, 0xfe7b, 0xfeb3, 0x0409, 0x0481, 0x1d7b, 0x1c4f,
  0x1e6d, 0x1b7f, 0x1e71, 0xe875, 0xe2cd, 0xe351, 0xe363, 0xe329, 0x049d,
  0x0427, 0xcbb3, 0x184d, 0x2ce1, 0x8861, 0x59b3, 0x2077, 0xff9d, 0xff2f,
]);
const cPOX_8B_PRIMES = new Uint16Array([
  0x2, 0x3, 0x5, 0x7, 0xb, 0xd, 0x11, 0x13, 0x17, 0x1d, 0x1f, 0x25, 0x29, 0x2b,
  0x2f, 0x35, 0x3b, 0x3d, 0x43, 0x47, 0x49, 0x4f, 0x53, 0x59, 0x61, 0x65, 0x67,
  0x6b, 0x6d, 0x71, 0x7f, 0x83, 0x89, 0x8b, 0x95, 0x97, 0x9d, 0xa3, 0xa7, 0xad,
  0xb3, 0xb5, 0xbf, 0xc1, 0xc5, 0xc7, 0xd3, 0xdf, 0xe3, 0xe5, 0xe9, 0xef, 0xf1,
  0xfb,
]);
const cPOX_SINGLE_DIGIT_PRIMES = new Uint16Array([0x3, 0x5, 0x7]);
const cPOX_MAGIC_PRIMES = new Uint16Array([0x33, 0x65]);

const cPOX_PRIME_INIT_A = 0x17cb;
const cPOX_PRIME_INIT_B = 0x0371;
const cPOX_PRIME_INIT_C = 0x2419;
const cPOX_PRIME_INIT_D = 0xf223;

const cPOX_ROUND_PRIME_NUM = 90;
const cPOX_BLOCK_NUM = 64;
const cPOX_8B_PRIME_NUM = 54;
const cPOX_ROUND_NUM = 31;
const cPOX_CHUNK_NUM = 16;
const cPOX_PORTION_NUM = 4;
const cPOX_SD_PRIME_NUM = 3;
const cPOX_MAGIC_PRIME_NUM = 2;

const cWORD_WIDTH = 16;
const cBYTE_WIDTH = 8;
const cUINT16_MAX = 2 ** 16 - 1;

const cMASK_DWORD_4F4Z = 0xffff0000;
const cMASK_DWORD_4Z4F = 0x0000ffff;
const cMASK_WORD_FZFZ = 0xf0f0;
const cMASK_WORD_ZFZF = 0x0f0f;
const cMASK_WORD_FZZZ = 0xf000;
const cMASK_WORD_ZZFZ = 0x00f0;
const cMASK_WORD_ZZZF = 0x000f;
const cMASK_WORD_ZZFF = 0x00ff;
const cMASK_WORD_FFZZ = 0xff00;
const cMASK_WORD_FZZF = 0xf00f;
const cMASK_WORD_FFFZ = 0xfff0;
const cMASK_WORD_ZFFF = 0x0fff;
const cMASK_NIBBLET_01 = 0b01;
const cMASK_NIBBLET_10 = 0b10;
const cMASK_NIBBLET_11 = 0b11;
const cMASK_NIBBLET_00 = 0b00;

const cCOMB_BIONOM = [
  [0, 1],
  [0, 2],
  [0, 3],
  [1, 2],
  [1, 3],
  [2, 3],
];
const cSEX_CHARS = [
  "0",
  "1",
  "2",
  "3",
  "4",
  "5",
  "6",
  "7",
  "8",
  "9",
  "A",
  "B",
  "C",
  "D",
  "E",
  "F",
  "G",
  "H",
  "I",
  "J",
  "K",
  "L",
  "M",
  "N",
  "O",
  "P",
  "Q",
  "R",
  "S",
  "T",
  "U",
  "V",
  "W",
  "X",
  "Y",
  "Z",
  "a",
  "b",
  "c",
  "d",
  "e",
  "f",
  "g",
  "h",
  "i",
  "j",
  "k",
  "l",
  "m",
  "n",
  "o",
  "p",
  "q",
  "r",
  "s",
  "t",
  "u",
  "v",
  "w",
  "x",
];
const cVIG_CHARS = [
  "A",
  "B",
  "C",
  "D",
  "E",
  "F",
  "G",
  "H",
  "I",
  "J",
  "@",
  "^",
  "&",
  "*",
  "$",
  "+",
  "!",
  ";",
  ":",
  "~",
];
const cHEX_CHARS = [
  "0",
  "1",
  "2",
  "3",
  "4",
  "5",
  "6",
  "7",
  "8",
  "9",
  "A",
  "B",
  "C",
  "D",
  "E",
  "F",
];
const cTET_CHARS = [
  "0",
  "1",
  "2",
  "3",
  "4",
  "5",
  "6",
  "7",
  "8",
  "9",
  "T",
  "E",
  "W",
  "R",
];
const cDUO_CHARS = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "*", "#"];
const cOCT_CHARS = ["0", "1", "2", "3", "4", "5", "6", "7"];
const cSEN_CHARS = ["0", "1", "2", "3", "4", "5"];
const cBIN_CHARS = ["0", "1", "2"];
const cSEX_SIZE = 3;
const cVIG_SIZE = 4;
const cHEX_SIZE = 4;
const cTET_SIZE = 5;
const cDUO_SIZE = 5;
const cOCT_SIZE = 6;
const cSEN_SIZE = 7;
const cBIN_SIZE = 16;
const cSEX_BASE = 60;
const cVIG_BASE = 20;
const cHEX_BASE = 16;
const cTET_BASE = 14;
const cDUO_BASE = 12;
const cOCT_BASE = 8;
const cSEN_BASE = 6;
const cBIN_BASE = 2;

const cBIONOM_SIZE = 6;

const iDiv = (a, b) => {
  return ~~(a / b);
};

const omega = (dwArr) => {
  dwArr[0] = (dwArr[0] & cMASK_DWORD_4F4Z) >> cWORD_WIDTH;
};

const epsilon = (dwArr) => {
  dwArr[0] &= cMASK_DWORD_4Z4F;
};

const lamed = (dwArr, by) => {
  dwArr[0] = (dwArr[0] << by) | (dwArr[0] >> (cWORD_WIDTH - by));
};

const gorda = (num, by) => {
  let resUint32 = new Uint32Array([num]);
  lamed(resUint32);

  if (resUint32[0] > cUINT16_MAX) {
    omega(resUint32);
  }

  const resUint16 = new Uint16Array([resUint32[0]]);
  return resUint16;
};

const tasu = (arrayA, arrayB, index) => {
  let resUint32 = new Uint32Array(1);
  resUint32[0] = arrayA[index] + arrayB[index];

  if (resUint32[0] > cUINT16_MAX) {
    epsilon(resUint32);
  }

  arrayA[index] = resUint32[0];
};

const centum = (ls, weights) => {
  let centum = new Uint32Array([0]);
  for (let i = 0; i < cPOX_PORTION_NUM; i++) {
    centum[0] += ls[i] * weights[i];
  }

  centum[0] = iDiv(centum[0], cPOX_PORTION_NUM);
  if (centum[0] > cUINT16_MAX) {
    omega(centum);
  }

  return centum[0];
};

const satum = (ls, weights) => {
  let satum = new Uint32Array([0]);
  for (let i = 0; i < cPOX_PORTION_NUM; i++) {
    satum[0] += ls[i] * weights[i];
  }

  satum[0] = iDiv(satum[0] + 1, 2);
  if (satum[0] > cUINT16_MAX) {
    epsilon(satum);
  }

  return satum[0];
};

const tamaam = (ls) => {
  let tmt = new Uint32Array([0]);
  for (let i = 0; i < cPOX_PORTION_NUM; i++) {
    tmt[0] += ls[i];
  }

  tmt[0] = iDiv(tmt[0], cPOX_PORTION_NUM);
  if (tmt[0] > cUINT16_MAX) {
    omega(tmt);
  }

  return tmt[0];
};

const deca = (ls) => {
  let dca = new Uint32Array([0]);
  for (let i = 0; i < cPOX_PORTION_NUM; i++) {
    dca[0] += ls[i];
  }

  dca[0] = iDiv(dca[0] + 1, 2);
  if (dca[0] > cUINT16_MAX) {
    epsilon(dca);
  }

  return dca[0];
};

const maxAndArgMax = (ls) => {
  let currMax = ls[0];
  let currIndex = 0;

  for (let i = 1; i < cPOX_PORTION_NUM; i++) {
    if (ls[i] > currMax) {
      currMax = ls[i];
      currIndex = i;
    }
  }

  return { max: currMax, argmax: currIndex };
};

const minAndArgMin = (ls) => {
  let currMin = ls[0];
  let currIndex = 0;

  for (let i = 1; i < cPOX_PORTION_NUM; i++) {
    if (ls[i] < currMin) {
      currMin = ls[i];
      currIndex = i;
    }
  }

  return { min: currMin, argmin: currIndex };
};

const wordArrayToByteArray = (wordarray) => {
  const wordToBye = (word) => {
    const lower = word & cMASK_WORD_ZZFF;
    const upper = (word & cMASK_WORD_FFZZ) >> cBYTE_WIDTH;

    return { lower: lower, upper: upper };
  };

  const upperLowerA = wordToBye(wordarray[0]);
  const upperLowerB = wordToBye(wordarray[1]);
  const upperLowerC = wordToBye(wordarray[2]);
  const upperLowerD = wordToBye(wordarray[3]);

  const byteArray = new Uint8Array([
    upperLowerA.lower,
    upperLowerA.upper,
    upperLowerB.lower,
    upperLowerB.upper,
    upperLowerC.lower,
    upperLowerC.upper,
    upperLowerD.lower,
    upperLowerD.upper,
  ]);

  return byteArray;
};

const wordToDouble = (wordA, wordB, dArr, index) => {
  const wArrD = new Uint32Array([wordA, wordB]);
  dArr[index] |= wArrD[0];
  dArr[index] |= wArrD[1] << 16;
};

const wordArrayToDoubleArray = (wArray) => {
  let dArr = new Uint32Array(2);
  wordToDouble(wArray[0], wArray[1], dArr, 0);
  wordToDouble(wArray[2], wArray[3], dArr, 1);
  return dArr;
};

const doubleArrayToQuad = (dArr) => {
  let quad = BigInt("0x0000000000000000");
  quad |= BigInt(dArr[0]);
  quad |= BigInt(dArr[1]) << BigInt(32);
  return BigInt.asUintN(64, quad);
};

const byteArrayToWordArrayAndPad = (bytearr) => {
  let size = bytearr.length;
  while (size % cPOX_BLOCK_NUM != 0) {
    size += 1;
  }
  let u16Array = new Uint16Array(size);
  for (let i = 0; i < size; i++) {
    u16Array[i] = bytearr[i];
  }
  return u16Array;
};

const stringToByteArray = (str) => {
  let utf8Encode = new TextEncoder();
  byteArray = utf8Encode.encode(str);
  return byteArray;
};

const processInput = (input) => {
  if (typeof input == "string") {
    const byteArray = stringToByteArray(input);
    return byteArrayToWordArrayAndPad(byteArray);
  } else if (typeof input == "object") {
    if (
      input.constructor.name == "Uint8Array" ||
      input.constructor.name == "Int8Array"
    ) {
      return byteArrayToWordArrayAndPad(input);
    }
  }
  return null;
};

const convertBaseFromDecimal = (base, size, chars, res, dec, offset) => {
  for (let i = size * offset + size - 1; i >= size * offset; i--) {
    res[i] = chars[dec % base];
    dec = iDiv(dec, base);
  }
};

const convertWordsToSexDigest = (words) => {
  let sex = "0".repeat(cSEX_SIZE * cPOX_PORTION_NUM).split("");
  for (let i = 0; i < cPOX_PORTION_NUM; i++) {
    convertBaseFromDecimal(cSEX_BASE, cSEX_SIZE, cSEX_CHARS, sex, words[i], i);
  }
  return sex.join("");
};

const convertWordsToVigDigest = (words) => {
  let vig = "0".repeat(cVIG_SIZE * cPOX_PORTION_NUM).split("");
  for (let i = 0; i < cPOX_PORTION_NUM; i++) {
    convertBaseFromDecimal(cVIG_BASE, cVIG_SIZE, cVIG_CHARS, vig, words[i], i);
  }
  return vig.join("");
};

const convertWordsToHexDigest = (words) => {
  let hex = "0".repeat(cHEX_SIZE * cPOX_PORTION_NUM).split("");
  for (let i = 0; i < cPOX_PORTION_NUM; i++) {
    convertBaseFromDecimal(cHEX_BASE, cHEX_SIZE, cHEX_CHARS, hex, words[i], i);
  }
  return hex.join("");
};

const convertWordsToTetDigest = (words) => {
  let tet = "0".repeat(cTET_SIZE * cPOX_PORTION_NUM).split("");
  for (let i = 0; i < cPOX_PORTION_NUM; i++) {
    convertBaseFromDecimal(cTET_BASE, cTET_SIZE, cTET_CHARS, tet, words[i], i);
  }
  return tet.join("");
};

const convertWordsToDuoDigest = (words) => {
  let duo = "0".repeat(cDUO_SIZE * cPOX_PORTION_NUM).split("");
  for (let i = 0; i < cPOX_PORTION_NUM; i++) {
    convertBaseFromDecimal(cDUO_BASE, cDUO_SIZE, cDUO_CHARS, duo, words[i], i);
  }
  return duo.join("");
};

const convertWordsToOctDigest = (words) => {
  let oct = "0".repeat(cOCT_SIZE * cPOX_PORTION_NUM).split("");
  for (let i = 0; i < cPOX_PORTION_NUM; i++) {
    convertBaseFromDecimal(cOCT_BASE, cOCT_SIZE, cOCT_CHARS, oct, words[i], i);
  }
  return oct.join("");
};

const convertWordsToSenDigest = (words) => {
  let sen = "0".repeat(cSEN_SIZE * cPOX_PORTION_NUM).split("");
  for (let i = 0; i < cPOX_PORTION_NUM; i++) {
    convertBaseFromDecimal(cSEN_BASE, cSEN_SIZE, cSEN_CHARS, sen, words[i], i);
  }
  return sen.join("");
};

const convertWordsToBinDigest = (words) => {
  let bin = "0".repeat(cBIN_SIZE * cPOX_PORTION_NUM).split("");
  for (let i = 0; i < cPOX_PORTION_NUM; i++) {
    convertBaseFromDecimal(cBIN_BASE, cBIN_SIZE, cBIN_CHARS, bin, words[i], i);
  }
  return bin.join("");
};

const get8BPrime = (num) => {
  const remainder = num % cPOX_8B_PRIME_NUM;
  return cPOX_8B_PRIMES[remainder];
};

const log2N = (num) => {
  return num > 1 ? 1 + log2N(iDiv(num, 2)) : 0;
};

const poxAlpha = (tempArray) => {
  const aleph = (tempArray[0] ^ tempArray[1]) & cMASK_WORD_ZZFF;
  const daal = (tempArray[2] ^ tempArray[3]) & cMASK_WORD_FFZZ;
  const theh = (aleph | daal) % cPOX_8B_PRIMES[0];
  const gaaf = (aleph ^ daal) % cPOX_8B_PRIMES[1];

  tempArray[0] >>= theh;
  tempArray[1] >>= ((theh + gaaf) % 2) + 1;
  tempArray[2] >>= gaaf;
};

const poxDelta = (tempArray) => {
  let alaf = (tempArray[0] ^ cMASK_WORD_FFFZ) % get8BPrime(tempArray[0]);
  let dalat = (tempArray[1] ^ cMASK_WORD_FZZF) % get8BPrime(tempArray[1]);
  let tit = (tempArray[2] & cMASK_WORD_ZFFF) % get8BPrime(tempArray[2]);
  let gaman = (tempArray[3] & cMASK_WORD_FFZZ) % get8BPrime(tempArray[3]);

  for (let i = 0; i < cPOX_PORTION_NUM; i++) {
    alaf >>= cPOX_SINGLE_DIGIT_PRIMES[dalat % cPOX_SD_PRIME_NUM];
    dalat = gorda(dalat, 2)[0];
    tit >>= cPOX_SINGLE_DIGIT_PRIMES[gaman % cPOX_SD_PRIME_NUM];
    gaman ^=
      (alaf ^ cMASK_WORD_ZZFF) >>
      cPOX_SINGLE_DIGIT_PRIMES[tit % cPOX_SD_PRIME_NUM];
  }

  tempArray[1] ^= tempArray[2] % cPOX_MAGIC_PRIMES[alaf % cPOX_MAGIC_PRIME_NUM];
  tempArray[2] ^= alaf + tit;
  tempArray[3] ^= tit + gaman;
};

const poxTheta = (tempArray) => {
  const alef = tempArray[0] % 2;
  const dalet = tempArray[1] % 2;
  const tet = tempArray[2] % 2;
  const gimmel = tempArray[3] % 2;

  const ctm = centum(tempArray, [alef, dalet, tet, gimmel]);
  const stm = satum(tempArray, [alef, dalet, tet, gimmel]);

  tempArray[0] ^= ((ctm >> gimmel) ^ cMASK_WORD_ZZFF) & cMASK_WORD_ZZZF;
  tempArray[3] ^= ((stm << alef) ^ cMASK_WORD_FZFZ) & cMASK_WORD_FZZZ;
};

const poxGamma = (tempArray) => {
  const minWithArgmin = minAndArgMin(tempArray);
  const maxWithArgmax = maxAndArgMax(tempArray);

  const mmin = minWithArgmin.min;
  const mmax = maxWithArgmax.max;
  const argmin = minWithArgmin.argmin;
  const argmax = maxWithArgmax.argmax;
  const ay = argmin & cMASK_NIBBLET_01;
  const dee = argmax ^ cMASK_NIBBLET_10;
  const thorn = argmin & cMASK_NIBBLET_11;
  const gee = argmax ^ cMASK_NIBBLET_00;

  const alaph = tempArray[ay] % get8BPrime(tempArray[thorn]);
  const dalath = (get8BPrime(mmax) ^ cMASK_WORD_ZFZF) % get8BPrime(mmin);
  const teth = mmax % get8BPrime(mmax);
  const gamal = tempArray[dee] % get8BPrime(iDiv(mmin + mmax, 2));

  tempArray[ay] >>= (alaph ^ cMASK_WORD_ZZFZ) % cWORD_WIDTH;
  tempArray[dee] >>= (gamal ^ cMASK_WORD_FZZZ) % ((mmax % 2) + 1);
  tempArray[thorn] ^= log2N(dalath) & cMASK_WORD_ZFFF;
  tempArray[gee] ^= log2N(teth) >> ((gamal % 2) + 1);
};

const poxRoundApplyAlphabet = (tempArray) => {
  poxAlpha(tempArray);
  poxDelta(tempArray);
  poxTheta(tempArray);
  poxGamma(tempArray);
};

const poxRoundApplyPrime = (tempArray) => {
  for (let i = 0; i < cPOX_ROUND_PRIME_NUM; i++) {
    tempArray[0] %= cPOX_ROUND_PRIMES[i];
    tempArray[1] %= cPOX_ROUND_PRIMES[i];
    tempArray[2] %= cPOX_ROUND_PRIMES[i];
    tempArray[3] %= cPOX_ROUND_PRIMES[i];
  }
};

const poxRoundAddTempsToFacts = (factorArray, tempArray) => {
  tasu(factorArray, tempArray, 0);
  tasu(factorArray, tempArray, 1);
  tasu(factorArray, tempArray, 2);
  tasu(factorArray, tempArray, 3);
};

const poxRoundApplyShuffle = (tempArray) => {
  const swap = (tempArray, indexof, indexwith) => {
    const tmp = tempArray[indexof];
    tempArray[indexof] = tempArray[indexwith];
    tempArray[indexwith] = tmp;
  };

  for (let i = 0; i < cBIONOM_SIZE; i++) {
    swap(tempArray, cCOMB_BIONOM[i][0], cCOMB_BIONOM[i][1]);
  }
};

const poxRound = (factorArray) => {
  let tempArray = new Uint16Array(factorArray);
  poxRoundApplyAlphabet(tempArray);
  poxRoundApplyPrime(tempArray);
  poxRoundApplyShuffle(tempArray);
  poxRoundAddTempsToFacts(factorArray, tempArray);
};

const debug = (arr) => console.log(arr[0], arr[1], arr[2], arr[3]);

const poxApplySubportion = (factorArray, subportion, index) => {
  const tmt = tamaam(subportion);
  const dca = deca(subportion);
  const tmtOddFactor = cUINT16_MAX * (tmt % 2);
  const dcaOddFactor = cUINT16_MAX * (dca % 2);

  const ng = (subportion[0] + index) % cPOX_PORTION_NUM;
  const chu = (subportion[1] + index) % cPOX_PORTION_NUM;
  const yo = (subportion[2] + index) % cPOX_PORTION_NUM;
  const eo = (subportion[3] + index) % cPOX_PORTION_NUM;

  const zam = subportion[0] % cPOX_8B_PRIMES[subportion[chu] % cPOX_8B_PRIME_NUM]
  const pez = subportion[1] % cPOX_8B_PRIMES[subportion[yo] % cPOX_8B_PRIME_NUM]
  const dit = subportion[2] % cPOX_8B_PRIMES[subportion[eo] % cPOX_8B_PRIME_NUM]
  const kit = subportion[3] % cPOX_8B_PRIMES[subportion[ng] % cPOX_8B_PRIME_NUM]

  factorArray[ng] ^= ((subportion[eo] | tmt) ^ dcaOddFactor) | zam;
  factorArray[chu] ^= ((subportion[yo] & dca) ^ tmtOddFactor) ^ pez;
  factorArray[yo] ^= (subportion[chu] ^ tmt ^ dcaOddFactor) | dit;
  factorArray[eo] ^= ((subportion[ng] | dca) ^ tmtOddFactor) ^ kit;
};

const poxProcessBlock = (factorArray, blockArray) => {
  for (let i = 0; i < cPOX_BLOCK_NUM; i += cPOX_CHUNK_NUM) {
    for (let j = i; j < i + cPOX_CHUNK_NUM; j += cPOX_PORTION_NUM) {
      let subportion = blockArray.subarray(j, j + cPOX_PORTION_NUM);
      for (let m = 0; m < cPOX_ROUND_NUM; m++) {
        poxApplySubportion(factorArray, subportion, m);
        poxRound(factorArray);
      }
    }
  }
};

function PoxDigest(
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
  quad
) {
  this.sexdigest = sexdigest;
  this.vigdigest = vigdigest;
  this.hexdigest = hexdigest;
  this.tetdigest = tetdigest;
  this.duodigest = octdigest;
  this.octdigest = duodigest;
  this.sendigest = sendigest;
  this.bindigest = bindigest;
  this.bytes = bytes;
  this.words = words;
  this.doubles = doubles;
  this.quad = quad;
}

poxHash = (message) => {
  /**
   * Converts the given message into a PoxDigest object
   * Parameters:
   *      message: string or Uint8Array
   *
   * Returns:
   *      PoxDigest
   *          PoxDigest.sexdigest: string
   *          PoxDigest.vigdigest: string
   *          PoxDigest.hexdigest: string
   *          PoxDigest.tetdigest: string
   *          PoxDigest.duodigest: string
   *          PoxDigest.octdigest: string
   *          PoxDigest.sendigest: string
   *          PoxDigest.bindigest: string
   *          PoxDigest.bytes: Uint8Array[8]
   *          PoxDigest.factors: Uint16Array[4]
   *          PoxDigest.doubles: Uint32Array[2]
   *          PoxDigest.quad: BigInt.asUintN(64)
   */
  const processedInput = processInput(message);
  if (processInput == null) {
    console.log(
      "Error occured, wrong input! Must be Uint8Array, Int8Array or string."
    );
    return null;
  }
  let factorArray = new Uint16Array([
    cPOX_PRIME_INIT_A,
    cPOX_PRIME_INIT_B,
    cPOX_PRIME_INIT_C,
    cPOX_PRIME_INIT_D,
  ]);

  for (let i = 0; i < processedInput.length; i += cPOX_BLOCK_NUM) {
    const portion = processedInput.subarray(i, i + cPOX_BLOCK_NUM);
    poxProcessBlock(factorArray, portion);
  }
  const sexdigest = convertWordsToSexDigest(factorArray);
  const vigdigest = convertWordsToVigDigest(factorArray);
  const hexdigest = convertWordsToHexDigest(factorArray);
  const tetdigest = convertWordsToTetDigest(factorArray);
  const duodigest = convertWordsToDuoDigest(factorArray);
  const octdigest = convertWordsToOctDigest(factorArray);
  const sendigest = convertWordsToSenDigest(factorArray);
  const bindigest = convertWordsToBinDigest(factorArray);
  const bytes = wordArrayToByteArray(factorArray);
  const doubles = wordArrayToDoubleArray(factorArray);
  const quad = doubleArrayToQuad(doubles);

  return new PoxDigest(
    sexdigest,
    vigdigest,
    hexdigest,
    tetdigest,
    duodigest,
    octdigest,
    sendigest,
    bindigest,
    bytes,
    factorArray,
    doubles,
    quad
  );
};

exports.poxHash = poxHash;
exports.PoxDigest = PoxDigest;
