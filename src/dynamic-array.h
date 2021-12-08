/*
 * ============================================================================
 *
 *       Filename:  dynamic.array.h
 *
 *    Description:  Header file of the dynamic array handler.
 *
 *        Created:  08/12/2021
 *       Compiler:  cc
 *
 *         Author:  Heesung Yang
 *   Organization:  Kyoungpook National University
 *
 * ============================================================================
 */

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

/*
 * Memory allocation
 */
#define MALLOC(p, s) \
if (!(p = malloc(s))) { \
    fprintf(stderr, "malloc: Insufficient memory.\n"); \
    exit(EXIT_FAILURE); \
}

/*
 * Memory reallocation
 * < #RULE1 >
 * If you make a function with this macro, 
 * you must not set the parameter with 'const'.
 */
#define REALLOC(p, s) \
if (!(p = realloc(p, s))) { \
    fprintf(stderr, "realloc: Insufficient memory.\n"); \
    exit(EXIT_FAILURE); \
}

/*
 * Dynaically allocated 1d array handler
 */
void *create_arr(unsigned int size, size_t element_size);
void *arr_doubling(void *arr, unsigned int size, size_t element_size);
void *arr_cut_in_half(void *arr, unsigned int size, size_t element_size);

#endif /* end of #ifndef DYNAMIC_ARRAY_H */