//////////////////////////////////////////////
//      PoxHash v1                          //
//      Implementation in Go			    //
//      By Chubak Bidpaa - March 2023       //
//      License: GPLv3                      //
//////////////////////////////////////////////

package libpoxh

const (
	poxBLOCK_NUM       int = 64
	pox8B_PRIME_NUM        = 54
	poxROUND_PRIME_NUM     = 32
	poxCHUNK_NUM           = 16
	poxROUND_NUM           = 64
	poxPORTION_NUM         = 4
	poxSD_PRIME_NUM        = 3
	poxMAGIC_PRIME_NUM     = 2

	poxPRIME_INIT_A uint16 = 0x9f91
	poxPRIME_INIT_B        = 0xdb3b
	poxPRIME_INIT_C        = 0xc091
	poxPRIME_INIT_D        = 0xac8b

	bitWORD_WIDTH_U16 uint16 = 16
	bitBYTE_WIDTH_u16        = 8
	bitUINT16_MAX_U16        = 65535
	bitWORD_WIDTH_U32 uint32 = 16
	bitUINT16_MAX_U32        = 65535
	bitBYTE_ARR_SIZE         = 8

	numCOMB_BIONOM = 6
	numRANGE_ZTF   = 4

	baseSEX_SIZE int    = 3
	baseVIG_SIZE        = 4
	baseHEX_SIZE        = 4
	baseTET_SIZE        = 5
	baseDUO_SIZE        = 5
	baseOCT_SIZE        = 6
	baseSEN_SIZE        = 7
	baseBIN_SIZE        = 16
	baseSEX_NUM  uint16 = 60
	baseVIG_NUM         = 20
	baseHEX_NUM         = 16
	baseTET_NUM         = 14
	baseDUO_NUM         = 12
	baseOCT_NUM         = 8
	baseSEN_NUM         = 6
	baseBIN_NUM         = 2

	maskDWORD_4F4Z uint32 = 0xffff0000
	maskDWORD_4Z4F        = 0x0000ffff
	maskWORD_FZFZ  uint16 = 0xf0f0
	maskWORD_ZFZF         = 0x0f0f
	maskWORD_FZZZ         = 0xf000
	maskWORD_ZZFZ         = 0x00f0
	maskWORD_ZZZF         = 0x000f
	maskWORD_ZZFF         = 0x00ff
	maskWORD_FFZZ         = 0xff00
	maskWORD_FZZF         = 0xf00f
	maskWORD_FFFZ         = 0xfff0
	maskWORD_ZFFF         = 0x0fff
	maskNIBBLET_01 int    = 0b01
	maskNIBBLET_10        = 0b10
	maskNIBBLET_11        = 0b11
	maskNIBBLET_00        = 0b00
)

