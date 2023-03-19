/////////////////////////////////////////////////////////////////////////////////////
//                       Runner for libpoxh.go (PoxHash Go)                        //
//                          March 2023 - Chubak Bidpaa                             //
/////////////////////////////////////////////////////////////////////////////////////
// MIT License                                                                     //
//                                                                                 //
// Copyright (c) 2023 Chubak Bidpaa                                                //
//                                                                                 //
// Permission is hereby granted, free of charge, to any person obtaining a copy    //
// of this software and associated documentation files (the "Software"), to deal   //
// in the Software without restriction, including without limitation the rights    //
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       //
// copies of the Software, and to permit persons to whom the Software is           //
// furnished to do so, subject to the following conditions:                        //
//                                                                                 //
// The above copyright notice and this permission notice shall be included in all  //
// copies or substantial portions of the Software.                                 //
//                                                                                 //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     //
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   //
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   //
// SOFTWARE.                                                                       //
/////////////////////////////////////////////////////////////////////////////////////

package main

import (
	"errors"
	"fmt"
	"math"
	"os"
	"pox/libpoxh"
	"strconv"
	"strings"
	"time"
	"unicode"
)

const (
	sizeMIN_FLAGS       = 3
	sizeMAX_FLAGS       = 24
	numMIN_ARGS         = 3
	numASCII            = 128
	indexBENCHMARK_BYTE = 94

	flagBENCHMARK   byte = 94
	flagJOIN        byte = 43
	flagEVERTHING   byte = 42
	flagALL_NON_DEC byte = 78
	flagALL_DECIMAL byte = 68
	flagBYTES       byte = 56
	flagWORDS       byte = 52
	flagDOUBLES     byte = 50
	flagQUAD        byte = 49
	flagSEX         byte = 103
	flagVIG         byte = 118
	flagHEX         byte = 104
	flagTET         byte = 116
	flagDUO         byte = 100
	flagOCT         byte = 111
	flagSEN         byte = 115
	flagBIN         byte = 98
	flagNS          byte = 57
	flagUS          byte = 54
	flagMS          byte = 51
	flagSS          byte = 53
	flagMM          byte = 48
	flagHELP        byte = 63
	flagDASH        byte = 45
	flagNHEADER     byte = 122
	flagECHO        byte = 101

	filePREFIX     = "file="
	filePREFIX_LEN = 5

	intPREFIX     = "int="
	intPREFIX_LEN = 4

	maxHEX = 2
	maxOCT = 5
	maxBIN = 8
	maxU8  = 255

	prefixBIN = "0b"
	prefixOCT = "0o"
	prefixHEX = "0x"

	basePREFIX_NUM = 2

	nsTO_NS int64 = 100
	nsTO_US int64 = 1000
	nsTO_MS int64 = 1000000
	nsTO_SS int64 = 1000000000
	nsTO_MM int64 = 60000000000

	eNOT_TRUNC_LEN int = 4
)

var wrongFLAGS = [][2]byte{
	{71, 103},
	{86, 118},
	{79, 111},
	{84, 116},
	{83, 115},
	{72, 104},
	{110, 78},
	{87, 52},
	{119, 52},
	{113, 49},
	{81, 49},
	{47, 63},
	{61, 43},
	{66, 98},
	{69, 42},
	{65, 42},
	{62, 63},
	{38, 42},
	{114, 116},
	{121, 116},
	{102, 103},
	{120, 104},
	{90, 122},
	{97, 122},
	{69, 101},
	{119, 101},
	{114, 101},
	{105, 101},
}

