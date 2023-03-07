from libpoxh import pox_hash
import sys

if __name__ == "__main__":
    print("Pox (Python) hashes for passed strings in hexdigest form:")
    for i, arg in enumerate(sys.argv[1:]):
        hash = pox_hash(arg.encode())
        hex = hash.hexdigest
        print(f"\t{i + 1} -> {hex} for\n\t `{arg}`")
