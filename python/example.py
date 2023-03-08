from libpoxh import pox_hash
import sys

if __name__ == "__main__":
    print("Pox (Python) hashes for passed strings in hexdigest, byte and word form:")
    for i, arg in enumerate(sys.argv[1:]):
        hash = pox_hash(arg.encode())
        hexdigest = hash.hexdigest
        bytes = hash.bytes
        words = hash.words
        doubles = hash.doubles
        quad = hash.quad[0]
        print("\n")
        print(f"\tArg #{i + 1} as follows")
        print(f"\t\thexdigest: {hexdigest}")
        print(f"\t\tbytes: uint8({bytes[0]}, {bytes[1]}, {bytes[2]}, {bytes[3]}, {bytes[4]}, {bytes[5]}, {bytes[6]}, {bytes[7]})")
        print(f"\t\twords: uint16({words[0]}, {words[1]}, {words[2]}, {words[3]})")
        print(f"\t\tdoubles: uint32({doubles[0]}, {doubles[1]})")
        print(f"\t\tquad: uint64({quad})")
        print("\n")