func toENotation(numIn float64, places int) string {
	num := math.Abs(float64(numIn))
	if num > 1.0 {
		numStr := fmt.Sprintf("%f", num)
		indexOfPeriod := strings.Index(numStr, ".")
		e := indexOfPeriod - 1
		firstDigit := numStr[0]
		truncs := ""
		strBytes := []byte(numStr)
		for _, c := range strBytes[1 : places+1] {
			if c == 46 {
				continue
			}
			truncs = fmt.Sprintf("%s%c", truncs, c)
		}

		if len(truncs) < places {
			for i := 0; i < places-len(truncs); i++ {
				truncs = fmt.Sprintf("%s%c", truncs, 48)
			}
		}

		var eStr string
		if e > 9 {
			eStr = fmt.Sprintf("%d", e)
		} else {
			eStr = fmt.Sprintf("0%d", e)
		}

		return fmt.Sprintf("%c.%se+%s", byte(firstDigit), truncs, eStr)
	} else if num > 0.0 && num < 1.0 {
		numStr := fmt.Sprintf("%f", num)
		firstNonZeroIndex := 0
		truncs := ""
		var firstDigit byte = 0
		strBytes := []byte(numStr)
		for i, c := range strBytes {
			if c != 48 && c != 46 && firstNonZeroIndex == 0 {
				firstNonZeroIndex = i
				firstDigit = c
				continue
			}

			if firstNonZeroIndex != 0 && len(truncs) < places {
				truncs = fmt.Sprintf("%s%c", truncs, c)
			}
		}

		if len(truncs) < places {
			for i := 0; i < places-len(truncs); i++ {
				truncs = fmt.Sprintf("%s%c", truncs, 48)
			}
		}

		e := firstNonZeroIndex - 1

		var eStr string
		if e > 9 {
			eStr = fmt.Sprintf("%d", e)
		} else {
			eStr = fmt.Sprintf("0%d", e)
		}

		return fmt.Sprintf("%c.%se-%s", byte(firstDigit), truncs, eStr)
	} else {
		return fmt.Sprintf("%f", num)
	}
}

func errorOut(message string) {
	os.Stderr.WriteString("\n")
	os.Stderr.WriteString(message)
	os.Stderr.WriteString("\n")
	os.Stderr.WriteString("\033[1;31mError ocurred\033[0m. Please pass \033[1;34m-?-\033[0m to show help\n")
	os.Exit(1)
}

func printHelp(exec string) {
	fmt.Printf("\033[1;30;42mHelp | Chubak#7400 (Discord) | @bidpaafx (Telegram) | Chubakbidpaa[at]gmail[dot]com\033[0m\n")
	fmt.Println()
	fmt.Printf("Examples \033[1m(flag go between two dashes!)\033[0m:\n")
	fmt.Printf("%s -N82- myword1\n", exec)
	fmt.Printf("%s -*+^- mywod to be joined\n", exec)
	fmt.Printf("%s -Dhob- word1 word 2\n", exec)
	fmt.Printf("%s -^^+- large seq  to join and  benchmark\n", exec)
	fmt.Printf("wget -qO- www.example.com | xargs bash -c '%s -h+- $@'\n", exec)
	fmt.Printf("If an argument stats with `%s`, it will lead to file read attempt, unless `%c` is passed\n", filePREFIX, flagJOIN)
	fmt.Printf("If an argument stats with `%s`, it will parse the int, prefixes 0b, 0o and 0x for bin, oct and hex and none for decimal apply\n", intPREFIX)
	fmt.Println()
	fmt.Printf("\033[1;32mFlags:\033[0m\n")
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Echo argument\n", flagECHO)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Don't print header message\n", flagNHEADER)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Benchmark run (pass two to only show benchmark)\n", flagBENCHMARK)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Join arguments with space (byte 32)\n", flagJOIN)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Print every digest\n", flagEVERTHING)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Print every non-decimal digest\n", flagALL_NON_DEC)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Print every decimal digest\n", flagALL_DECIMAL)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Print bytes digest (eight unsigned 8-bit integers)\n", flagBYTES)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Print words digest (four unsigned 16-bit integers)\n", flagWORDS)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Print doubles digest (two unsigned 32-bit integers)\n", flagDOUBLES)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Print quad digest (one unsigned 64-bit integer)\n", flagQUAD)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Print sexagesimal digest (base sixty)\n", flagSEX)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Print vigesimal digest (base twenty)\n", flagVIG)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Print hexadecimal digest (base sixteen)\n", flagHEX)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Print tetradecimal digest (base fourteen)\n", flagTET)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Print duodecimal digest (base twelve)\n", flagDUO)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Print octal digest (base eight)\n", flagOCT)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Print senary digest (base six)\n", flagSEN)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Print binary digest (base two)\n", flagBIN)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Print total time in nanoseconds\n", flagNS)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Print total time in mictoseconds\n", flagUS)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Print total time in milliseconds\n", flagMS)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Print total time in seconds\n", flagSS)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Print total time in minutes\n", flagMM)
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Print Help\n\n", flagHELP)
	os.Exit(1)
}

