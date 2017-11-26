#ifndef _LSYSTEM_H_
#define _LSYSTEM_H_

#include <string.h>
#include <stdlib.h>

#define VAR_MAX 256 // Maximum number of variables

typedef struct rule {
    int n; // Length of rule replacement string
    char* rep; // String to replace rule character with
               // If rep is NULL, the character is constant
} rule_t;

typedef struct lsystem {
    unsigned char n; // Number of variables
    rule_t* rules; // Array of length n specifying rules as i -> rules[i]
} lsys_t;
typedef struct lsystem_string lsys_str_t;

// Defines a new deterministic L-system of n variables. n should be less than 
// or equal to 256 (as variables are internally represented as chars).
// Returns NULL on failure, otherwise a pointer to the new L-system
lsys_t* lsys_new(unsigned char n) {
    lsys_t* L = malloc(sizeof(lsys_t));
    if (L == NULL) return NULL;

    L->n = n;
    L->rules = malloc(sizeof(rule_t*) * n);
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
    return; // TODO : this
}

// Iterates L-system L for n iterations
lsys_str_t* lsys_it(lsys_t* L, int n) {
    return NULL; // TODO : this
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

// Frees a L-system string
void lsys_str_free(lsys_str_t* S) {
    return; // TODO : this
}

#endif /* _LSYSTEM_H_ */