var (
	poxROUND_PRIMES = [poxROUND_PRIME_NUM]uint16{0xe537, 0xbd71, 0x9ef9, 0xbbcf, 0xf8dd, 0xceb7, 0xbaa1, 0x8f9f, 0xb0ed,
		0xfc4f, 0x9787, 0xf01f, 0xe1d1, 0xbcb9, 0xd565, 0xc011, 0xc1e1, 0xb58d,
		0xd4e1, 0x9ea1, 0xee49, 0x97cd, 0xdac9, 0xe257, 0xa32b, 0xafbb, 0xa5e3,
		0xfc43, 0xbf71, 0xe401, 0x8ebd, 0xd549}
	pox8BPRIMES = [pox8B_PRIME_NUM]uint16{0x2, 0x3, 0x5, 0x7, 0xb, 0xd, 0x11, 0x13, 0x17, 0x1d, 0x1f, 0x25, 0x29,
		0x2b, 0x2f, 0x35, 0x3b, 0x3d, 0x43, 0x47, 0x49, 0x4f, 0x53, 0x59, 0x61,
		0x65, 0x67, 0x6b, 0x6d, 0x71, 0x7f, 0x83, 0x89, 0x8b, 0x95, 0x97, 0x9d,
		0xa3, 0xa7, 0xad, 0xb3, 0xb5, 0xbf, 0xc1, 0xc5, 0xc7, 0xd3, 0xdf, 0xe3,
		0xe5, 0xe9, 0xef, 0xf1, 0xfb}
	poxMAGIC_PRIMES        = [...]uint16{0x33, 0x65}
	poxSINGLE_DIGIT_PRIMES = [...]uint16{0x3, 0x5, 0x7}

	iterCOMB_BIONOM = [numCOMB_BIONOM][2]int{{0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}}
	iterRANGE_ZTF   = [numRANGE_ZTF]int{0, 1, 2, 3}

	byteZERO_CHAR byte = 48

	bytesCHAR_SEX = [60]byte{
		48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
		65, 66, 67, 68, 69, 70, 71, 72, 73, 74,
		75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
		85, 86, 87, 88, 89, 90, 97, 98, 99, 100,
		101, 102, 103, 104, 105, 106, 107, 108,
		109, 110, 111, 112, 113, 114, 115, 116,
		117, 118, 119, 120,
	}
	bytesCHAR_VIG = [20]byte{
		65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 64, 94, 38, 42, 36, 43, 33, 59, 58, 126,
	}
	bytesCHAR_HEX = [16]byte{
		48,
		49,
		50,
		51,
		52,
		53,
		54,
		55,
		56,
		57,
		65,
		66,
		67,
		68,
		69,
		70}
	bytesCHAR_TET = [14]byte{
		48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 84, 69, 87, 82,
	}
	bytesCHAR_DUO = [12]byte{
		48,
		49,
		50,
		51,
		52,
		53,
		54,
		55,
		56,
		57,
		42,
		35}
	bytesCHAR_OCT = [8]byte{
		48,
		49,
		50,
		51,
		52,
		53,
		54,
		55,
	}
	bytesCHAR_SEN = [6]byte{
		48,
		49,
		50,
		51,
		52,
		53,
	}
	bytesCHAR_BIN = [2]byte{48, 49}
)

type factorType [poxPORTION_NUM]uint16
type blockType [poxBLOCK_NUM]uint16
type byteType [bitBYTE_ARR_SIZE]uint8

func omega(num uint32) uint32 {
	return (num & maskDWORD_4F4Z) >> bitWORD_WIDTH_U32
}

func epsilon(num uint32) uint32 {
	return num & maskDWORD_4Z4F
}

func ladca(num, by uint32) uint32 {
	return (num << by) | (num >> (bitWORD_WIDTH_U32 - by))
}

func gorda(num uint16, by uint32) uint16 {
	var res uint32 = uint32(num)
	res = ladca(res, by)

	if res > bitUINT16_MAX_U32 {
		res = omega(res)
	}

	return uint16(res)
}

func tasu(a, b uint16) uint16 {
	var aa, bb uint32 = uint32(a), uint32(b)
	a_plus_b := aa + bb

	if a_plus_b > bitUINT16_MAX_U32 {
		a_plus_b = epsilon(a_plus_b)
	}

	return uint16(a_plus_b)
}

func centum(arr, weights factorType) uint16 {
	var stm uint32 = 0
	for i, intgr := range arr {
		stm += uint32(intgr) * uint32(weights[i])
	}
	stm /= uint32(poxPORTION_NUM)

	if stm > bitUINT16_MAX_U32 {
		stm = omega(stm)
	}

	return uint16(stm)
}

func satem(arr, weights factorType) uint16 {
	var stm uint32 = 0
	for i, intgr := range arr {
		stm += uint32(intgr) * uint32(weights[i])
	}
	stm = (stm + 1) / 2

	if stm > bitUINT16_MAX_U32 {
		stm = epsilon(stm)
	}

	return uint16(stm)
}

func tamaam(arr factorType) uint16 {
	var stm uint32 = 0
	for _, intgr := range arr {
		stm += uint32(intgr)
	}
	stm /= uint32(poxPORTION_NUM)

	if stm > bitUINT16_MAX_U32 {
		stm = omega(stm)
	}

	return uint16(stm)
}

func deca(arr factorType) uint16 {
	var stm uint32 = 0
	for _, intgr := range arr {
		stm += uint32(intgr)
	}
	stm = (stm + 1) / 2

	if stm > bitUINT16_MAX_U32 {
		stm = epsilon(stm)
	}

	return uint16(stm)
}

func minAndArgmin(arr factorType) (uint16, int) {
	currMin := arr[0]
	currIndex := 0

	for i, intgr := range arr {
		if intgr < currMin {
			currMin = intgr
			currIndex = i
		}
	}

	return currMin, currIndex
}

