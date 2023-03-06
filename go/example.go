package main

import (
	"fmt"
	"pox/libpox"
	"os"
)


func main() {
	fmt.Println("Pox (Go) hashes for passed strings in hexdigest form:")
	for i, arg := range os.Args[1:] {
		hash := libpox.PoxHash([]byte(arg))
		fmt.Printf("\t%d -> %s for\n\t `%s`\n", i + 1, hash.Hexdigest, arg)
	}
}
