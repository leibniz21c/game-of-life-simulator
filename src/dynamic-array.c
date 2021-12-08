/*
 * ============================================================================
 *
 *       Filename:  dynamic-array.c
 *
 *    Description:  Dynamic array handler using stdlib
 *
 *        Created:  08/12/2021
 *       Compiler:  cc
 *
 *         Author:  Heesung Yang
 *   Organization:  Kyoungpook National University
 *
 * ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "dynamic-array.h"

/*
 * Create dynaically allocated 1d array
 */
void 
*create_arr(unsigned int size, size_t element_size)
{
    void *arr;

    MALLOC(arr, size*element_size);
    return arr;
}

/*
 * Array doubling with reallocation.
 * It follows #RULE1
 */
void 
*arr_doubling(void *arr, unsigned int size, size_t element_size)
{
    REALLOC(arr, 2*size*element_size);
    return arr;
}

/*
 * Array trimming to half with reallocation.
 * It follows #RULE1
 */
void
*arr_cut_in_half(void *arr, unsigned int size, size_t element_size)
{
    if (size <= 1) {
        fprintf(stderr, "_arr_cut_in_half: Array size is less than 2.\n");
        exit(EXIT_FAILURE);
    }
    REALLOC(arr, size*element_size/2);
    return arr;
}