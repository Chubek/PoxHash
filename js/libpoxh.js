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

const cWORD_WIDTH = 16;
const cBYTE_WIDTH = 8;
const cUINT16_MAX = 2 ** 16 - 1;
const cNUM_SD_PRIME = 3;
const cHEX_SIZE = 4;

const cONE_UPPER16 = 0xffff0000;
const cONE_LOWER16 = 0x0000ffff;
const cMASK_FZFZ = 0xf0f0;
const cMASK_ZFZF = 0x0f0f;
const cMASK_FZZZ = 0xf000;
const cMASK_ZZFZ = 0x00f0;
const cMASK_ZZZF = 0x000f;
const cMASK_ZZFF = 0x00ff;
const cMASK_FFZZ = 0xff00;
const cMASK_FZZF = 0xf00f;
const cMASK_FFFZ = 0xfff0;
const cMASK_ZFFF = 0x0fff;
const cMASK_01 = 0b01;
const cMASK_10 = 0b10;
const cMASK_11 = 0b11;
const cMASK_00 = 0b00;

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
const cBIONOM_SIZE = 6;

const iDiv = (a, b) => {
  return ~~(a / b);
};

const rotateLeft = (num, by) => {
  let resUint32 = new Uint32Array([num]);
  resUint32[0] = (resUint32[0] << by) | (resUint32[0] >> (cWORD_WIDTH - by));

  if (resUint32[0] > cUINT16_MAX) {
    resUint32[0] = (resUint32[0] & cONE_UPPER16) >> cWORD_WIDTH;
  }

  const resUint16 = new Uint16Array([resUint32[0]]);
  return resUint16;
};

const addWithOverflow = (arrayA, arrayB, index) => {
  let resUint32 = new Uint32Array(1);
  resUint32[0] = arrayA[index] + arrayB[index];

  if (resUint32[0] > cUINT16_MAX) {
    resUint32[0] &= cONE_LOWER16;
  }

  arrayA[index] = resUint32[0];
};

const weightedAvg = (ls, weights) => {
  let weightedAvg = 0;
  for (let i = 0; i < cPOX_PORTION_NUM; i++) {
    weightedAvg += ls[i] * weights[i];
  }

  weightedAvg = iDiv(weightedAvg, cPOX_PORTION_NUM);
  if (weightedAvg > cUINT16_MAX) {
    weightedAvg = (weightedAvg & cONE_UPPER16) >> cWORD_WIDTH;
  }

  return weightedAvg;
};

