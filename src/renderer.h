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

/* Game status flags */
typedef char game_status_t;
#define STATUS_NO_STATE         ((char)0x00) /* 0000 0000 */
#define STATUS_F1_START         ((char)0x01) /* 0000 0001 */
#define STATUS_F2_STOP          ((char)0x02) /* 0000 0010 */
#define STATUS_F3_FAST_START    ((char)0x04) /* 0000 0100 */
#define STATUS_F4_MODIFY        ((char)0x08) /* 0000 1000 */
#define STATUS_F5_SAVE          ((char)0x10) /* 0001 0000 */

#define HAS_STATE(s)                (s | (char)0x1f)
#define IS_STATE_F1_START(s)        (s & (char)0x01)
#define IS_STATE_F2_STOP(s)         (s & (char)0x02)
#define IS_STATE_F3_FAST_START(s)   (s & (char)0x04)
#define IS_STATE_F4_MODIFY(s)       (s & (char)0x08)
#define IS_STATE_F5_SAVE(s)         (s & (char)0x10)

/* Key type */
typedef int key_t;

void new_cell_map(const char *file_name);
void keyboard_io_handler(int signo);
void _render_standout_interface_element();
void render_upper_interface(const char *file_name);
void render_map();
void _init_game_window_setting();
void _render_grid();
void _rendering_cell_map();

void print_message(const char *msg);
void clear_message();

key_t game_mode_modify();
coordinate_t transform_curse2game_coordinate(coordinate_t origin);
coordinate_t transform_game2curse_coordinate(coordinate_t origin);
bool is_valid_coordinate(coordinate_t origin);

#endif // end of #ifndef RENDERER_H
