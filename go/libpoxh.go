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
	poxPRIME_NUM           = 32
	poxCHUNK_NUM           = 16
	poxROUND_NUM           = 8
	poxPORTION_NUM         = 4
	poxSD_PRIME_NUM        = 3
	poxMAGIC_PRIME_NUM     = 2

	poxPRIME_A uint16 = 0x9f91
	poxPRIME_B        = 0xdb3b
	poxPRIME_C        = 0xc091
	poxPRIME_D        = 0xac8b

	bitWORD_WIDTH_U16 uint16 = 16
	bitBYTE_WIDTH_u16        = 8
	bitUINT16_MAX_U16        = 65535
	bitWORD_WIDTH_U32 uint32 = 16
	bitUINT16_MAX_U32        = 65535
	bitBYTE_ARR_SIZE         = 8

	numCOMB_BIONOM = 6
	numRANGE_ZTF   = 4
	numHEX_SIZE    = 4

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
	poxPRIMES = [poxPRIME_NUM]uint16{0xe537, 0xbd71, 0x9ef9, 0xbbcf, 0xf8dd, 0xceb7, 0xbaa1, 0x8f9f, 0xb0ed,
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
	byteHEX            = [bitUINT16_MAX_U32]byte{
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

func lamed(num, by uint32) uint32 {
	return (num << by) | (num >> (bitWORD_WIDTH_U32 - by))
}

func rotateLeft(num uint16, by uint32) uint16 {
	var res uint32 = uint32(num)
	res = lamed(res, by)

	if res > bitUINT16_MAX_U32 {
		res = omega(res)
	}

	return uint16(res)
}

func addWithOverFLow(a, b uint16) uint16 {
	var aa, bb uint32 = uint32(a), uint32(b)
	a_plus_b := aa + bb

	if a_plus_b > bitUINT16_MAX_U32 {
		a_plus_b = epsilon(a_plus_b)
	}

	return uint16(a_plus_b)
}

func weightedAvg(arr, weights factorType) uint16 {
	var wavg uint32 = 0
	for i, intgr := range arr {
		wavg += uint32(intgr) * uint32(weights[i])
	}
	wavg /= uint32(poxPORTION_NUM)

	if wavg > bitUINT16_MAX_U32 {
		wavg = omega(wavg)
	}

	return uint16(wavg)
}

func weightedMed(arr, weights factorType) uint16 {
	var wmed uint32 = 0
	for i, intgr := range arr {
		wmed += uint32(intgr) * uint32(weights[i])
	}
	wmed = (wmed + 1) / 2

	if wmed > bitUINT16_MAX_U32 {
		wmed = epsilon(wmed)
	}

	return uint16(wmed)
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

func decimalToHex(dec uint16) string {
	hex := []byte{byteZERO_CHAR, byteZERO_CHAR, byteZERO_CHAR, byteZERO_CHAR}
	for i := 0; i < numHEX_SIZE; i++ {
		hex[numHEX_SIZE-i-1] = byteHEX[dec%bitWORD_WIDTH_U16]
		dec /= bitWORD_WIDTH_U16
	}
	return string(hex)
}

func wordArrToHexDigest(wordarr factorType) string {
	hex_a := decimalToHex(wordarr[0])
	hex_b := decimalToHex(wordarr[1])
	hex_c := decimalToHex(wordarr[2])
	hex_d := decimalToHex(wordarr[3])
	return hex_a + hex_b + hex_c + hex_d
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

func newBlock(data []uint16, start int) blockType {
	var ret blockType
	z := 0
	for i := start; i < start+poxBLOCK_NUM; i++ {
		ret[z] = data[i]
		z++
	}
	return ret
}

func newFactorArray() factorType {
	var ret factorType
	ret[0] = poxPRIME_A
	ret[1] = poxPRIME_B
	ret[2] = poxPRIME_C
	ret[3] = poxPRIME_D
	return ret
}

func poxAlpha(tempArray factorType) factorType {
	var aleph, theh, daal, gaaf uint16 = 0, 0, 0, 0

	aleph = (tempArray[0] ^ tempArray[1]) & maskWORD_ZZFF
	theh = (tempArray[2] ^ tempArray[3]) & maskWORD_FFZZ
	daal = (aleph | theh) % pox8BPRIMES[0]
	gaaf = (aleph ^ theh) % pox8BPRIMES[1]

	tempArrayCpy := copyWordArray(tempArray)

	tempArrayCpy[0] >>= daal
	tempArrayCpy[1] >>= ((daal + gaaf) % 2) + 1
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
		dalat = rotateLeft(dalat, 2)
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
	var alef, dalet, tet, gimmel, wavg, wmed uint16 = 0, 0, 0, 0, 0, 0

	alef = tempArray[0] % 2
	dalet = tempArray[1] % 2
	tet = tempArray[2] % 2
	gimmel = tempArray[3] % 2

	wavg = weightedAvg(tempArray, factorType{alef, dalet, tet, gimmel})
	wmed = weightedMed(tempArray, factorType{alef, dalet, tet, gimmel})

	tempArrayCpy := copyWordArray(tempArray)

	tempArrayCpy[0] ^= ((wavg >> gimmel) ^ maskWORD_ZZFF) & maskWORD_ZZZF
	tempArrayCpy[3] ^= ((wmed << alef) ^ maskWORD_FZFZ) & maskWORD_FZZZ

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
	for i := 0; i < poxPRIME_NUM; i++ {
		tempArrayCpy[0] ^= poxPRIMES[i]
		tempArrayCpy[1] &= poxPRIMES[i]
		tempArrayCpy[2] ^= poxPRIMES[i]
		tempArrayCpy[3] &= poxPRIMES[i]
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
		factorArrayCpy[i] = addWithOverFLow(factorArrayCpy[i], tempArray[i])
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

func poxApplyBytes(factorArray, portion factorType) factorType {
	var avg, med, sum uint16 = 0, 0, 0

	sum = sumWordArray(portion)
	avg = sum / uint16(poxPORTION_NUM)
	med = (sum + 1) / 2
	avgOddFactor := bitUINT16_MAX_U16 * (avg % 2)
	medOddFactor := bitUINT16_MAX_U16 * (med % 2)

	factorArrayCpy := copyWordArray(factorArray)

	factorArrayCpy[0] ^= (portion[0] + avg) ^ medOddFactor
	factorArrayCpy[1] ^= (portion[1] + med) ^ avgOddFactor
	factorArrayCpy[2] ^= (portion[2] + avg) ^ medOddFactor
	factorArrayCpy[3] ^= (portion[3] + med) ^ avgOddFactor

	return factorArrayCpy
}

func poxProcessBlock(factorArray factorType, block blockType) factorType {
	factorArrayCpy := copyWordArray(factorArray)
	for i := 0; i < poxBLOCK_NUM; i += poxCHUNK_NUM {
		for j := i; j < i+poxCHUNK_NUM; j += poxPORTION_NUM {
			portion := newPortion(block, j)
			z := poxROUND_NUM
			for z > 0 {
				factorArrayCpy = poxApplyBytes(factorArrayCpy, portion)
				factorArrayCpy = poxRound(factorArrayCpy)
				z--
			}
		}
	}
	return factorArrayCpy
}

type PoxHashTy struct {
	Hexdigest string    `json:"hexdigest"`
	Bytes     [8]uint8  `json:"bytes"`
	Words     [4]uint16 `json:"words"`
	Doubles   [2]uint32 `json:"doubles"`
	Quad	  uint64	`json:"quad"`
}

func PoxHash(data []byte) PoxHashTy {
	// Converts the given data to a PoxHashTy object
	// Parameters:
	//		data: []byte
	//
	// Returns:
	//		PoxHashTy
	//			PoxHashTy.Hexdigest: string
	//			PoxHashTy.Bytes: [8]uint8
	//			PoxHashTy.Words: [4]uint16
	//			PoxHashTy.Doubles [2]uint32
	//			PoxHashTy.Quad 	uint64
	padded := byteArrToWordArrAndPad(data)
	factorArray := newFactorArray()

	for i := 0; i < len(padded); i += poxBLOCK_NUM {
		block := newBlock(padded, i)
		factorArray = poxProcessBlock(factorArray, block)
	}

	hexdigest := wordArrToHexDigest(factorArray)
	bytes := wordArrToByteArr(factorArray)
	doubles := wordArrayToDoubleArray(factorArray)
	quad := wordToQuad(factorArray[0], factorArray[1], factorArray[2], factorArray[3])

	return PoxHashTy{
		Hexdigest: hexdigest, 
		Bytes: bytes, 
		Words: factorArray,
		Doubles: doubles,
		Quad: quad,		
	}
}
