package libpox

const (
	poxBLOCK_NUM    int = 64
	pox8B_PRIME_NUM int = 54
	poxPRIME_NUM    int = 32
	poxPORTION_NUM  int = 16
	poxROUND_NUM    int = 8
	poxFACT_NUM     int = 4

	poxPRIME_A uint16 = 0x9f91
	poxPRIME_B uint16 = 0xdb3b
	poxPRIME_C uint16 = 0xc091
	poxPRIME_D uint16 = 0xac8b

	bitWORD_WIDTH_U16 uint16 = 16
	bitWORD_WIDTH_U32 uint32 = 16
	bitBYTE_WIDTH_u16 uint16 = 8
	bitUINT16_MAX_U16 uint16 = 65535
	bitUINT16_MAX_U32 uint32 = 65535
	bitBYTE_ARR_SIZE         = 8

	numSD_PRIME    int = 3
	numCOMB_BIONOM int = 6
	numRANGE_ZTF   int = 4
	numHEX_SIZE    int = 4

	maskONE_UPPER16 uint32 = 0xffff0000
	maskONE_LOWER16 uint32 = 0x0000ffff
	maskFZFZ        uint16 = 0xf0f0
	maskZFZF        uint16 = 0x0f0f
	maskFZZZ        uint16 = 0xf000
	maskZZFZ        uint16 = 0x00f0
	maskZZZF        uint16 = 0x000f
	maskZZFF        uint16 = 0x00ff
	maskFFZZ        uint16 = 0xff00
	maskFZZF        uint16 = 0xf00f
	maskFFFZ        uint16 = 0xfff0
	maskZFFF        uint16 = 0x0fff
	mask01          int    = 0b01
	mask10          int    = 0b10
	mask11          int    = 0b11
	mask00          int    = 0b00
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
		80}
)

type factorType [poxFACT_NUM]uint16
type blockType [poxBLOCK_NUM]uint16
type byteType [bitBYTE_ARR_SIZE]uint8

func rotateLeft(num uint16, by uint32) uint16 {
	var res uint32 = uint32(num)
	res = (res << by) | (res >> (bitWORD_WIDTH_U32 - by))

	if res > bitUINT16_MAX_U32 {
		res = (res & maskONE_UPPER16) >> bitWORD_WIDTH_U32
	}

	return uint16(res)
}

func addWithOverFLow(a, b uint16) uint16 {
	var aa, bb uint32 = uint32(a), uint32(b)
	a_plus_b := aa + bb

	if a_plus_b > bitUINT16_MAX_U32 {
		a_plus_b &= maskONE_LOWER16
	}

	return uint16(a_plus_b)
}

