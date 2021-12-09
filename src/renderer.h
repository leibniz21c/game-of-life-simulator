/*
 * ============================================================================
 *
 *       Filename:  renderer.h
 *
 *    Description:  Header file of renderer
 *
 *        Created:  09/12/2021
 *       Compiler:  cc
 *
 *         Author:  Heesung Yang
 *   Organization:  Kyoungpook National University
 *
 * ============================================================================
 */
#ifndef RENDERER_H
#define RENDERER_H

/*
 * Menu interface constant 
 */
/* F1 Start  F2 Stop  F3 FastStart  F4 Modify  F5 Save */
#define __UPPER_INTERFACE_LEN 53
#define __COL_F1_START(NUM_COLS) (NUM_COLS - __UPPER_INTERFACE_LEN)
#define __COL_F2_STOP(NUM_COLS) (NUM_COLS - __UPPER_INTERFACE_LEN + 10)
#define __COL_F3_FAST_START(NUM_COLS) (NUM_COLS - __UPPER_INTERFACE_LEN + 19)
#define __COL_F4_MODIFY(NUM_COLS) (NUM_COLS - __UPPER_INTERFACE_LEN + 33)
#define __COL_F5_SAVE(NUM_COLS) (NUM_COLS - __UPPER_INTERFACE_LEN + 44)

void new_cell_map(const char *file_name);
void render_interface(const char *file_name);
void _init_game_window_setting();
void _render_grid();

#endif // end of #ifndef RENDERER_H
