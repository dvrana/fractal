#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "lsys.h"

int main(int argc, char* argv[]) {
    // Initialize a L-system of 2 variables
    lsys_t* L = lsys_new(2);
    if (L == NULL) return 1;

    // Clean up
    lsys_free(L);

    return 0;
}
