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
/* macros */
#define INIT_ARR_SIZE 1
#define SEARCH_NO_ELEMENT -1

/* direction */
typedef enum {north_east, east, south_east, south, south_west, west, north_west} direction_t;

/*
 * Types
 */
typedef unsigned long long count_t; /* counting variable types */
typedef struct { int row, col; } coordinate_t; /* coordinate type */
typedef struct { count_t num_of_cells, capacity; coordinate_t *coordinate_arr; } map_t; /* map type */

/*
 * Logics
 */
void freemap(map_t map);
map_t _get_cell_map_need_to_check(map_t map);
count_t _get_num_of_adjacent_living_cells(map_t map, coordinate_t pos);
coordinate_t get_onestep_coordinate(coordinate_t pos, direction_t dir);

map_t init_map(); /* initialize cell map */
map_t _cell_birth(map_t map, coordinate_t pos); /* birth cell */
map_t _cell_death(map_t map, coordinate_t pos); /* kill cell */ 
int _search_pos(map_t map, coordinate_t pos, int left, int right); /* search cell position */

/*
 * Debug
 */
void print_map(map_t map); /* DEBUG : for debug */

#endif /* end of #ifndef GAME_H */