func maxAndArgmax(arr factorType) (uint16, int) {
	currMax := arr[0]
	currIndex := 0

	for i, intgr := range arr {
		if intgr > currMax {
			currMax = intgr
			currIndex = i
		}
	}

	return currMax, currIndex
}

func wordToByte(word uint16) (uint8, uint8) {
	var lower, upper uint16 = word & maskWORD_ZZFF, (word & maskWORD_FFZZ) >> bitBYTE_WIDTH_u16
	return uint8(lower), uint8(upper)
}

func wordToDouble(wordA, wordB uint16) uint32 {
	var res uint32 = 0
	aDouble := uint32(wordA)
	bDouble := uint32(wordB)

	res |= aDouble
	res |= (bDouble << 16)

	return res
}

func wordArrayToDoubleArray(warr factorType) [2]uint32 {
	lower := wordToDouble(warr[0], warr[1])
	upper := wordToDouble(warr[2], warr[3])

	return [2]uint32{lower, upper}
}

func wordToQuad(wordA, wordB, wordC, wordD uint16) uint64 {
	var res uint64 = 0
	aQuad := uint64(wordA)
	bQuad := uint64(wordB)
	cQuad := uint64(wordC)
	dQuad := uint64(wordD)

	res |= aQuad
	res |= (bQuad << 16)
	res |= (cQuad << 32)
	res |= (dQuad << 48)

	return res

}

func wordArrToByteArr(wordarr factorType) byteType {
	var bytearr byteType
	z := 0
	for _, word := range wordarr {
		lower, upper := wordToByte(word)
		bytearr[z] = lower
		bytearr[z+1] = upper
		z += 2
	}
	return bytearr
}

func byteArrToWordArrAndPad(bytearr []byte) []uint16 {
	length := len(bytearr)
	for length%poxBLOCK_NUM != 0 {
		length++
	}
	padded := make([]uint16, length)
	for i, b := range bytearr {
		padded[i] = uint16(b)
	}
	return padded
}

func decimalToBase(base, dec uint16, size, offset int, chars, res []byte) {
	for i := ((offset * size) + size) - 1; i >= offset*size; i-- {
		res[i] = chars[dec%base]
		dec /= base
	}
}

func wordArrToSexDigest(wordarr factorType) string {
	var sex [baseSEX_SIZE * poxPORTION_NUM]byte
	for i, word := range wordarr {
		decimalToBase(baseSEX_NUM, word, baseSEX_SIZE, i, bytesCHAR_SEX[:], sex[:])
	}
	return string(sex[:])
}

func wordArrToVigDigest(wordarr factorType) string {
	var vig [baseVIG_SIZE * poxPORTION_NUM]byte
	for i, word := range wordarr {
		decimalToBase(baseVIG_NUM, word, baseVIG_SIZE, i, bytesCHAR_VIG[:], vig[:])
	}
	return string(vig[:])
}

func wordArrToHexDigest(wordarr factorType) string {
	var hex [baseHEX_SIZE * poxPORTION_NUM]byte
	for i, word := range wordarr {
		decimalToBase(baseHEX_NUM, word, baseHEX_SIZE, i, bytesCHAR_HEX[:], hex[:])
	}
	return string(hex[:])
}

func wordArrToTetDigest(wordarr factorType) string {
	var tet [baseTET_SIZE * poxPORTION_NUM]byte
	for i, word := range wordarr {
		decimalToBase(baseTET_NUM, word, baseTET_SIZE, i, bytesCHAR_TET[:], tet[:])
	}
	return string(tet[:])
}

func wordArrToDuoDigest(wordarr factorType) string {
	var duo [baseDUO_SIZE * poxPORTION_NUM]byte
	for i, word := range wordarr {
		decimalToBase(baseDUO_NUM, word, baseDUO_SIZE, i, bytesCHAR_DUO[:], duo[:])
	}
	return string(duo[:])
}

func wordArrToOctDigest(wordarr factorType) string {
	var oct [baseOCT_SIZE * poxPORTION_NUM]byte
	for i, word := range wordarr {
		decimalToBase(baseOCT_NUM, word, baseOCT_SIZE, i, bytesCHAR_OCT[:], oct[:])
	}
	return string(oct[:])
}

