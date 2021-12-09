/*
 * ============================================================================
 *
 *       Filename:  renderer.c
 *
 *    Description:  Rendering tools implementation
 *
 *        Created:  09/12/2021
 *       Compiler:  cc
 *
 *         Author:  Heesung Yang
 *   Organization:  Kyoungpook National University
 *
 * ============================================================================
 */

#include <string.h>
#include <ncurses.h>
#include <time.h>

#include "game.h"
#include "renderer.h"

/* Global var */
map_t curr_map, next_map;

/**
 * @brief 
 * Create new cell map
 */
void
new_cell_map(const char *file_name)
{
    _init_game_window_setting();
    _render_grid();
    render_interface(file_name);
    refresh();

    
    getch();
    getch();
    getch();
    endwin();
}

/**
 * @brief 
 * Rendering
 */
void
render_interface(const char *file_name)
{
    time_t time_now = time(NULL);

    /*
     * File name (Upper left)
     */
    if (!strcmp(file_name, "-")) /* No file name */
        mvprintw(1, 1, "FILE : %d.grf", time_now); /* Draw file name to upper grid */
    else if (strlen(file_name) + 7 + __UPPER_INTERFACE_LEN < COLS) /* 7 is strlen("FILE :") */
        mvprintw(1, 1, "FILE : %s", file_name); /* Draw file name to upper grid */
    else /* strlen(file_name) + 7 + __UPPER_INTERFACE_LEN >= COLS */
        mvprintw(1, 1, "FILE : ...%s", 
            file_name + strlen(file_name) - COLS + 7 + 4 + __UPPER_INTERFACE_LEN + 1); /* Draw file name to upper grid, 4 is strlen(" ...")  */

    /*
     * Menu interfaces
     */
    standout();
    mvprintw(1, __COL_F1_START(COLS), "F1");
    mvprintw(1, __COL_F2_STOP(COLS), "F2");
    mvprintw(1, __COL_F3_FAST_START(COLS), "F3");
    mvprintw(1, __COL_F4_MODIFY(COLS), "F4");
    mvprintw(1, __COL_F5_SAVE(COLS), "F5");
    standend();

    mvprintw(1, __COL_F1_START(COLS) + 3, "Start");
    mvprintw(1, __COL_F2_STOP(COLS) + 3, "Stop");
    mvprintw(1, __COL_F3_FAST_START(COLS) + 3, "FastStart");
    mvprintw(1, __COL_F4_MODIFY(COLS) + 3, "Modify");
    mvprintw(1, __COL_F5_SAVE(COLS) + 3, "Save");

}

/**
 * @brief 
 * Initialize game window setting
 */
void 
_init_game_window_setting()
{
    int i;

    /* Init window setting */
    initscr(); /* Start curses mode 		  */

    /* Init key setting */
    cbreak(); /* Line buffering disabled */
    noecho(); /* Turn off the echo */
    keypad(stdscr, TRUE); /* Interactive mode key available */
    
    /* Init cursor setting */
    curs_set(0); /* cursor size zero */
}

/**
 * @brief 
 * Draw base interface
 */
void
_render_grid()
{
    int i;
    
    /*
     * Drawing basic grid 
     */
    /* Create border */
    border(ACS_VLINE,
        ACS_VLINE,
        ACS_HLINE,
        ACS_HLINE,
        ACS_ULCORNER,
        ACS_URCORNER,
        ACS_LLCORNER,
        ACS_LRCORNER); 

    /* Create upper horizontal line */
    mvaddch(2, 0, ACS_LTEE);
    for (i = 0 ;i < COLS - 2; i ++) addch(ACS_HLINE);
    addch(ACS_RTEE);
}