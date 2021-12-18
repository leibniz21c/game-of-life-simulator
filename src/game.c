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
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "dynamic-array.h"
#include "game.h"

/**
 * @brief  game constants
 */
static const coordinate_t direction[8] = {
    {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}
}; /* unit direction coordinates */

/**
 * @brief  Free map
 */
void freemap(map_t map)
{
    /* If capacity is zero, it is empty map. */
    map.num_of_cells = map.capacity = 0;
    free(map.coordinate_arr);
}

/**
 * @brief 
 * Copy map
 * return copied_map(map_t)
 * It contains new reference to heap memory which has same values of origin map.
 */
map_t
_copy_map(map_t origin)
{
    int i;
    map_t copied_map = init_map();

    /* copy metadatas */
    copied_map.num_of_cells = origin.num_of_cells;
    copied_map.capacity = origin.capacity;

    /* new allocation */
    copied_map.coordinate_arr = create_arr(origin.capacity, sizeof(*(copied_map.coordinate_arr)));

    /* copy elements one by one */
    for (i = 0 ;i < origin.num_of_cells; i ++)
        copied_map.coordinate_arr[i] = origin.coordinate_arr[i];

    return copied_map;
}

/**
 * @brief 
 * Get cell map to check to move to the next step
 */
map_t 
_get_cell_map_need_to_check(map_t map)
{
    int i, j;
    map_t cellmap = init_map();

    for (i = 0; i < map.num_of_cells; i ++) {
        cellmap = _cell_birth(cellmap, map.coordinate_arr[i]);
        for (j = (int)north_east; j < 8; j ++)
            cellmap = _cell_birth(cellmap, get_onestep_coordinate(map.coordinate_arr[i], j));
    }

    return cellmap;
}

/**
 * @brief 
 * get next map
 */
map_t 
_get_next_map(map_t map)
{
    int i, searched_index, num_of_adjacent_living_cells;
    coordinate_t check_pos;
    map_t need_to_check_cellmap, new_map;

    /* Initialize */
    need_to_check_cellmap = _get_cell_map_need_to_check(map);
    new_map = _copy_map(map);

    /* one by one check */
    for (i = 0; i < need_to_check_cellmap.num_of_cells;i ++) {
        check_pos = need_to_check_cellmap.coordinate_arr[i];
        searched_index = _search_coordinate(map, check_pos, 0, map.num_of_cells - 1);
        num_of_adjacent_living_cells = _get_num_of_adjacent_living_cells(map, check_pos);

        if (SEARCH_NO_ELEMENT == searched_index
            && _get_num_of_adjacent_living_cells(map, check_pos) == RULE1_CONSTANT)
                /* Dead cell with 3 adjacent alived cells case */
                new_map = _cell_birth(new_map, check_pos);
        else if (SEARCH_NO_ELEMENT != searched_index
                  && !(num_of_adjacent_living_cells == RULE2_CONSTANT1 
                      || num_of_adjacent_living_cells == RULE2_CONSTANT2))
                /* Living cell with not 2 or 3 adjacent living cells case */
                new_map = _cell_death(new_map, check_pos);
    }

    /* clean */
    freemap(map);
    freemap(need_to_check_cellmap);

    return new_map;
}

/**
 * @brief 
 * Get number of adjacent living cells
 */
count_t 
_get_num_of_adjacent_living_cells(map_t map, coordinate_t pos)
{
    int i;
    count_t num_of_adjacent_living_cells = 0;

    /* counting from north east direction to north with clockwise */
    for (i = (int)north_east; i < 8; i ++)
        if (_search_coordinate(map, get_onestep_coordinate(pos, i), 0, map.num_of_cells - 1) != SEARCH_NO_ELEMENT) 
            num_of_adjacent_living_cells ++;

    return num_of_adjacent_living_cells;
}

/**
 * @brief 
 * Get coordinate moved one step from origin coordinate
 */
coordinate_t 
get_onestep_coordinate(coordinate_t pos, direction_t dir)
{
    coordinate_t moved;

    moved.row = pos.row + direction[dir].row;
    moved.col = pos.col + direction[dir].col;

    return moved;
}