func wordArrToSenDigest(wordarr factorType) string {
	var sen [baseSEN_SIZE * poxPORTION_NUM]byte
	for i, word := range wordarr {
		decimalToBase(baseSEN_NUM, word, baseSEN_SIZE, i, bytesCHAR_SEN[:], sen[:])
	}
	return string(sen[:])
}

func wordArrToBinDigest(wordarr factorType) string {
	var bin [baseBIN_SIZE * poxPORTION_NUM]byte
	for i, word := range wordarr {
		decimalToBase(baseBIN_NUM, word, baseBIN_SIZE, i, bytesCHAR_BIN[:], bin[:])
	}
	return string(bin[:])
}

func get8BPrime(num uint16) uint16 {
	return pox8BPRIMES[num%uint16(pox8B_PRIME_NUM)]
}

func log2N(num uint16) uint16 {
	if num > 1 {
		return 1 + log2N(num/2)
	}
	return 0
}

func sumWordArray(wordarr factorType) uint16 {
	var sum uint16 = 0
	for _, word := range wordarr {
		sum += word
	}
	return sum
}

func copyWordArray(wordarr factorType) factorType {
	var ret factorType
	for i, word := range wordarr {
		ret[i] = word
	}
	return ret
}

func newPortion(block blockType, start int) factorType {
	var ret factorType
	ret[0] = block[start]
	ret[1] = block[start+1]
	ret[2] = block[start+2]
	ret[3] = block[start+3]
	return ret
}

func newBlock(message []uint16, start int) blockType {
	var ret blockType
	z := 0
	for i := start; i < start+poxBLOCK_NUM; i++ {
		ret[z] = message[i]
		z++
	}
	return ret
}

func newFactorArray() factorType {
	var ret factorType
	ret[0] = poxPRIME_INIT_A
	ret[1] = poxPRIME_INIT_B
	ret[2] = poxPRIME_INIT_C
	ret[3] = poxPRIME_INIT_D
	return ret
}

func poxAlpha(tempArray factorType) factorType {
	var aleph, daal, theh, gaaf uint16 = 0, 0, 0, 0

	aleph = (tempArray[0] ^ tempArray[1]) & maskWORD_ZZFF
	daal = (tempArray[2] ^ tempArray[3]) & maskWORD_FFZZ
	theh = (aleph | daal) % pox8BPRIMES[0]
	gaaf = (aleph ^ daal) % pox8BPRIMES[1]

	tempArrayCpy := copyWordArray(tempArray)

	tempArrayCpy[0] >>= theh
	tempArrayCpy[1] >>= ((theh + gaaf) % 2) + 1
	tempArrayCpy[2] >>= gaaf

	return tempArrayCpy
}

func poxDelta(tempArray factorType) factorType {
	var alaf, dalat, tit, gaman uint16 = 0, 0, 0, 0

	alaf = (tempArray[0] ^ maskWORD_FFFZ) % get8BPrime(tempArray[0])
	dalat = (tempArray[1] ^ maskWORD_FZZF) % get8BPrime(tempArray[1])
	tit = (tempArray[2] & maskWORD_ZFFF) % get8BPrime(tempArray[2])
	gaman = (tempArray[3] & maskWORD_FFZZ) % get8BPrime(tempArray[3])

	for i := 0; i < poxPORTION_NUM; i++ {
		alaf >>= poxSINGLE_DIGIT_PRIMES[dalat%uint16(poxSD_PRIME_NUM)]
		dalat = gorda(dalat, 2)
		tit >>= poxSINGLE_DIGIT_PRIMES[gaman%uint16(poxSD_PRIME_NUM)]
		gaman ^= (alaf ^ maskWORD_ZZFF) >> poxSINGLE_DIGIT_PRIMES[tit%uint16(poxSD_PRIME_NUM)]
	}

	tempArrayCpy := copyWordArray(tempArray)

	tempArrayCpy[1] ^= tempArray[2] % poxMAGIC_PRIMES[alaf%poxMAGIC_PRIME_NUM]
	tempArrayCpy[2] ^= alaf + tit
	tempArrayCpy[3] ^= tit + gaman

	return tempArrayCpy
}