func isAllDigit(numStr string) bool {
	for _, c := range numStr {
		if !unicode.IsDigit(c) {
			return false
		}
	}
	return true
}

func getExecName(args0 string) string {
	slashIndex := 0
	for i := len(args0) - 1; i >= 0; i-- {
		if args0[i] == '/' {
			break
		}
		slashIndex = i
	}
	return string([]byte(args0[slashIndex:]))
}

func checkForWrongFlags(flags []byte) {
	for _, flag := range flags {
		for _, wrongFlagTuple := range wrongFLAGS {
			wrongFlag, rightFlag := wrongFlagTuple[0], wrongFlagTuple[1]
			if flag == wrongFlag {
				fmt.Printf("No flag for `%c`, perhaps you meant `%c`?", flag, rightFlag)
				errorOut("Flag error")
			}
		}
	}
}

func argHasFlag(argBytes []byte, flg byte) bool {
	for _, b := range argBytes {
		if b == byte(flg) {
			return true
		}
	}
	return false
}

func searchFlagsForOccurance(flagBytes []byte) byte {
	var occuranceArray [numASCII]byte
	for _, b := range flagBytes {
		occuranceArray[b] += 1
	}

	var ret byte = 0

	if occuranceArray[indexBENCHMARK_BYTE] == 2 {
		ret = '^'
	}

	if occuranceArray[indexBENCHMARK_BYTE] > 2 {
		errorOut("`^` can appear at most twice")
	}

	for i, sum := range occuranceArray {
		if i == indexBENCHMARK_BYTE || i == 45 {
			continue
		}
		if sum > 1 {
			ret = byte(i)
			return ret
		}
	}

	return ret
}

