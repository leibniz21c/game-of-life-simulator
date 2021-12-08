/*
 * ============================================================================
 *
 *       Filename:  game.c
 *
 *    Description:  Main logic of gol
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

#include "dynamic-array.h"
#include "game.h"

/*
 * Initialize map
 */
map_t
init_map()
{
    map_t new_map = {
        0, 
        1, 
        (coordinate_t *)create_arr(INIT_ARR_SIZE, sizeof(coordinate_t))};
    return new_map;
}

/*
 * Birth cell function with O(N)
 */
map_t
_cell_birth(map_t map, coordinate_t pos)
{
    int i, target_pos;

    /* 
     * empty map case 
     * Since capacity == 1 and num_of_cells == 0,
     * just put in cell to first element.
     */
    if (map.num_of_cells == 0) {
        /*
         * Just put in one cell
         */
        map.coordinate_arr[0] = pos;
        map.num_of_cells ++;
        return map;
    }

    /* 
     * Already alived cell case 
     * Since map already living cell at the pos,
     * we don't need to move anything.
     */
    if (_search_coordinate(map, pos, 0, map.num_of_cells - 1) != SEARCH_NO_ELEMENT)
        return map;

    /* If we need, array doubling. */
    if (map.num_of_cells >= map.capacity) {
        map.coordinate_arr = arr_doubling(map.coordinate_arr, map.capacity, sizeof(*(map.coordinate_arr)));
        map.capacity *= 2;
    }

    /*
     * general case (no duplicated cell position) 
     * insert position to sparse matrix form map.
     */
    for (target_pos = 0; target_pos < map.num_of_cells; target_pos++) {
        if (pos.row == map.coordinate_arr[target_pos].row) {
            if (pos.col < map.coordinate_arr[target_pos].col) 
                break;
        } else if (pos.row < map.coordinate_arr[target_pos].row) 
            break;
    }
    /* array shifting */
    for (i = map.num_of_cells; i > target_pos; i --)
        map.coordinate_arr[i] = map.coordinate_arr[i - 1];
    /* put in cell to target position */
    map.coordinate_arr[target_pos] = pos;
        
    /* Update metadata of map and return */
    map.num_of_cells ++;
    return map;
}

/*
 * Killing cell function with O(N)
 */
map_t
_cell_death(map_t map, coordinate_t pos)
{
    int i, target_pos;

    /* 
     * Already dead cell case
     * We don't need to move anything.
     */
    if ((target_pos = _search_coordinate(map, pos, 0, map.num_of_cells - 1)) == SEARCH_NO_ELEMENT)
        return map;

    /*
     * general case (just killing living cell)
     */
    /* array shifting */
    for (i = target_pos; i < map.num_of_cells - 1 ; i ++)
        map.coordinate_arr[i] = map.coordinate_arr[i + 1];
        
    /* Update metadata of map and return */
    map.num_of_cells --;

    /* 
     * If we need, we have to reduce array size.
     */
    if (map.num_of_cells*2 <= map.capacity && map.capacity != 1) {
        map.coordinate_arr = arr_cut_in_half(map.coordinate_arr, map.capacity, sizeof(*(map.coordinate_arr)));
        map.capacity /= 2;
    }

    return map;
}

/*
 * Implemented with recursive binary search algorithm < O(logN) >.
 * This binary search algorithm aims to sparse matrix search.
 */
int
_search_coordinate(map_t map, coordinate_t pos, int left, int right) 
{
    int mid = (left + right)/2;

    /* Binary search terminal condition */
    if (left > right) return SEARCH_NO_ELEMENT;

    /* comparing by row */
    if (map.coordinate_arr[mid].row < pos.row) 
        return _search_coordinate(map, pos, mid + 1, right);
    else if (map.coordinate_arr[mid].row == pos.row) {
        /* comparing by col */
        if (map.coordinate_arr[mid].col < pos.col) 
            return _search_coordinate(map, pos, left, mid - 1);
        else if (map.coordinate_arr[mid].col == pos.col)
            return mid; /* Found out case */
        else /* map.coordinate_arr[mid].col > pos.col */
            return _search_coordinate(map, pos, mid + 1, right);
    }
    else /* map.coordinate_arr[mid].row > pos.row */
        return _search_coordinate(map, pos, left, mid - 1);
}

/*
 * Temporal implemented for DEBUG
 */
void
print_map(map_t map)
{
    int i;

    puts("map info :");
    fprintf(
        stdout, 
        "map.num_of_cells == %lld\n", 
        map.num_of_cells);
    fprintf(
        stdout, 
        "map.capacity == %lld\n", 
        map.capacity);
    fprintf(
        stdout, 
        "map.coorinate_arr == 0x%lld\n", 
        map.coordinate_arr);

    for (i = 0 ;i < map.num_of_cells; i ++) {
        printf("(%d, %d)\t", 
            map.coordinate_arr[i].row, 
            map.coordinate_arr[i].col);
        if (!((i + 1)%4)) putchar('\n');
    }
}