func poxTheta(tempArray factorType) factorType {
	var alef, dalet, tet, gimmel, ctm, stm uint16 = 0, 0, 0, 0, 0, 0

	alef = tempArray[0] % 2
	dalet = tempArray[1] % 2
	tet = tempArray[2] % 2
	gimmel = tempArray[3] % 2

	ctm = centum(tempArray, factorType{alef, dalet, tet, gimmel})
	stm = satem(tempArray, factorType{alef, dalet, tet, gimmel})

	tempArrayCpy := copyWordArray(tempArray)

	tempArrayCpy[0] ^= ((ctm >> gimmel) ^ maskWORD_ZZFF) & maskWORD_ZZZF
	tempArrayCpy[3] ^= ((stm << alef) ^ maskWORD_FZFZ) & maskWORD_FZZZ

	return tempArrayCpy
}

func poxGamma(tempArray factorType) factorType {
	var alaph, dalath, teth, gamal uint16 = 0, 0, 0, 0

	mmax, argmax := maxAndArgmax(tempArray)
	mmin, argmin := minAndArgmin(tempArray)
	ay := argmin & maskNIBBLET_01
	dee := argmax ^ maskNIBBLET_10
	thorn := argmin & maskNIBBLET_11
	gee := argmax ^ maskNIBBLET_00

	alaph = tempArray[ay] % get8BPrime(tempArray[thorn])
	dalath = (get8BPrime(mmax) ^ maskWORD_ZFZF) % get8BPrime(mmin)
	teth = mmax % get8BPrime(mmax)
	gamal = tempArray[dee] % get8BPrime(uint16((uint32(mmin)+uint32(mmax))/2))

	tempArrayCpy := copyWordArray(tempArray)

	tempArrayCpy[ay] >>= (alaph ^ maskWORD_ZZFZ) % bitWORD_WIDTH_U16
	tempArrayCpy[dee] >>= (gamal ^ maskWORD_FZZZ) % ((mmax % 2) + 1)
	tempArrayCpy[thorn] ^= log2N(dalath) & maskWORD_ZFFF
	tempArrayCpy[gee] ^= log2N(teth) >> ((gamal % 2) + 1)

	return tempArrayCpy
}

func poxRoundApplyAlphabet(tempArray factorType) factorType {
	tempArrayCpy := copyWordArray(tempArray)
	tempArrayCpy = poxAlpha(tempArrayCpy)
	tempArrayCpy = poxDelta(tempArrayCpy)
	tempArrayCpy = poxTheta(tempArrayCpy)
	tempArrayCpy = poxGamma(tempArrayCpy)

	return tempArrayCpy
}

func poxRoundApplyPrime(tempArray factorType) factorType {
	tempArrayCpy := copyWordArray(tempArray)
	for i := 0; i < poxROUND_PRIME_NUM; i++ {
		tempArrayCpy[0] ^= poxROUND_PRIMES[i]
		tempArrayCpy[1] &= poxROUND_PRIMES[i]
		tempArrayCpy[2] ^= poxROUND_PRIMES[i]
		tempArrayCpy[3] &= poxROUND_PRIMES[i]
	}

	return tempArrayCpy
}

func poxRoundApplyShuffle(tempArray factorType) factorType {
	tempArrayCpy := copyWordArray(tempArray)
	var iof, iwith int = 0, 0
	var tmp uint16
	for i := 0; i < numCOMB_BIONOM; i++ {
		iof, iwith = iterCOMB_BIONOM[i][0], iterCOMB_BIONOM[i][1]
		tmp = tempArrayCpy[iof]
		tempArrayCpy[iof] = tempArrayCpy[iwith]
		tempArrayCpy[iwith] = tmp
	}
	return tempArrayCpy
}

func poxRoundApplyAddition(factorArray, tempArray factorType) factorType {
	factorArrayCpy := copyWordArray(factorArray)
	for i := 0; i < poxPORTION_NUM; i++ {
		factorArrayCpy[i] = tasu(factorArrayCpy[i], tempArray[i])
	}
	return factorArrayCpy
}

func poxRound(factorArray factorType) factorType {
	tempArray := copyWordArray(factorArray)

	tempArray = poxRoundApplyAlphabet(tempArray)
	tempArray = poxRoundApplyPrime(tempArray)
	tempArray = poxRoundApplyShuffle(tempArray)
	additionResult := poxRoundApplyAddition(factorArray, tempArray)

	return additionResult
}

