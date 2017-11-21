/* Written by Dylan Vrana (dylanvrana@gmail.com)
 *
 * An implementation of the square-diamond algorithm for terrain generation
 * 
 *
 */

#ifndef _DIAMONDSQUARE_H_
#define _DIAMONDSQUARE_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <cairo.h>

// Maximum and minimum intensity values
#define INTENSITY_MIN 0.0
#define INTENSITY_MAX 0.8

#define round(x) (int)(x+0.5)
#define random() ((double)rand()) / (double)INT_MAX // Random value on [0,1]

typedef struct terrain {
    int** topography;
    int maxh;       // Maximum value of a height
    int dim;        // Array row/col count
    int max_steps;  // The total number of steps that can be taken
} terrain_t;

/* Calculates the dimension of an array from a step count
 * For example, a 0-step is 2x2 (a square), 1-step 3x3, 2-step 5x5, etc
 * so n-step = 2^n+ 1
 */
int dim_from_steps(int steps) {
   return (1 << steps) + 1;
}

/* Returns a scaled intensity value (assuming that T->maxh is the max height
 * (equal to 1.0 = white) and 0 is the min height (equal to 0.0 = black).
 */
double scaled_col(terrain_t* T, int height) {
    const double intensity_range = INTENSITY_MAX - INTENSITY_MIN;
    return (intensity_range * (double)height / (double)(T->maxh)) + INTENSITY_MIN;
}

/* Draws a gradient square with intensity i0 at (x0,y0), i1 at (x0,y1), i2 at (x1,y1), i3 at (x1,y0)
 * to cr.  Returns true on succes, false on failure
 */
bool draw_square(cairo_t* cr, int x0, int y0, int x1, int y1, double i0, double i1, double i2, double i3) {
    // Create the pattern
    cairo_pattern_t* pat = cairo_pattern_create_mesh();
    cairo_mesh_pattern_begin_patch(pat);
    
    cairo_mesh_pattern_move_to(pat,x0,y0);
    cairo_mesh_pattern_set_corner_color_rgb(pat,0,i0,i0,i0);

    cairo_mesh_pattern_line_to(pat,x0,y1);
    cairo_mesh_pattern_set_corner_color_rgb(pat,1,i1,i1,i1);
    
    cairo_mesh_pattern_line_to(pat,x1,y1);
    cairo_mesh_pattern_set_corner_color_rgb(pat,2,i2,i2,i2);
    
    cairo_mesh_pattern_line_to(pat,x1,y0);
    cairo_mesh_pattern_set_corner_color_rgb(pat,3,i3,i3,i3);
    
    cairo_mesh_pattern_end_patch(pat);

    if (cairo_pattern_status(pat) != CAIRO_STATUS_SUCCESS) return false;

    cairo_rectangle(cr, x0, y0, x1, y1);
    cairo_set_source(cr, pat);
    cairo_fill(cr);
    
    // Clean up
    cairo_pattern_destroy(pat);
    return true;
}

// TODO : errorcheck properly
/* Draws terrain out to file filename, a png of width width and height height
 *
 * Returns true on success, false on failure.
 */
bool draw_terrain(terrain_t* T,const char* filename,int width,int height) {
    // Initialize
    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,width,height);
    cairo_t* cr = cairo_create(surface);

    // Loop over the array, creating squares
    for (int i = 0; i < T->dim-1; i++) {
        int x0 = (width * i) / (T->dim-1);
        int x1 = (width * (i+1)) / (T->dim-1);
        for (int j = 0; j < T->dim-1; j++) {
            int y0 = height * j / (T->dim-1);
            int y1 = height * (j+1) / (T->dim-1);
            double i0 = scaled_col(T,T->topography[i][j]);
            double i1 = scaled_col(T,T->topography[i][j+1]);
            double i2 = scaled_col(T,T->topography[i+1][j+1]);
            double i3 = scaled_col(T,T->topography[i+1][j]);
            if (!draw_square(cr,x0,y0,x1,y1,i0,i1,i2,i3)) return false;
        }
    }

    // Write to png
    cairo_surface_write_to_png(surface,filename);

    // Clean up
    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    return true;
}

void d_s_recurse(terrain_t* T,int steps_remaining,int random_magnitude) {
    if (steps_remaining <= 0) return;
    int step_size = (1 << steps_remaining) >> 1; // Step size is 2^(n-1), or
    // Diamond step                              // half the side len of square
    // Set the center of the diamond equal to the average of the four 
    // corners plus a random offset
    for (int i = step_size; i < T->dim; i += 2*step_size) {
        for (int j = step_size; j < T->dim; j += 2*step_size) {
            int r = round(random() * random_magnitude) - (random_magnitude / 2);
            int sum_elev = T->topography[i - step_size][j - step_size] +
                T->topography[i + step_size][j - step_size] +
                T->topography[i - step_size][j + step_size] +
                T->topography[i + step_size][j + step_size];
            T->topography[i][j] = (sum_elev / 4) + r;
        }
    }

    
    // Square step
    for (int i = 0; i < T->dim; i += step_size) {
        int j = (!((i / step_size) % 2)) * step_size;
        for (; j < T->dim; j += 2*step_size) {
            int r = round(random() * random_magnitude) - (random_magnitude / 2);
            int n_corners = (i != 0) + (j != 0) + (i != T->dim - 1) + (j != T->dim - 1);

            int sum_elev = (i != 0) ? T->topography[i - step_size][j] : 0;
            sum_elev += (j != 0) ? T->topography[i][j - step_size] : 0;
            sum_elev += (i != T->dim - 1) ? T->topography[i + step_size][j] : 0;
            sum_elev += (j != T->dim - 1) ? T->topography[i][j + step_size] : 0;

            T->topography[i][j] = (sum_elev / 4) + r;
        }
    }
    d_s_recurse(T,steps_remaining - 1, random_magnitude / 2);

    return;
}

// Runs the Diamond-Square algorithm to generate terrain
void d_s(terrain_t* T) {
    // Use constant initializations for the corners
    int initial = T->maxh / 2;
    /*T->topography[0][0] = initial;
    T->topography[0][T->dim-1] = initial;
    T->topography[T->dim-1][0] = initial;
    T->topography[T->dim-1][T->dim-1] = initial;*/
    T->topography[0][0] = (int)(random() * T->maxh);
    T->topography[0][T->dim-1] = (int)(random() * T->maxh);
    T->topography[T->dim-1][0] = (int)(random() * T->maxh);
    T->topography[T->dim-1][T->dim-1] = (int)(random() * T->maxh);

    return d_s_recurse(T,T->max_steps,T->maxh / 2);
}

terrain_t* new_terrain(int steps, int maxh) {
    // TODO : rewrite this to use a proper array, rather than this pointer bullshit
    terrain_t* T = malloc(sizeof(struct terrain));

    T->dim = dim_from_steps(steps);
    T->maxh = maxh;
    T->max_steps = steps;

    T->topography = malloc(sizeof(int*) * T->dim);
    for (int i = 0; i < T->dim; i++) T->topography[i] = malloc(sizeof(int) * T->dim);

    d_s(T);

    return T;
}

/* Debugging function to print out terrain
 */
void print_terrain(terrain_t* T) {
    for (int i = 0; i < T->dim; i++) {
        for (int j = 0; j < T->dim; j++) {
            printf("%x  ",T->topography[i][j]);
        }
        printf("\n");
    }
}

void free_terrain(terrain_t* T) {
    for (int i = 0; i < T->dim; i++) free(T->topography[i]);
    free(T->topography);
    free(T);
    return;
}

#endif /* _DIAMONDSQUARE_H_ */