/**
 * @brief 
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

/**
 * @brief 
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

/**
 * @brief 
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

/**
 * @brief 
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
            return _search_coordinate(map, pos, mid + 1, right);
        else if (map.coordinate_arr[mid].col == pos.col)
            return mid; /* Found out case */
        else /* map.coordinate_arr[mid].col > pos.col */
            return _search_coordinate(map, pos, left, mid - 1);
    } else {
        /* map.coordinate_arr[mid].row > pos.row */
        return _search_coordinate(map, pos, left, mid - 1);
    }
}

/**
 * @brief 
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
        "map.coorinate_arr == 0x%p\n", 
        map.coordinate_arr);

    for (i = 0 ;i < map.num_of_cells; i ++) {
        printf("(%d, %d)\t", 
            map.coordinate_arr[i].row, 
            map.coordinate_arr[i].col);
        if (!((i + 1)%4)) putchar('\n');
    }
}

/**
 * @brief 
 * Save map to file (grf)
 */
void 
save_instance(const char *file_name, map_t map)
{
    int fd, i, pid;

    if ((pid = fork()) == -1) {
        fprintf(stderr, "folk: error!\n");
        exit(EXIT_FAILURE);
    }
    
    if (pid == 0) {    
        /* Storing by child process. */
        if ((fd = open(file_name, O_WRONLY | O_CREAT, 0644)) == -1) {
            fprintf(stderr, "open: save_instance error\n");
            exit(EXIT_FAILURE);
        }
        write_map(fd, map);     
        exit(EXIT_SUCCESS);  
    }
}

/**
 * @brief 
 * Write map to fd 
 */
void 
write_map(int fd, map_t map)
{
    int i;

    /* Write metadatas */
    if (write(fd, (void *)&map.num_of_cells, sizeof(map.num_of_cells)) == -1) {
        fprintf(stderr, "write: map.num_of_cells error\n");
        exit(EXIT_FAILURE);
    }
    if (write(fd, (void *)&map.capacity, sizeof(map.capacity)) == -1) {
        fprintf(stderr, "write: map.capacity error\n");
        exit(EXIT_FAILURE);
    }

    /* Write datas */
    for (i = 0 ;i < map.num_of_cells; i ++) {
        if (write(fd, (void *)&map.coordinate_arr[i], sizeof(coordinate_t)) == -1) {
            fprintf(stderr, "write: map.coordinate %d element error\n", i);
            exit(EXIT_FAILURE);
        }
    }
}

/**
 * @brief 
 * Load map from file (grf)
 */
map_t 
load_file(const char *file_name)
{
    int fd, i;
    
    if ((fd = open(file_name, O_RDONLY)) == -1) {
        fprintf(stderr, "open: load file error. Is there a %s?\n", file_name);
        exit(EXIT_FAILURE);
    }
    return read_map(fd);
}

/**
 * @brief 
 * Read map from fd 
 */
map_t 
read_map(int fd)
{
    map_t map = init_map();
    coordinate_t temp_coord;
    count_t temp;
    int i;

    map.capacity = LOAD_MAP_INIT_CAPACITY;
    map.coordinate_arr = create_arr(map.capacity, sizeof(*map.coordinate_arr));

    /* Read metadatas */
    if (read(fd, (void *)&map.num_of_cells, sizeof(map.num_of_cells)) == -1) {
        fprintf(stderr, "read: map.num_of_cells error\n");
        exit(EXIT_FAILURE);
    }

    if (read(fd, (void *)&temp, sizeof(temp)) == -1) {
        fprintf(stderr, "read: map.capacity error\n");
        exit(EXIT_FAILURE);
    }

    /* Write datas */
    for (i = 0 ;i < map.num_of_cells; i ++) {
        if (read(fd, (void *)&temp_coord, sizeof(temp_coord)) == -1) {
            fprintf(stderr, "write: map.coordinate %d element error\n", i);
            exit(EXIT_FAILURE);
        }
        map = _cell_birth(map, temp_coord);
    }

    return map;
}