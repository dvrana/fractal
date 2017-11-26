/* Written by Dylan Vrana (dylanvrana@gmail.com)
 *
 * A portable library for defining, iterating, and drawing L-systems
 * 
 *
 */

#ifndef _LSYSTEM_H_
#define _LSYSTEM_H_

typedef struct lsystem lsys_t;
typedef struct lsystem_string lsys_str_t;

// Defines a new deterministic L-system of n variables n should be less than 
// or equal to 256 (as variables are internally represented as chars).
lsys_t* lsys_new(int n);

// Adds a rule to L mapping variable from to string to which has length n
// Note that constants (ie. i -> i) need not be specified
void lsys_add(lsys_t* L, char from, char* to, int n);

// Iterates L-system L for n iterations
lsys_str_t* lsys_it(lsys_t* L, int n);

// Debugging code.  Prints out a L-system string with arbitrary (but 
// consistent) character choices for variables.  May reuse variables 
// for large L-systems (> 60 different variables)
void lsys_print(lsys_str_t* S);

// Frees a L-system specification
void lsys_free(lsys_t* L);

// Frees a L-system string
void lsys_str_free(lsys_str_t* S);

#endif /* _LSYSTEM_H_ */
