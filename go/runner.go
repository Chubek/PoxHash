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
	"os"
	"pox/libpoxh"
	"time"
)

const (
	sizeMIN_FLAGS       = 3
	sizeMAX_FLAGS       = 24
	numMIN_ARGS         = 3
	numASCII            = 128
	numWRONG_FLAGS      = 34
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
	flagNHEADER     byte = 122
	flagECHO        byte = 101

	filePREFIX     = "file="
	filePREFIX_LEN = 5
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
	{90, 122},
	{97, 122},
	{69, 101},
	{119, 101},
	{114, 101},
	{105, 101},
}

func errorOut(message string) {
	fmt.Println()
	fmt.Printf(message)
	fmt.Println()
	fmt.Printf("\033[1;31mError ocurred\033[0m. Please pass \033[1;34m-?-\033[0m to show help\n")
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
	fmt.Printf("If an argument stats with `%s`, it will lead to file read attempt, unles `%c` is passed\n", filePREFIX, flagJOIN)
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
	fmt.Printf("\033[1;33m\t`%c`\033[0m: Print Help\n\n", flagHELP)
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
		case flagBENCHMARK, flagJOIN, flagNHEADER, flagECHO:
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

func printHashes(hashes []libpoxh.PoxDigest, flag []byte, totalTime int64) {
	if argHasFlag(flag, flagBENCHMARK) {
		fmt.Printf("Total time for hashing %d unsigned bytearrays(s): %dus\n", len(hashes), totalTime)
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
	duo := argHasFlag(flag, flagDUO)
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
		fmt.Printf("----\n")
	}
}

func assertFile(arg string) bool {
	return len(arg) > filePREFIX_LEN && arg[:filePREFIX_LEN] == filePREFIX
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
	if !assertFile(arg) {
		return []uint8(arg)
	}

	return []uint8(readGivenFile(string(arg[filePREFIX_LEN:])))
}

func main() {
	validateFlags(len(os.Args), os.Args)
	flagsByte := []byte(os.Args[1])

	if !argHasFlag(flagsByte, flagNHEADER) {
		fmt.Printf("\033[1;30;47mPoxHashRunner   |    Go    |  March 2023 - Chubak Bidpaa  |  GPLv3  \033[0m\n")
	}

	echoArg := argHasFlag(flagsByte, flagECHO)
	hashes := make([]libpoxh.PoxDigest, len(os.Args)-2)
	var totalTime, t1, t2 int64
	if argHasFlag(flagsByte, flagJOIN) {
		argsJoined := joinArgs(os.Args[2:])
		if echoArg {
			fmt.Printf("Joined Args: \n`%s`\n", argsJoined)
		}
		t1 = getTimeInUS()
		hashes[0] = libpoxh.PoxHash([]uint8(argsJoined))
		t2 = getTimeInUS()
		printHashes(hashes[:1], flagsByte, t2-t1)
	} else {
		var processedArg []uint8
		cursor := 0
		for i, arg := range os.Args[2:] {
			if echoArg {
				fmt.Printf("Arg %d: %s\n", i+1, arg)
			}
			processedArg = processArg(arg)
			t1 = getTimeInUS()
			hashes[cursor] = libpoxh.PoxHash(processedArg)
			t2 = getTimeInUS()
			totalTime += t2 - t1
			cursor += 1
		}
		printHashes(hashes, flagsByte, totalTime)
	}
}
