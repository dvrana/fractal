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
    terrain_t* T = new_terrain(10,1<<12);

    // Write diamond-square
    if (!draw_terrain(T,"hello.png",1<<10,1<<10))
        printf("Drawing failed!\n");

    // Clean up
    free_terrain(T);

    return 0;
}
