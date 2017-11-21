/* Written by Dylan Vrana (dylanvrana@gmail.com)
 *
 * An implementation of the square-diamond algorithm for terrain generation
 * 
 *
 */

#ifndef _DIAMONDSQUARE_H_
#define _DIAMONDSQUARE_H_

typedef struct terrain {
    int** topography;
    int maxh;   // Maximum value of a height
    int a_lim;  // Array row count
    int b_lim;  // Array col count
} terrain_t;

bool draw_terrain(terrain_t* T,const char* filename,int width,int height);

terrain_t* new_terrain(int steps, int maxh);

void free_terrain(terrain_t* T);

#endif /* _DIAMONDSQUARE_H_ */
