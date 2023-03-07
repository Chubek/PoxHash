#include <stdio.h>
#include "libpoxh.h"

int main(int argc, char **argv)
{
    printf("Pox (Header-Only C) hashes for passed strings in hexdigest form:\n");
    for (int i = 1; i < argc; i++)
    {
        poxhash_t hash = pox_hash(argv[i]);
        printf("\t%d -> %s for\n\t `%s`\n", i, hash.hexdigest, argv[i]);
    }

    return 0;
}