const weightedMed = (ls, weights) => {
  let weightedMed = 0;
  for (let i = 0; i < cPOX_PORTION_NUM; i++) {
    weightedMed += ls[i] * weights[i];
  }

  weightedMed = iDiv(weightedMed + 1, 2);
  if (weightedMed > cUINT16_MAX) {
    weightedMed &= cONE_LOWER16;
  }

  return weightedMed;
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
    const lower = word & cMASK_ZZFF;
    const upper = (word & cMASK_FFZZ) >> cBYTE_WIDTH;

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

const byteArrayToWordArrayAndPad = (bytearr) => {
  let size = bytearr.length;
  while (size % cPOX_BLOCK_NUM != 0) {
    size += 1;
  }

  let wordArray = new Uint16Array(size);
  for (let i = 0; i < bytearr.length; i++) {
    wordArray[i] = bytearr[i];
  }

  return wordArray;
};

const stringToByteArray = (str) => {
  let size = str.length;
  let byteArray = new Uint8Array(size);

  for (let i = 0; i < size; i++) {
    byteArray[i] = str.charCodeAt(i);
  }

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

const decimalToHex = (num) => {
  let ret = "0000";
  for (let i = 0; i < cHEX_SIZE; i++) {
    ret = setCharAt(ret, cHEX_SIZE - i - 1, cHEX_CHARS[num % cWORD_WIDTH]);
    num = iDiv(num, cWORD_WIDTH);
  }

  return ret;
};

const wordArrayToHex = (wordarr) => {
  const hexA = decimalToHex(wordarr[0]);
  const hexB = decimalToHex(wordarr[1]);
  const hexC = decimalToHex(wordarr[2]);
  const hexD = decimalToHex(wordarr[3]);

  return `${hexA}${hexB}${hexC}${hexD}`;
};

const get8BPrime = (num) => {
  const remainder = num % cPOX_NUM_8B_PRIME;
  return cPOX_8B_PRIMES[remainder];
};

const setCharAt = (str, index, chr) => {
  if (index > str.length - 1) return str;
  return str.substring(0, index) + chr + str.substring(index + 1);
};

const log2N = (num) => {
  return num > 1 ? 1 + log2N(iDiv(num, 2)) : 0;
};

const sumArray = (array) => {
  let sum = 0;
  for (let i = 0; i < cPOX_PORTION_NUM; i++) {
    sum += array[i];
  }
  return sum;
};

const poxAlpha = (tempArray) => {
  const aleph = (tempArray[0] ^ tempArray[1]) & cMASK_ZZFF;
  const theh = (tempArray[2] ^ tempArray[3]) & cMASK_FFZZ;
  const daal = (aleph | theh) % cPOX_8B_PRIMES[0];
  const gaaf = (aleph ^ theh) % cPOX_8B_PRIMES[1];

  tempArray[0] >>= daal;
  tempArray[1] >>= ((daal + gaaf) % 2) + 1;
  tempArray[2] >>= gaaf;
};

const poxDelta = (tempArray) => {
  let alaf = (tempArray[0] ^ cMASK_FFFZ) % get8BPrime(tempArray[0]);
  let dalat = (tempArray[1] ^ cMASK_FZZF) % get8BPrime(tempArray[1]);
  let tit = (tempArray[2] & cMASK_ZFFF) % get8BPrime(tempArray[2]);
  let gaman = (tempArray[3] & cMASK_FFZZ) % get8BPrime(tempArray[3]);

  for (let i = 0; i < cPOX_PORTION_NUM; i++) {
    alaf >>= cPOX_SINGLE_DIGIT_PRIMES[dalat % cNUM_SD_PRIME];
    dalat = rotateLeft(dalat, 2)[0];
    tit >>= cPOX_SINGLE_DIGIT_PRIMES[gaman % cNUM_SD_PRIME];
    gaman ^=
      (alaf ^ cMASK_ZZFF) >> cPOX_SINGLE_DIGIT_PRIMES[tit % cNUM_SD_PRIME];
  }

  tempArray[1] ^= tempArray[2] % cPOX_MAGIC_PRIMES[1];
  tempArray[2] ^= alaf + tit;
  tempArray[3] ^= tit + gaman;
};

const poxTheta = (tempArray) => {
  const alef = tempArray[0] % 2;
  const dalet = tempArray[1] % 2;
  const tet = tempArray[2] % 2;
  const gimmel = tempArray[3] % 2;

  const weighted_avg = weightedAvg(tempArray, [alef, dalet, tet, gimmel]);
  const weighted_med = weightedMed(tempArray, [alef, dalet, tet, gimmel]);

  tempArray[0] ^= ((weighted_avg >> gimmel) ^ cMASK_ZZFF) & cMASK_ZZZF;
  tempArray[3] ^= ((weighted_med << alef) ^ cMASK_FZFZ) & cMASK_FZZZ;
};

const poxGamma = (tempArray) => {
  const minWithArgmin = minAndArgMin(tempArray);
  const maxWithArgmax = maxAndArgMax(tempArray);

  const mmin = minWithArgmin.min;
  const mmax = maxWithArgmax.max;
  const argmin = minWithArgmin.argmin;
  const argmax = maxWithArgmax.argmax;
  const ay = argmin & cMASK_01;
  const dee = argmax ^ cMASK_10;
  const thorn = argmin & cMASK_11;
  const gee = argmax ^ cMASK_00;

  const alaph = tempArray[ay] % get8BPrime(tempArray[thorn]);
  const dalath = (get8BPrime(mmax) ^ cMASK_ZFZF) % get8BPrime(mmin);
  const teth = mmax % get8BPrime(mmax);
  const gamal = tempArray[dee] % get8BPrime(iDiv(mmin + mmax, 2));

  tempArray[ay] >>= (alaph ^ cMASK_ZZFZ) % cWORD_WIDTH;
  tempArray[dee] >>= (gamal ^ cMASK_FZZZ) % ((mmax % 2) + 1);
  tempArray[thorn] ^= log2N(dalath) & cMASK_ZFFF;
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
  addWithOverflow(factorArray, tempArray, 0);
  addWithOverflow(factorArray, tempArray, 1);
  addWithOverflow(factorArray, tempArray, 2);
  addWithOverflow(factorArray, tempArray, 3);
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

const poxApplySubportion = (factorArray, subportion) => {
  const avgSubportion = iDiv(sumArray(subportion), cPOX_PORTION_NUM);
  const medSubportion = iDiv(sumArray(subportion) + 1, 2);
  const avgOddFactor = cUINT16_MAX * (avgSubportion % 2);
  const medOddFactor = cUINT16_MAX * (medSubportion % 2);

  factorArray[0] ^= (subportion[0] + avgSubportion) ^ medOddFactor;
  factorArray[1] ^= (subportion[1] + medSubportion) ^ avgOddFactor;
  factorArray[2] ^= (subportion[2] + avgSubportion) ^ medOddFactor;
  factorArray[3] ^= (subportion[3] + medSubportion) ^ avgOddFactor;
};

const poxProcessBlock = (factorArray, blockArray) => {
  for (let i = 0; i < cPOX_BLOCK_NUM; i += cPOX_CHUNK_NUM) {
    for (let j = i; j < i + cPOX_CHUNK_NUM; j += cPOX_PORTION_NUM) {
      let subportion = blockArray.subarray(j, j + cPOX_PORTION_NUM);
      for (let _m = 0; _m < cPOX_ROUND_NUM; _m++) {
        poxApplySubportion(factorArray, subportion);
        poxRound(factorArray);
      }
    }
  }
};

function PoxHashTy(hexdigest, bytes, words) {
  this.hexdigest = hexdigest;
  this.bytes = bytes;
  this.words = words;
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

  const hexdigest = wordArrayToHex(factorArray);
  const bytes = wordArrayToByteArray(factorArray);

  return new PoxHashTy(hexdigest, bytes, factorArray);
};

exports.poxHash = poxHash;
exports.PoxHashTy = PoxHashTy;
