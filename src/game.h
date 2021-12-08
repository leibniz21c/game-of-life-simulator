/*
 * ============================================================================
 *
 *       Filename:  game.h
 *
 *    Description:  Header file of game.c
 *
 *        Created:  08/12/2021
 *       Compiler:  cc
 *
 *         Author:  Heesung Yang
 *   Organization:  Kyoungpook National University
 *
 * ============================================================================
 */
#ifndef GAME_H
#define GAME_H

/*
 * Game constants 
 */
#define INIT_ARR_SIZE 1
#define SEARCH_NO_ELEMENT -1

/*
 * Types
 */
typedef unsigned long long count_t; /* counting variable types */
typedef struct { int row, col; } coordinate_t; /* coordinate type */
typedef struct { count_t num_of_cells; count_t capacity; coordinate_t *coordinate_arr; } map_t; /* map type */

/*
 * Logics
 */
map_t init_map(); /* initialize cell map */
map_t _cell_birth(map_t map, coordinate_t pos); /* birth cell */
map_t _cell_death(map_t map, coordinate_t pos); /* kill cell */ 
int _search_pos(map_t map, coordinate_t pos, int left, int right); /* search cell position */

/*
 * Debug
 */
void print_map(map_t map); /* DEBUG : for debug */

#endif /* end of #ifndef GAME_H */