func weightedAvg(arr, weights factorType) uint16 {
	var wavg uint32 = 0
	for i, intgr := range arr {
		wavg += uint32(intgr) * uint32(weights[i])
	}
	wavg /= uint32(poxFACT_NUM)

	if wavg > bitUINT16_MAX_U32 {
		wavg = (wavg & maskONE_UPPER16) >> bitWORD_WIDTH_U32
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
		wmed = (wmed & maskONE_UPPER16) >> bitWORD_WIDTH_U32
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
	var lower, upper uint16 = word & maskZZFF, (word & maskFFZZ) >> bitBYTE_WIDTH_u16
	return uint8(lower), uint8(upper)
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

	aleph = (tempArray[0] ^ tempArray[1]) & maskZZFF
	theh = (tempArray[2] ^ tempArray[3]) & maskFFZZ
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

	alaf = (tempArray[0] ^ maskFFFZ) % get8BPrime(tempArray[0])
	dalat = (tempArray[1] ^ maskFZZF) % get8BPrime(tempArray[1])
	tit = (tempArray[2] & maskZFFF) % get8BPrime(tempArray[2])
	gaman = (tempArray[3] & maskFFZZ) % get8BPrime(tempArray[3])

	for i := 0; i < poxFACT_NUM; i++ {
		alaf >>= poxSINGLE_DIGIT_PRIMES[dalat%uint16(numSD_PRIME)]
		dalat = rotateLeft(dalat, 2)
		tit >>= poxSINGLE_DIGIT_PRIMES[gaman%uint16(numSD_PRIME)]
		gaman ^= (alaf ^ maskZZFF) >> poxSINGLE_DIGIT_PRIMES[tit%uint16(numSD_PRIME)]
	}

	tempArrayCpy := copyWordArray(tempArray)

	tempArrayCpy[1] ^= tempArray[2] % poxMAGIC_PRIMES[1]
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

	tempArrayCpy[0] ^= ((wavg >> gimmel) ^ maskZZFF) & maskZZZF
	tempArrayCpy[3] ^= ((wmed << alef) ^ maskFZFZ) & maskFZZZ

	return tempArrayCpy
}

func poxGamma(tempArray factorType) factorType {
	var alaph, dalath, teth, gamal uint16 = 0, 0, 0, 0

	mmax, argmax := maxAndArgmax(tempArray)
	mmin, argmin := minAndArgmin(tempArray)
	ay := argmin & mask01
	dee := argmax ^ mask10
	thorn := argmin & mask11
	gee := argmax ^ mask00

	alaph = tempArray[ay] % get8BPrime(tempArray[thorn])
	dalath = (get8BPrime(mmax) ^ maskZFZF) % get8BPrime(mmin)
	teth = mmax % get8BPrime(mmax)
	gamal = tempArray[dee] % get8BPrime(uint16((uint32(mmin)+uint32(mmax))/2))

	tempArrayCpy := copyWordArray(tempArray)

	tempArrayCpy[ay] >>= (alaph ^ maskZZFZ) % bitWORD_WIDTH_U16
	tempArrayCpy[dee] >>= (gamal ^ maskFZZZ) % ((mmax % 2) + 1)
	tempArrayCpy[thorn] ^= log2N(dalath) & maskZFFF
	tempArrayCpy[gee] ^= log2N(teth) >> ((gamal % 2) + 1)

	return tempArrayCpy
}

func poxRoundApplyOp(tempArray factorType) factorType {
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
	for i := 0; i < poxFACT_NUM; i++ {
		factorArrayCpy[i] = addWithOverFLow(factorArrayCpy[i], tempArray[i])
	}
	return factorArrayCpy
}

func poxRound(factorArray factorType) factorType {
	tempArray := copyWordArray(factorArray)

	tempArray = poxRoundApplyOp(tempArray)
	tempArray = poxRoundApplyPrime(tempArray)
	tempArray = poxRoundApplyShuffle(tempArray)
	additionResult := poxRoundApplyAddition(factorArray, tempArray)

	return additionResult
}

func poxApplyBytes(factorArray, portion factorType) factorType {
	var avg, med, sum uint16 = 0, 0, 0

	sum = sumWordArray(portion)
	avg = sum / uint16(poxFACT_NUM)
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
	for i := 0; i < poxBLOCK_NUM; i += poxPORTION_NUM {
		for j := i; j < i+poxPORTION_NUM; j += poxFACT_NUM {
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
	Hexdigest string     `json:"hexdigest"`
	Bytes     byteType   `json:"bytes"`
	Factors   factorType `json:"factors"`
}

func PoxHash(data []byte) PoxHashTy {
	padded := byteArrToWordArrAndPad(data)
	factorArray := newFactorArray()

	for i := 0; i < len(padded); i += poxBLOCK_NUM {
		block := newBlock(padded, i)
		factorArray = poxProcessBlock(factorArray, block)
	}

	hexdigest := wordArrToHexDigest(factorArray)
	bytes := wordArrToByteArr(factorArray)

	return PoxHashTy{Hexdigest: hexdigest, Bytes: bytes, Factors: factorArray}
}
