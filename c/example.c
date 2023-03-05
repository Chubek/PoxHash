#include <stdio.h>
#include "libpox.h"

int main(int argc, char **argv) {
    printf("Pox hashes for passed strings in hexdigest form:\n");
    for (int i = 0; i < argc; i++) {
        poxhash_t hash = pox_hash(argv[i]);
        printf("\t%d -> %s\n", i + 1, hash.hexdigest);
    }

    return 0;
} 