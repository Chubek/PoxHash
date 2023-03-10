package main

import (
	"fmt"
	"os"
	"pox/libpoxh"
)

func main() {
	fmt.Println("Pox (Go) hashes for passed strings in various forms:")
	for i, arg := range os.Args[1:] {
		hash := libpoxh.PoxHash([]byte(arg))
		sexdigest := hash.Sexdigest
		vigdigest := hash.Vigdigest
		hexdigest := hash.Hexdigest
		tetdigest := hash.Tetdigest
		duodigest := hash.Duodigest
		octdigest := hash.Octdigest
		sendigest := hash.Sendigest
		bindigest := hash.Bindigest
		bytes := hash.Bytes
		words := hash.Words
		doubles := hash.Doubles
		quad := hash.Quad
		fmt.Printf("\n")
        fmt.Printf("\tArg #%d as follows\n", i + 1)
		fmt.Printf("\t\tsexdigest: %s\n", sexdigest)
		fmt.Printf("\t\tvigdigest: %s\n", vigdigest)
        fmt.Printf("\t\thexdigest: %s\n", hexdigest)
		fmt.Printf("\t\ttetdigest: %s\n", tetdigest)
		fmt.Printf("\t\tduodigest: %s\n", duodigest)
        fmt.Printf("\t\toctdigest: %s\n", octdigest)
		fmt.Printf("\t\tsendigest: %s\n", sendigest)
        fmt.Printf("\t\tbindigest: %s\n", bindigest)
        fmt.Printf("\t\tbytes: uint8(%d, %d, %d, %d, %d, %d, %d, %d)\n", bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5], bytes[6], bytes[7])
        fmt.Printf("\t\twords: uint16(%d, %d, %d, %d)\n", words[0], words[1], words[2], words[3])
        fmt.Printf("\t\tdoubles: uint32(%d, %d)\n", doubles[0], doubles[1])
		fmt.Printf("\t\tquad: uint64(%d)\n", quad)
		fmt.Printf("\n")
	}
}