func poxApplyBytes(factorArray, portion factorType, index uint16) factorType {
	var tmt, dca uint16 = 0, 0

	tmt = tamaam(portion)
	dca = deca(portion)
	tmtOddFactor := bitUINT16_MAX_U16 * (tmt % 2)
	dcaOddFactor := bitUINT16_MAX_U16 * (dca % 2)

	ng := (portion[0] + index) % poxPORTION_NUM
	chu := (portion[1] + index) % poxPORTION_NUM
	yo := (portion[2] + index) % poxPORTION_NUM
	eo := (portion[3] + index) % poxPORTION_NUM

	factorArrayCpy := copyWordArray(factorArray)

	factorArrayCpy[ng] ^= (portion[eo] | tmt) ^ dcaOddFactor
	factorArrayCpy[chu] ^= (portion[yo] & dca) ^ tmtOddFactor
	factorArrayCpy[yo] ^= (portion[chu] ^ tmt) ^ dcaOddFactor
	factorArrayCpy[eo] ^= (portion[ng] | dca) ^ tmtOddFactor

	return factorArrayCpy
}

func poxProcessBlock(factorArray factorType, block blockType) factorType {
	factorArrayCpy := copyWordArray(factorArray)
	for i := 0; i < poxBLOCK_NUM; i += poxCHUNK_NUM {
		for j := i; j < i+poxCHUNK_NUM; j += poxPORTION_NUM {
			portion := newPortion(block, j)
			var z uint16 = 0
			for z < poxROUND_NUM {
				factorArrayCpy = poxApplyBytes(factorArrayCpy, portion, z)
				factorArrayCpy = poxRound(factorArrayCpy)
				z++
			}
		}
	}
	return factorArrayCpy
}

type PoxDigest struct {
	Sexdigest string    `json:"sexdigest"`
	Vigdigest string    `json:"vigdigest"`
	Hexdigest string    `json:"hexdigest"`
	Tetdigest string    `json:"tetdigest"`
	Duodigest string    `json:"duodigest"`
	Octdigest string    `json:"octdigest"`
	Sendigest string    `json:"sendigest"`
	Bindigest string    `json:"bindigest"`
	Bytes     [8]uint8  `json:"bytes"`
	Words     [4]uint16 `json:"words"`
	Doubles   [2]uint32 `json:"doubles"`
	Quad      uint64    `json:"quad"`
}

func PoxHash(message []uint8) PoxDigest {
	// Converts the given message to a PoxDigest object
	// Parameters:
	//		message: []uint8
	//
	// Returns:
	//		PoxDigest
	//			PoxDigest.Sexdigest: string
	//			PoxDigest.Vigdigest: string
	//			PoxDigest.Hexdigest: string
	//			PoxDigest.Tetdigest: string
	//			PoxDigest.Duodigest: string
	//			PoxDigest.Octdigest: string
	//			PoxDigest.Sendigest: string
	//			PoxDigest.Bindigest: string
	//			PoxDigest.Bytes: [8]uint8
	//			PoxDigest.Words: [4]uint16
	//			PoxDigest.Doubles [2]uint32
	//			PoxDigest.Quad 	uint64
	padded := byteArrToWordArrAndPad(message)
	factorArray := newFactorArray()

	for i := 0; i < len(padded); i += poxBLOCK_NUM {
		block := newBlock(padded, i)
		factorArray = poxProcessBlock(factorArray, block)
	}

	sexdigest := wordArrToSexDigest(factorArray)
	vigdigest := wordArrToVigDigest(factorArray)
	hexdigest := wordArrToHexDigest(factorArray)
	tetdigest := wordArrToTetDigest(factorArray)
	duodigest := wordArrToDuoDigest(factorArray)
	octdigest := wordArrToOctDigest(factorArray)
	sendigest := wordArrToSenDigest(factorArray)
	bindigest := wordArrToBinDigest(factorArray)
	bytes := wordArrToByteArr(factorArray)
	doubles := wordArrayToDoubleArray(factorArray)
	quad := wordToQuad(factorArray[0], factorArray[1], factorArray[2], factorArray[3])

	return PoxDigest{
		Sexdigest: sexdigest,
		Vigdigest: vigdigest,
		Hexdigest: hexdigest,
		Tetdigest: tetdigest,
		Duodigest: duodigest,
		Octdigest: octdigest,
		Sendigest: sendigest,
		Bindigest: bindigest,
		Bytes:     bytes,
		Words:     factorArray,
		Doubles:   doubles,
		Quad:      quad,
	}
}