func validateFlags(lenArgs int, args []string) {
	if lenArgs < numMIN_ARGS-1 {
		errorOut("No flag passed")
	}

	argFlagsBytes := []byte(args[1])
	lenFlags := len(argFlagsBytes)
	if lenFlags < sizeMIN_FLAGS || lenFlags > sizeMAX_FLAGS {
		errorOut("Length of the first argument must at least be 3 and at most 24")
	}

	if argFlagsBytes[0] != 45 || argFlagsBytes[lenFlags-1] != 45 {
		errorOut("The flag argument must begin and end with `-`")
	}

	checkForWrongFlags(argFlagsBytes)

	execName := getExecName(args[0])
	if args[1] == "-?-" {
		printHelp(execName)
	}

	helpPassed := argHasFlag(argFlagsBytes, flagHELP)
	if helpPassed && lenFlags > sizeMIN_FLAGS {
		errorOut("You may not pass the `?` flag along with other flag")
	}

	reoccurrance := searchFlagsForOccurance(argFlagsBytes)
	if reoccurrance != 0 && reoccurrance != byte(flagBENCHMARK) {
		fmt.Printf("Flag `%c` appears twice", reoccurrance)
		errorOut("Only `^` can appear twice")
	}
	doubleBenchmark := reoccurrance == flagBENCHMARK

	if lenArgs < numMIN_ARGS {
		errorOut("You must pass at least one argument to hash")
	}

	allFlagsPassed := argHasFlag(argFlagsBytes, flagEVERTHING)
	allFlagsDecPassed := argHasFlag(argFlagsBytes, flagALL_DECIMAL)
	allFlagsNondecPassed := argHasFlag(argFlagsBytes, flagALL_NON_DEC)
	benchmarkHasPassed := argHasFlag(argFlagsBytes, flagBENCHMARK)

	for _, flag := range argFlagsBytes[1 : len(argFlagsBytes)-1] {
		switch flag {
		case flagBENCHMARK, flagJOIN, flagNHEADER, flagECHO:
			continue
		case flagNS, flagUS, flagMS, flagSS, flagMM:
			if !benchmarkHasPassed {
				errorOut("When a timestamp flag has passed, `^` must be passed as well")
			} else if doubleBenchmark {
				errorOut("When double benchmark (`^^`) is passed, you may not pass a timestamp flag")
			}
			continue
		case flagEVERTHING:
			if allFlagsDecPassed || allFlagsNondecPassed {
				errorOut("You may not pass `*` when you have passed `N` or `D`")
			}
			continue
		case flagALL_NON_DEC:
			if allFlagsPassed {
				errorOut("You may not pass `N` when `*` is passed")
			}
			continue
		case flagALL_DECIMAL:
			if allFlagsPassed {
				errorOut("You may not pass `D` when `*` is passed")
			}
			continue
		case flagBYTES:
			if allFlagsDecPassed || allFlagsPassed {
				errorOut("You may not pass a decimal digest flag when `*` or `D` is passed")
			}
			continue
		case flagWORDS:
			if allFlagsDecPassed || allFlagsPassed {
				errorOut("You may not pass a decimal digest flag when `*` or `D` is passed")
			}
			continue
		case flagDOUBLES:
			if allFlagsDecPassed || allFlagsPassed {
				errorOut("You may not pass a decimal digest flag when `*` or `D` is passed")
			}
			continue
		case flagQUAD:
			if allFlagsDecPassed || allFlagsPassed {
				errorOut("You may not pass a decimal digest flag when `*` or `D` is passed")
			}
			continue
		case flagSEX:
			if allFlagsNondecPassed || allFlagsPassed {
				errorOut("You may not pass a non-decimal digest flag when `*` or `N` is passed")
			}
			continue
		case flagVIG:
			if allFlagsNondecPassed || allFlagsPassed {
				errorOut("You may not pass a non-decimal digest flag when `*` or `N` is passed")
			}
			continue
		case flagHEX:
			if allFlagsNondecPassed || allFlagsPassed {
				errorOut("You may not pass a non-decimal digest flag when `*` or `N` is passed")
			}
			continue
		case flagTET:
			if allFlagsNondecPassed || allFlagsPassed {
				errorOut("You may not pass a non-decimal digest flag when `*` or `N` is passed")
			}
			continue
		case flagDUO:
			if allFlagsNondecPassed || allFlagsPassed {
				errorOut("You may not pass a non-decimal digest flag when `*` or `N` is passed")
			}
			continue
		case flagOCT:
			if allFlagsNondecPassed || allFlagsPassed {
				errorOut("You may not pass a non-decimal digest flag when `*` or `N` is passed")
			}
			continue
		case flagSEN:
			if allFlagsNondecPassed || allFlagsPassed {
				errorOut("You may not pass a non-decimal digest flag when `*` or `N` is passed")
			}
			continue
		case flagBIN:
			if allFlagsNondecPassed || allFlagsPassed {
				errorOut("You may not pass a non-decimal digest flag when `*` or `N` is passed")
			}
			continue
		case flagHELP:
			if lenFlags > sizeMIN_FLAGS {
				errorOut("You may not pass the `?` flag along with other flag")
			}
		case flagDASH:
			errorOut("You may not use `-` in the first argument other than in the first, and the last letter")
		default:
			errorOut("Unknown flag detected!")
		}
	}
}

func getTimeInNS() int64 {
	return time.Now().UnixNano()
}

func convertTime(time, divisor int64) string {
	return toENotation(float64(time)/float64(divisor), eNOT_TRUNC_LEN)
}

func allAreFalse(bools []bool) bool {

	for _, bl := range bools {
		if bl {
			return false
		}
	}
	return true
}

