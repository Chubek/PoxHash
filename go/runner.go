package main

import (
	"fmt"
	"os"
	"pox/libpoxh"
	"time"
)

const (
	sizeMIN_FLAGS       = 3
	sizeMAX_FLAGS       = 24
	numMIN_ARGS         = 3
	numASCII            = 128
	numWRONG_FLAGS      = 28
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
	flagHELP        byte = 63
	flagDASH        byte = 45
)

var wrongFLAGS = [numWRONG_FLAGS][2]byte{
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
	{51, 50},
	{53, 52},
	{54, 94},
	{55, 56},
	{57, 56},
	{48, 49},
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
}

func errorOut(message string) {
	fmt.Println()
	fmt.Printf(message)
	fmt.Println()
	fmt.Printf("\033[1;31mError ocurred\033[0m. Please pass \033[1;34m-?-\033[0m to show help\n")
	os.Exit(1)
}

func printHelp(exec string) {
	fmt.Printf("\033[1;42mHelp | Chubak#7400 (Discord) | @bidpaafx (Telegram) | Chubakbidpaa[at]gmail\033[0m\n")
	fmt.Println()
	fmt.Printf("Examples \033[1m(flag go between two dashes!)\033[0m:\n")
	fmt.Printf("%s -g^8o- myword1\n", exec)
	fmt.Printf("%s -E+- mywod to be joined\n", exec)
	fmt.Printf("%s -*E- word1 word 2\n", exec)
	fmt.Printf("%s -htd- a_word\n", exec)
	fmt.Println()
	fmt.Printf("\033[1;32mFlags:\033[0m\n")
	fmt.Printf("\033[1;35m\t`^`\033[0m: Benchmark run (pass two to only show benchmark)\n")
	fmt.Printf("\033[1;35m\t`+`\033[0m: Join arguments with space (byte 32)\n")
	fmt.Printf("\033[1;35m\t`*`\033[0m: Print every digest\n")
	fmt.Printf("\033[1;35m\t`N`\033[0m: Print every non-decimal digest\n")
	fmt.Printf("\033[1;35m\t`D`\033[0m: Print every decimal digest\n")
	fmt.Printf("\033[1;35m\t`4`\033[0m: Print bytes digest (eight unsigned 8-bit integers)\n")
	fmt.Printf("\033[1;35m\t`2`\033[0m: Print words digest (four unsigned 16-bit integers)\n")
	fmt.Printf("\033[1;35m\t`1`\033[0m: Print doubles digest (two unsigned 32-bit integers)\n")
	fmt.Printf("\033[1;35m\t`Q`\033[0m: Print quad digest (one unsigned 64-bit integer)\n")
	fmt.Printf("\033[1;35m\t`g`\033[0m: Print sexagesimal digest (base sixty)\n")
	fmt.Printf("\033[1;35m\t`v`\033[0m: Print vigesimal digest (base twenty)\n")
	fmt.Printf("\033[1;35m\t`h`\033[0m: Print hexadecimal digest (base sixteen)\n")
	fmt.Printf("\033[1;35m\t`t`\033[0m: Print tetradecimal digest (base fourteen)\n")
	fmt.Printf("\033[1;35m\t`d`\033[0m: Print duodecimal digest (base twelve)\n")
	fmt.Printf("\033[1;35m\t`o`\033[0m: Print octal digest (base eight)\n")
	fmt.Printf("\033[1;35m\t`s`\033[0m: Print senary digest (base six)\n")
	fmt.Printf("\033[1;35m\t`b`\033[0m: Print binary digest (base two)\n")
	fmt.Printf("\033[1;35m\t`?`\033[0m: Print Help\n\n")
	os.Exit(1)
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

	if lenArgs < numMIN_ARGS {
		errorOut("You must pass at least one argument to hash")
	}

	allFlagsPassed := argHasFlag(argFlagsBytes, flagEVERTHING)
	allFlagsDecPassed := argHasFlag(argFlagsBytes, flagALL_DECIMAL)
	allFlagsNondecPassed := argHasFlag(argFlagsBytes, flagALL_NON_DEC)

	for _, flag := range argFlagsBytes[1 : len(argFlagsBytes)-1] {
		switch flag {
		case flagBENCHMARK:
			continue
		case flagJOIN:
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

func joinArgs(argsSlicedAfterTwo []string) string {
	joined := ""
	for _, arg := range argsSlicedAfterTwo {
		joined += arg
		joined += " "
	}
	joined = string([]byte(joined)[:len(joined)-1])
	return joined
}

func getTimeInUS() int64 {
	return time.Now().UnixMicro()
}

func allAreFalse(bools []bool) bool {

	for _, bl := range bools {
		if bl {
			return false
		}
	}
	return true
}

func printHashes(hashes []libpoxh.PoxDigest, flag []byte, totalTime int64, joined string) {
	if argHasFlag(flag, flagBENCHMARK) {
		fmt.Printf("Total time for hashing %d bytestring(s): %dus\n", len(hashes), totalTime)
	}

	reoccurrance := searchFlagsForOccurance([]byte(flag))
	if reoccurrance == flagBENCHMARK {
		fmt.Println()
		os.Exit(0)
	}

	everything := argHasFlag(flag, flagEVERTHING)
	allFlagsDecimal := argHasFlag(flag, flagALL_DECIMAL)
	allFLagsNondecimal := argHasFlag(flag, flagALL_NON_DEC)
	byte := argHasFlag(flag, flagBYTES)
	word := argHasFlag(flag, flagWORDS)
	dub := argHasFlag(flag, flagDOUBLES)
	quad := argHasFlag(flag, flagQUAD)
	sex := argHasFlag(flag, flagSEX)
	vig := argHasFlag(flag, flagVIG)
	hex := argHasFlag(flag, flagHEX)
	tet := argHasFlag(flag, flagTET)
	duo := argHasFlag(flag, flagTET)
	oct := argHasFlag(flag, flagOCT)
	sen := argHasFlag(flag, flagSEN)
	bin := argHasFlag(flag, flagBIN)

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

	for i, hash := range hashes {
		fmt.Printf("----\n")
		fmt.Printf("Requested digests for bytestring #%d%s\n", i+1, joined)
		if everything || allFlagsDecimal || byte {
			fmt.Printf("\tBytes: U8[%d, %d, %d, %d, %d, %d, %d, %d]\n", hash.Bytes[0], hash.Bytes[1], hash.Bytes[2], hash.Bytes[3], hash.Bytes[4], hash.Bytes[5], hash.Bytes[6], hash.Bytes[7])
		}
		if everything || allFlagsDecimal || word {
			fmt.Printf("\tWords: U16[%d, %d, %d, %d]\n", hash.Words[0], hash.Words[1], hash.Words[2], hash.Words[3])
		}
		if everything || allFlagsDecimal || dub {
			fmt.Printf("\tdoubles: U32[%d, %d]\n", hash.Doubles[0], hash.Doubles[1])
		}
		if everything || allFlagsDecimal || quad {
			fmt.Printf("\tQuad: U64[%lu]\n", hash.Quad)
		}
		if everything || allFLagsNondecimal || sex {
			fmt.Printf("\tSexdigest: %s\n", hash.Sexdigest)
		}
		if everything || allFLagsNondecimal || vig {
			fmt.Printf("\tVigdigest: %s\n", hash.Vigdigest)
		}
		if everything || allFLagsNondecimal || hex {
			fmt.Printf("\tHexdigest: %s\n", hash.Hexdigest)
		}
		if everything || allFLagsNondecimal || tet {
			fmt.Printf("\tTetdigest: %s\n", hash.Tetdigest)
		}
		if everything || allFLagsNondecimal || duo {
			fmt.Printf("\tDuodigest: %s\n", hash.Duodigest)
		}
		if everything || allFLagsNondecimal || oct {
			fmt.Printf("\tOctdigest: %s\n", hash.Octdigest)
		}
		if everything || allFLagsNondecimal || sen {
			fmt.Printf("\tSendgiest: %s\n", hash.Sendigest)
		}
		if everything || allFLagsNondecimal || bin {
			fmt.Printf("\tBindigest: %s\n", hash.Bindigest)
		}
	}
	fmt.Printf("\nFinished run for PoxHash example code (Go implementation)\n")
}

func main() {
	fmt.Printf("\033[1;47mPoxHash   |    Go    |  March 2023 - Chubak Bidpaa  |  GPLv3  \033[0m\n")
	validateFlags(len(os.Args), os.Args)

	hashes := make([]libpoxh.PoxDigest, len(os.Args)-2)
	var totalTime, t1, t2 int64
	flagsByte := []byte(os.Args[1])
	if argHasFlag(flagsByte, flagJOIN) {
		argsJoined := joinArgs(os.Args[2:])
		t1 = getTimeInUS()
		hashes[0] = libpoxh.PoxHash([]uint8(argsJoined))
		t2 = getTimeInUS()
		printHashes(hashes[:1], flagsByte, t2-t1, " (joined arguments):")
	} else {
		cursor := 0
		for _, arg := range os.Args[2:] {
			t1 = getTimeInUS()
			hashes[cursor] = libpoxh.PoxHash([]uint8(arg))
			t2 = getTimeInUS()
			totalTime += t2 - t1
			cursor += 1
		}
		printHashes(hashes, flagsByte, totalTime, ":")
	}
}
