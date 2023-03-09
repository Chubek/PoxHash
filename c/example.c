#include <stdio.h>
#include "libpoxh.h"

int main(int argc, char **argv)
{
    printf("Pox (Header-Only C) hashes for passed strings in various forms:\n");
    for (int i = 1; i < argc; i++)
    {
        poxhash_t hash = pox_hash(argv[i]);
        char *sexdigest = hash.sexdigest;
        char *hexdigest = hash.hexdigest;
        char *duodigest = hash.duodigest;
        char *octdigest = hash.octdigest;
        char *bindigest = hash.bindigest;
        uint8_t *bytes = hash.bytes;
        uint16_t *words = hash.words;
        uint32_t *doubles = hash.doubles;
        uint64_t quad = hash.quad;
        printf("\n");
        printf("\tArg #%u as follows\n", i);
        printf("\t\tsexdigest: %s\n", sexdigest);
        printf("\t\thexdigest: %s\n", hexdigest);
        printf("\t\tduodigest: %s\n", duodigest);
        printf("\t\toctdigest: %s\n", octdigest);
        printf("\t\tbindigest: %s\n", bindigest);
        printf("\t\tbytes: uint8(%u, %u, %u, %u, %u, %u, %u, %u)\n", (uint32_t)bytes[0], (uint32_t)bytes[1], (uint32_t)bytes[2], (uint32_t)bytes[3], (uint32_t)bytes[4], (uint32_t)bytes[5], (uint32_t)bytes[6], (uint32_t)bytes[7]);
        printf("\t\twords: uint16(%hu, %hu, %hu, %hu)\n", words[0], words[1], words[2], words[3]);
        printf("\t\tdoubles: uint32(%u, %u)\n", doubles[0], doubles[1]);
		printf("\t\tquad: uint64(%lu)\n", quad);
        printf("\n");
    }

    return 0;
}