func printHashes(hashes []libpoxh.PoxDigest, flags []byte, totalTime int64) {
	lenHashes := len(hashes)
	reoccurrance := searchFlagsForOccurance(flags)
	doubleBenchmark := reoccurrance == flagBENCHMARK

	if argHasFlag(flags, flagBENCHMARK) {
		fmt.Printf("| %d Message(s) ||", lenHashes)
		hasPrinted := false
		if argHasFlag(flags, flagNS) || doubleBenchmark {
			fmt.Printf(" %sns |", convertTime(totalTime, nsTO_NS))
			hasPrinted = true
		}
		if argHasFlag(flags, flagUS) || doubleBenchmark {
			fmt.Printf(" %sus |", convertTime(totalTime, nsTO_US))
			hasPrinted = true
		}
		if argHasFlag(flags, flagMS) || doubleBenchmark {
			fmt.Printf(" %sms |", convertTime(totalTime, nsTO_MS))
			hasPrinted = true
		}
		if argHasFlag(flags, flagSS) || doubleBenchmark {
			fmt.Printf(" %ss |", convertTime(totalTime, nsTO_SS))
			hasPrinted = true
		}
		if argHasFlag(flags, flagMM) || doubleBenchmark {
			fmt.Printf(" %sm |", convertTime(totalTime, nsTO_MM))
			hasPrinted = true
		}
		if !hasPrinted {
			fmt.Printf(" %sus |", convertTime(totalTime, nsTO_US))
		}
		fmt.Println()
	}

	if doubleBenchmark {
		fmt.Println()
		os.Exit(0)
	}

	everything := argHasFlag(flags, flagEVERTHING)
	allFlagsDecimal := argHasFlag(flags, flagALL_DECIMAL)
	allFLagsNondecimal := argHasFlag(flags, flagALL_NON_DEC)
	byte := argHasFlag(flags, flagBYTES)
	word := argHasFlag(flags, flagWORDS)
	dub := argHasFlag(flags, flagDOUBLES)
	quad := argHasFlag(flags, flagQUAD)
	sex := argHasFlag(flags, flagSEX)
	vig := argHasFlag(flags, flagVIG)
	hex := argHasFlag(flags, flagHEX)
	tet := argHasFlag(flags, flagTET)
	duo := argHasFlag(flags, flagDUO)
	oct := argHasFlag(flags, flagOCT)
	sen := argHasFlag(flags, flagSEN)
	bin := argHasFlag(flags, flagBIN)

	allFalse := allAreFalse(
		[]bool{
			everything,
			allFlagsDecimal,
			allFLagsNondecimal,
			byte,
			word,
			dub,
			quad,
			sex,
			vig,
			hex,
			tet,
			duo,
			oct,
			sen,
			bin,
		})

	if allFalse {
		errorOut("You have not specfied any digests to be printed. Please pass at least one, or `*` for all")
	}

	for _, hash := range hashes {
		if everything || allFlagsDecimal || byte {
			fmt.Printf("Bytes: U8[%d, %d, %d, %d, %d, %d, %d, %d]\n", hash.Bytes[0], hash.Bytes[1], hash.Bytes[2], hash.Bytes[3], hash.Bytes[4], hash.Bytes[5], hash.Bytes[6], hash.Bytes[7])
		}
		if everything || allFlagsDecimal || word {
			fmt.Printf("Words: U16[%d, %d, %d, %d]\n", hash.Words[0], hash.Words[1], hash.Words[2], hash.Words[3])
		}
		if everything || allFlagsDecimal || dub {
			fmt.Printf("Doubles: U32[%d, %d]\n", hash.Doubles[0], hash.Doubles[1])
		}
		if everything || allFlagsDecimal || quad {
			fmt.Printf("Quad: U64[%d]\n", hash.Quad)
		}
		if everything || allFLagsNondecimal || sex {
			fmt.Printf("Sexdigest: %s\n", hash.Sexdigest)
		}
		if everything || allFLagsNondecimal || vig {
			fmt.Printf("Vigdigest: %s\n", hash.Vigdigest)
		}
		if everything || allFLagsNondecimal || hex {
			fmt.Printf("Hexdigest: %s\n", hash.Hexdigest)
		}
		if everything || allFLagsNondecimal || tet {
			fmt.Printf("Tetdigest: %s\n", hash.Tetdigest)
		}
		if everything || allFLagsNondecimal || duo {
			fmt.Printf("Duodigest: %s\n", hash.Duodigest)
		}
		if everything || allFLagsNondecimal || oct {
			fmt.Printf("Octdigest: %s\n", hash.Octdigest)
		}
		if everything || allFLagsNondecimal || sen {
			fmt.Printf("Sendgiest: %s\n", hash.Sendigest)
		}
		if everything || allFLagsNondecimal || bin {
			fmt.Printf("Bindigest: %s\n", hash.Bindigest)
		}
		fmt.Printf("\033[2m----\033[0m\n")
	}
}

func assertFile(arg string) bool {
	return len(arg) > filePREFIX_LEN && strings.HasPrefix(arg, filePREFIX)
}

func assertInt(arg string) bool {
	return len(arg) > intPREFIX_LEN && strings.HasPrefix(arg, intPREFIX)
}

