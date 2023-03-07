package main

import (
	"fmt"
	"os"
	"pox/libpoxh"
)

func main() {
	fmt.Println("Pox (Go) hashes for passed strings in hexdigest, byte and word form:")
	for i, arg := range os.Args[1:] {
		hash := libpoxh.PoxHash([]byte(arg))
		hexdigest := hash.Hexdigest
		bytes := hash.Bytes
		words := hash.Words
		fmt.Printf("\n")
        fmt.Printf("\tArg #%d as follows\n", i + 1)
        fmt.Printf("\t\thexdigest: %s\n", hexdigest)
        fmt.Printf("\t\tbytes: uint8(%d, %d, %d, %d, %d, %d, %d, %d)\n", bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5], bytes[6], bytes[7])
        fmt.Printf("\t\twords: uint16(%d, %d, %d, %d)\n", words[0], words[1], words[2], words[3])
        fmt.Printf("\n")
	}
}
