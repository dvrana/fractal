/* Written by Dylan Vrana (dylanvrana@gmail.com)
 *
 * An implementation of the square-diamond algorithm for ds generation
 * 
 *
 */

#ifndef _DIAMONDSQUARE_H_
#define _DIAMONDSQUARE_H_

typedef struct ds ds_t;

bool draw_ds(ds_t* T,const char* filename,int width,int height);

ds_t* new_ds(int steps, int maxh);

void free_ds(ds_t* T);

#endif /* _DIAMONDSQUARE_H_ */