func toInt(arg string) []uint8 {
	split := strings.Split(arg, ",")

	var result []uint8
	var convt uint64
	var err error
	for _, num := range split {
		sansPrefix := string(num[basePREFIX_NUM:])
		prefix := string(num[:basePREFIX_NUM])
		switch prefix {
		case prefixBIN:
			if len(sansPrefix) > maxBIN {
				errorOut("Size of binary number should not exceed 8")
			}
			convt, err = strconv.ParseUint(sansPrefix, 2, 8)
			result = append(result, uint8(convt))
			break
		case prefixOCT:
			if len(sansPrefix) > maxOCT {
				errorOut("Size of octal number should not exceed 5")
			}
			convt, err = strconv.ParseUint(sansPrefix, 8, 8)
			result = append(result, uint8(convt))
			break
		case prefixHEX:
			if len(sansPrefix) > maxOCT {
				errorOut("Size of hexadecimal number should not exceed 2")
			}
			convt, err = strconv.ParseUint(sansPrefix, 16, 8)
			result = append(result, uint8(convt))
			break
		default:
			if !isAllDigit(num) {
				errorOut("With 'int=' prefix you must pass byte-sized integers in base 16, 8, 10 and 2")
			}
			convt, err = strconv.ParseUint(num, 10, 8)
			if convt > maxU8 {
				errorOut("Given integer must be byte-sized (0-255)")
			}
			result = append(result, uint8(convt))
			if err != nil {
				errorOut(fmt.Sprintf("%s", err))
			}
			break
		}
	}
	return result
}

func joinArgs(argsSlicedAfterTwo []string) string {
	joined := ""
	warned := false
	for _, arg := range argsSlicedAfterTwo {
		if assertFile(arg) && !warned {
			fmt.Printf("\033[1;33mWarning:\033[0m: The `filepath=` prefix is ignored in join mode\n")
			warned = true
		}

		joined += arg
		joined += " "
	}
	joined = string([]byte(joined)[:len(joined)-1])
	return joined
}

func isRegularFile(fpath string) {
	stats, err := os.Stat(fpath)

	if errors.Is(err, os.ErrNotExist) || stats.IsDir() {
		errorOut("Specfied file does not exist or is a directory. Pass `+` with only one argument to ignore")
	}
}

func readGivenFile(fpath string) []byte {
	isRegularFile(fpath)
	contents, err := os.ReadFile(fpath)

	if err != nil {
		fmt.Println("\033[1;31mError reading file\033[0m")
		fmt.Println(err)
		os.Exit(1)
	}
	return contents
}

func processArg(arg string) []uint8 {
	if !assertFile(arg) && !assertInt(arg) {
		return []uint8(arg)
	} else if assertInt(arg) {
		return toInt(string(arg[intPREFIX_LEN:]))
	}
	return []uint8(readGivenFile(string(arg[filePREFIX_LEN:])))
}

func main() {
	validateFlags(len(os.Args), os.Args)
	flagsByte := []byte(os.Args[1])

	if !argHasFlag(flagsByte, flagNHEADER) {
		fmt.Printf("\033[1;30;47m   PoxHashRunner   |    Go    |  March 2023 - Chubak Bidpaa  |  MIT  \033[0m\n")
	}

	echoArg := argHasFlag(flagsByte, flagECHO)
	hashes := make([]libpoxh.PoxDigest, len(os.Args)-2)
	var totalTime, t1, t2 int64
	if argHasFlag(flagsByte, flagJOIN) {
		argsJoined := joinArgs(os.Args[2:])
		if echoArg {
			fmt.Printf("Joined Args: \n`%s`\n", argsJoined)
		}
		t1 = getTimeInNS()
		hashes[0] = libpoxh.PoxHash([]uint8(argsJoined))
		t2 = getTimeInNS()
		printHashes(hashes[:1], flagsByte, t2-t1)
	} else {
		var processedArg []uint8
		cursor := 0
		for i, arg := range os.Args[2:] {
			if echoArg {
				fmt.Printf("Arg %d: %s\n", i+1, arg)
			}
			processedArg = processArg(arg)
			t1 = getTimeInNS()
			hashes[cursor] = libpoxh.PoxHash(processedArg)
			t2 = getTimeInNS()
			totalTime += t2 - t1
			cursor += 1
		}
		printHashes(hashes, flagsByte, totalTime)
	}
}
