#ifndef _LSYSTEM_H_
#define _LSYSTEM_H_

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lsys.h"

#define VAR_MAX 256 // Maximum number of variables

typedef struct rule {
    int n; // Length of rule replacement string
    unsigned char* rep; // String to replace rule character with
               // If rep is NULL, the character is constant
} rule_t;

typedef struct lsystem {
    unsigned char n; // Number of variables
    rule_t* rules; // Array of length n specifying rules as i -> rules[i]
} lsys_t;

typedef struct lsystem_string {
    unsigned char* string; // payload
    int n; // length of string
    int n_max; // length of allocated buffer
} lsys_str_t;

// Defines a new deterministic L-system of n variables. n should be less than 
// or equal to 256 (as variables are internally represented as chars).
// Returns NULL on failure, otherwise a pointer to the new L-system
lsys_t* lsys_new(unsigned char n) {
    lsys_t* L = malloc(sizeof(lsys_t));
    if (L == NULL) return NULL;

    L->n = n;
    L->rules = malloc(sizeof(rule_t) * n);
    if (L->rules == NULL) {
        free(L);
        return NULL;
    }
    for (int i = 0; i < n; i++) {
        L->rules[i].rep = NULL;
        L->rules[i].n = 1;
    }

    return L;
}

// Adds a rule to L mapping variable from to string to which has length n
// Note that constants (ie. i -> i) need not be specified
void lsys_add(lsys_t* L, char from, char* to, int n) {
    L->rules[from].rep = to;
    L->rules[from].n = n;
    return;
}

// Frees a L-system string
void lsys_str_free(lsys_str_t* S) {
    free(S->string);
    free(S);
    return;
}

// Doubles the length of the string in S.  
// 
// Returns true on success, false on failure
bool extend_string(lsys_str_t* S) {
    int new_max = S->n_max * 2;
    unsigned char* str_new = realloc(S,new_max);
    if (str_new == NULL) return false;

    S->string = str_new;
    S->n_max = new_max;
    return true;
}

// Iterates L-system L on character c for n_it iterations, appending
// the results to S->string, updating S as necessary.  Re-sizes the
// array holding S if necessary
// Returns true on success, or false on failure (generally because of
// failure to resize the array; L-system memory requirements grow
// exponentially with iteration count in the general case).  On failure,
// sets S->string to be the pre-failure string before returning.
bool iterate_character(lsys_t* L, lsys_str_t* S, unsigned char c, int n_it) {
    if (n_it <= 0) return true;

    rule_t* R = &(L->rules[c]);

    // If c is constant, write C to S and return true
    if (R->rep == NULL) {
        if (S->n >= S->n_max) {
            if (!extend_string(S)) return false;
        }
        S->string[S->n] = c;
        S->n++;
        return true;
    }

    // If we're at the lowest level, directly write the replacement
    if (n_it == 1) {
        if (S->n + R->n >= S->n_max) {
            if (!extend_string(S)) return false;
        }

        memcpy(S->string + S->n,R->rep,R->n);
        S->n = S->n + R->n;
        return true;
    }

    // Otherwise recurse on each character in the replacement
    for (int i = 0; i < R->n; i++) {
        if (!iterate_character(L, S, R->rep[i], n_it - 1)) return false;
    }

    return true;
}

// Iterates L-system L for n iterations with initial state initial of length
// n_init
//
// Iterates by-character (to save memory writes) and when the allocated 
// memory runs out of space, reallocates double space
// Returns NULL on failure (or, if inputs are bad, crashes uninformatively)
lsys_str_t* lsys_it(lsys_t* L, char* initial, int n_init, int n_it) {
    lsys_str_t* S = malloc(sizeof(lsys_str_t));
    if (S == NULL) return NULL;

    S->n = 0;
    S->n_max = n_init * n_it;

    for (int i = 0; i < n_init; i++) {
        if (!iterate_character(L,S,initial[i],n_it)) {
            lsys_str_free(S);
            return NULL;
        }
    }

    return S;
}

// Debugging code.  Prints out a L-system string with arbitrary (but 
// consistent) character choices for variables.  May reuse variables 
// for large L-systems (> 60 different variables)
void lsys_print(lsys_str_t* S) {
    return; // TODO : this
}

// Frees a L-system specification
// Assumes L points to a well-constructed lsys_t
void lsys_free(lsys_t* L) {
    free(L->rules);
    free(L);
    return;
}

#endif /* _LSYSTEM_H_ */
