//////////////////////////////////////////////
//      PoxHash v1                          //
//      Implementation in JavaScript			  //
//      By Chubak Bidpaa - March 2023       //
//      License: GPLv3                      //
//////////////////////////////////////////////

const cPOX_PRIMES = new Uint16Array([
  0xe537, 0xbd71, 0x9ef9, 0xbbcf, 0xf8dd, 0xceb7, 0xbaa1, 0x8f9f, 0xb0ed,
  0xfc4f, 0x9787, 0xf01f, 0xe1d1, 0xbcb9, 0xd565, 0xc011, 0xc1e1, 0xb58d,
  0xd4e1, 0x9ea1, 0xee49, 0x97cd, 0xdac9, 0xe257, 0xa32b, 0xafbb, 0xa5e3,
  0xfc43, 0xbf71, 0xe401, 0x8ebd, 0xd549,
]);
const cPOX_8B_PRIMES = new Uint16Array([
  0x2, 0x3, 0x5, 0x7, 0xb, 0xd, 0x11, 0x13, 0x17, 0x1d, 0x1f, 0x25, 0x29, 0x2b,
  0x2f, 0x35, 0x3b, 0x3d, 0x43, 0x47, 0x49, 0x4f, 0x53, 0x59, 0x61, 0x65, 0x67,
  0x6b, 0x6d, 0x71, 0x7f, 0x83, 0x89, 0x8b, 0x95, 0x97, 0x9d, 0xa3, 0xa7, 0xad,
  0xb3, 0xb5, 0xbf, 0xc1, 0xc5, 0xc7, 0xd3, 0xdf, 0xe3, 0xe5, 0xe9, 0xef, 0xf1,
  0xfb,
]);
const cPOX_MAGIC_PRIMES = new Uint16Array([0x33, 0x65]);
const cPOX_SINGLE_DIGIT_PRIMES = new Uint16Array([0x3, 0x5, 0x7]);
const cPOX_PRIME_A = 0x9f91;
const cPOX_PRIME_B = 0xdb3b;
const cPOX_PRIME_C = 0xc091;
const cPOX_PRIME_D = 0xac8b;

const cPOX_BLOCK_NUM = 64;
const cPOX_NUM_8B_PRIME = 54;
const cPOX_NUM_PRIME = 32;
const cPOX_CHUNK_NUM = 16;
const cPOX_ROUND_NUM = 8;
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
const cRANGE_ZTG = [0, 1, 2, 3];
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
const cDUO_CHARS = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "*", "#"];
const cOCT_CHARS = ["0", "1", "2", "3", "4", "5", "6", "7"];
const cBIN_CHARS = ["0", "1", "2"];
const cHEX_SIZE = 4;
const cDUO_SIZE = 5;
const cOCT_SIZE = 6;
const cBIN_SIZE = 16;
const cHEX_BASE = 16;
const cDUO_BASE = 12;
const cOCT_BASE = 8;
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
  for (let i = size * offset + size; i >= size * offset; i--) {
    res[i] = chars[dec % base];
    dec = iDiv(dec, base);
  }
};

const convertWordsToHexDigest = (words) => {
  let hex = "0".repeat(cHEX_SIZE * cPOX_PORTION_NUM).split("");
  for (let i = 0; i < cPOX_PORTION_NUM; i++) {
    convertBaseFromDecimal(cHEX_BASE, cHEX_SIZE, cHEX_CHARS, hex, words[i], i);
  }
  return hex.join("");
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

const convertWordsToBinDigest = (words) => {
  let bin = "0".repeat(cBIN_SIZE * cPOX_PORTION_NUM).split("");
  for (let i = 0; i < cPOX_PORTION_NUM; i++) {
    convertBaseFromDecimal(cBIN_BASE, cBIN_SIZE, cBIN_CHARS, bin, words[i], i);
  }
  return bin.join("");
};

const get8BPrime = (num) => {
  const remainder = num % cPOX_NUM_8B_PRIME;
  return cPOX_8B_PRIMES[remainder];
};

const log2N = (num) => {
  return num > 1 ? 1 + log2N(iDiv(num, 2)) : 0;
};

const poxAlpha = (tempArray) => {
  const aleph = (tempArray[0] ^ tempArray[1]) & cMASK_WORD_ZZFF;
  const theh = (tempArray[2] ^ tempArray[3]) & cMASK_WORD_FFZZ;
  const daal = (aleph | theh) % cPOX_8B_PRIMES[0];
  const gaaf = (aleph ^ theh) % cPOX_8B_PRIMES[1];

  tempArray[0] >>= daal;
  tempArray[1] >>= ((daal + gaaf) % 2) + 1;
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
  for (let i = 0; i < cPOX_NUM_PRIME; i++) {
    tempArray[0] ^= cPOX_PRIMES[i];
    tempArray[1] &= cPOX_PRIMES[i];
    tempArray[2] ^= cPOX_PRIMES[i];
    tempArray[3] &= cPOX_PRIMES[i];
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

  factorArray[ng] ^= (subportion[eo] | tmt) ^ dcaOddFactor;
  factorArray[chu] ^= (subportion[yo] & dca) ^ tmtOddFactor;
  factorArray[yo] ^= subportion[chu] ^ tmt ^ dcaOddFactor;
  factorArray[eo] ^= (subportion[ng] | dca) ^ tmtOddFactor;
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

function PoxHashTy(
  hexdigest,
  duodigest,
  octdigest,
  bindigest,
  bytes,
  words,
  doubles,
  quad
) {
  this.hexdigest = hexdigest;
  this.octdigest = duodigest;
  this.duodigest = octdigest;
  this.bindigest = bindigest;
  this.bytes = bytes;
  this.words = words;
  this.doubles = doubles;
  this.quad = quad;
}

poxHash = (data) => {
  /**
   * Converts the given data into a PoxHashTy object
   * Parameters:
   *      data: string or Uint8Array
   *
   * Returns:
   *      PoxHashTy
   *          PoxHashTy.hexdigest: string
   *          PoxHashTy.bytes: Uint8Array[8]
   *          PoxHashTy.factors: Uint16Array[4]
   *          PoxHashTy.doubles: Uint32Array[2]
   *          PoxHashTy.quad: BigInt.asUintN(64)
   */
  const processedInput = processInput(data);
  if (processInput == null) {
    console.log(
      "Error occured, wrong input! Must be Uint8Array, Int8Array or string."
    );
    return null;
  }
  let factorArray = new Uint16Array([
    cPOX_PRIME_A,
    cPOX_PRIME_B,
    cPOX_PRIME_C,
    cPOX_PRIME_D,
  ]);

  for (let i = 0; i < processedInput.length; i += cPOX_BLOCK_NUM) {
    const portion = processedInput.subarray(i, i + cPOX_BLOCK_NUM);
    poxProcessBlock(factorArray, portion);
  }

  const hexdigest = convertWordsToHexDigest(factorArray);
  const duodigest = convertWordsToDuoDigest(factorArray);
  const octdigest = convertWordsToOctDigest(factorArray);
  const bindigest = convertWordsToBinDigest(factorArray);
  const bytes = wordArrayToByteArray(factorArray);
  const doubles = wordArrayToDoubleArray(factorArray);
  const quad = doubleArrayToQuad(doubles);

  return new PoxHashTy(
    hexdigest,
    duodigest,
    octdigest,
    bindigest,
    bytes,
    factorArray,
    doubles,
    quad
  );
};

exports.poxHash = poxHash;
exports.PoxHashTy = PoxHashTy;
