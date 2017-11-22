#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <cairo.h>
#include <time.h>
#include "diamond_square.h"

int main(int argc, char* argv[]) {
    // Seed the rng
    srand(clock());

    // Run diamond-square
    ds_t* T = new_ds(10,1<<12);

    // Write diamond-square
    if (!draw_ds(T,"hello.png",1<<10,1<<10))
        printf("Drawing failed!\n");

    // Clean up
    free_ds(T);

    return 0;
}
