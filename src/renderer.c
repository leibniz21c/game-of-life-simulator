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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <termios.h>

#include "args.h"
#include "game.h"
#include "renderer.h"

/* Global var */
char glb_file_name[FILE_NAME_SIZE];
map_t curr_map;
game_status_t game_status = STATUS_NO_STATE;
struct itimerval default_timeset, start_timeset, fast_start_timeset;

/**
 * @brief 
 * Get user keyboard input like F1, F2, F3, ..., Enter, rightarrow, q, ESC and execute.
 */
void 
keyboard_io_handler(int signo)
{
    key_t input;
	sigset_t sigset, oldset;
	sigfillset(&sigset);	
    
    /* Get keyboard input */
    if ((input = wgetch(stdscr)) == ERR) {
		fprintf(stderr, "SIGIO asynchronous read() error\n");
        exit(EXIT_FAILURE);
    }

    /* key processing */
    switch (input) {
        case KEY_F(1):
            game_status = STATUS_F1_START; /* set status flag */
            /* interface standout mode */
            _render_standout_interface_element();   
            
            /* print message */
            clear_message();
            print_message("F1 Pressed");

            /* Game start courutine */
            signal(SIGALRM, status_start_alarm_handler);
            setitimer(ITIMER_REAL, &start_timeset, NULL);

            break;
        case KEY_F(2):
            game_status = STATUS_F2_STOP;
            /* interface standout mode */
            _render_standout_interface_element();   
            
            /* print message */
            clear_message();
            print_message("F2 Pressed");

            /* Game stop */
            signal(SIGALRM, SIG_IGN);
            setitimer(ITIMER_REAL, &default_timeset, NULL);
            
            break;
        case KEY_F(3):
            game_status = STATUS_F3_FAST_START;
            /* interface standout mode */
            _render_standout_interface_element();   
            
            /* print message */
            clear_message();
            print_message("F3 Pressed");

            /* Game start courutine */
            signal(SIGALRM, status_start_alarm_handler);
            setitimer(ITIMER_REAL, &fast_start_timeset, NULL);

            break;
        case KEY_F(4):
            game_status = STATUS_F4_MODIFY;
            /* interface standout mode */
            _render_standout_interface_element();   
            
            /* print message */
            clear_message();
            print_message("F4 Pressed");

            /* Game stop */
            signal(SIGALRM, SIG_IGN);
            setitimer(ITIMER_REAL, &default_timeset, NULL);

            game_mode_modify();
            break;
        case KEY_F(5):
            game_status = STATUS_F5_SAVE;
            /* interface standout mode */
            _render_standout_interface_element();  

            /* print message */
            clear_message();
            print_message("F5 Pressed"); 

            /* Game stop */
            signal(SIGALRM, SIG_IGN);
            setitimer(ITIMER_REAL, &default_timeset, NULL);

            /* print message     */
            clear_message();
            print_message("Saving...");

            /* Save instance */
            save_instance(glb_file_name, curr_map);

            /* print message */
            clear_message();
            print_message("Save success!");
            break;
        case 'q':
            // tty_mode(1); /* rollback terminal setting */
            endwin(); /* termination curses mode */
            exit(EXIT_SUCCESS);
        default:
            break;
    }

}

/**
 * @brief 
 * Start game routine with normal speed
 */
void 
status_start_alarm_handler(int signo)
{
    _erasing_cell_map(curr_map);
    curr_map = _get_next_map(curr_map);
    _rendering_cell_map(curr_map);
}

/**
 * @brief 
 * interface element standout drawing
 */
void
_render_standout_interface_element()
{
    /* No status */
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

    standout();
    /* each status */
    switch (game_status) {
        case STATUS_F1_START: 
            mvprintw(1, __COL_F1_START(COLS), "F1 Start");
            break;
        case STATUS_F2_STOP: 
            mvprintw(1, __COL_F2_STOP(COLS), "F2 Stop");
            break;
        case STATUS_F3_FAST_START: 
            mvprintw(1, __COL_F3_FAST_START(COLS), "F3 FastStart");
            break;
        case STATUS_F4_MODIFY: 
            mvprintw(1, __COL_F4_MODIFY(COLS), "F4 Modify");
            break;
        case STATUS_F5_SAVE:
            mvprintw(1, __COL_F5_SAVE(COLS), "F5 Save");
            break;
        default:
            break;
    }
    standend();
    refresh();
}

/**
 * @brief 
 * Rendering
 */
void
render_upper_interface(const char *file_name)
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

    /* standout elements */
    _render_standout_interface_element(); /* contains refresh() */
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
    keypad(stdscr, TRUE); /* Special key inputs available */
    
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

    /* Create upper horizontal line */
    mvaddch(LINES - 3, 0, ACS_LTEE);
    for (i = 0 ;i < COLS - 2; i ++) addch(ACS_HLINE);
    addch(ACS_RTEE);

    refresh();
}

/**
 * @brief 
 * rendering cell map : coroutine
 */
void
_rendering_cell_map(map_t map)
{
    int i;
    char *mark = "  ";
    coordinate_t coord;

    standout();
    /* one by one cell marking */
    for (i = 0 ;i < map.num_of_cells;i ++) {
        coord = transform_game2curse_coordinate(map.coordinate_arr[i]);
        
        /* If this cooordinate is not valid, remove this cell. */
        if (!is_valid_coordinate(coord)) {
            map = _cell_death(map, coord);
            i --;
        } 
        else 
            mvaddstr(coord.row, coord.col, mark);
    }
    standend();
    refresh();
}

/**
 * @brief 
 * erasing cell map : coroutine
 */
void
_erasing_cell_map(map_t map)
{
    int i;
    char *mark = "  ";
    coordinate_t coord;

    /* one by one cell marking */
    for (i = 0 ;i < map.num_of_cells;i ++) {
        coord = transform_game2curse_coordinate(map.coordinate_arr[i]);
        mvaddstr(coord.row, coord.col, mark);
    }
    refresh();
}


/**
 * @brief 
 * print message
 */
void 
print_message(const char *msg)
{
    mvwaddstr(stdscr, LINES - 2, 1, msg);
    refresh();
}

/**
 * @brief 
 * clear message
 */
void
clear_message()
{
    int i;

    move(LINES - 2, 1);
    for (i = 1; i < COLS - 2; i ++)
        addch(' ');
    refresh();
}

/**
 * @brief 
 * Create game with new cell map
 */
void new_game(const char *file_name)
{
    time_t time_now = time(NULL);

    /* Create empty map */
    curr_map = init_map();

    /* No file name */
    if (!strcmp(file_name, "-"))
        sprintf(glb_file_name, "%ld.grf", time_now);
    else 
        strcpy(glb_file_name, file_name);

    _create_game();
}

/**
 * @brief 
 * Create game with loaded cell map
 */
void load_game(const char *file_name)
{
    /* Globalize file name */
    strcpy(glb_file_name, file_name);

    /* Load map */
    curr_map = load_file(glb_file_name);

    _create_game();
}

/**
 * @brief 
 * Create new cell map
 */
void
_create_game()
{
    struct sigaction iosig;
    int fcntl_flag;

    /*
     * curses setting
     */
    _init_game_window_setting(); /* Initialize setting */
    _render_grid(); /* Draw grid */
    render_upper_interface(glb_file_name); /* Draw interface */
    refresh();

    /*
     * start to get SIGIO asynchronous keyboard input 
     */
    /* Set sigaction and handler */
    iosig.sa_handler = keyboard_io_handler;
    sigemptyset(&iosig.sa_mask);
    iosig.sa_flags = 0;

    if (sigaction(SIGIO, &iosig, 0) == -1) {
        fprintf(stderr, "sigaction: error\n");
        exit(EXIT_FAILURE);
    }

    /* Set stdin to async mode */
    fcntl_flag = fcntl(0, F_GETFL, 0);
    fcntl_flag |= O_ASYNC;
    fcntl(0, F_SETFL, fcntl_flag);

    /* If SIGIO is generated with stdin, only this process can get it. */
    fcntl(0, F_SETOWN, getpid());

    /*
     * itimer initialize
     */
    default_timeset.it_value.tv_sec = __ITIMER_DEFAULT_VALUE_SEC;
    default_timeset.it_value.tv_usec = __ITIMER_DEFAULT_VALUE_USEC;
    default_timeset.it_interval.tv_sec = __ITIMER_DEFAULT_INTERVAL_SEC;
    default_timeset.it_interval.tv_usec = __ITIMER_DEFAULT_INTERVAL_USEC;
    start_timeset.it_value.tv_sec = __ITIMER_START_VALUE_SEC;
    start_timeset.it_value.tv_usec = __ITIMER_START_VALUE_USEC;
    start_timeset.it_interval.tv_sec = __ITIMER_START_INTERVAL_SEC;
    start_timeset.it_interval.tv_usec = __ITIMER_START_INTERVAL_USEC;
    fast_start_timeset.it_value.tv_sec = __ITIMER_FAST_START_VALUE_SEC;
    fast_start_timeset.it_value.tv_usec = __ITIMER_FAST_START_VALUE_USEC;
    fast_start_timeset.it_interval.tv_sec = __ITIMER_FAST_START_INTERVAL_SEC;
    fast_start_timeset.it_interval.tv_usec = __ITIMER_FAST_START_INTERVAL_USEC;


    /* DEBUG */
    _rendering_cell_map(curr_map);
    while (1) sleep(1);
    
    /* Termination */
    endwin();
}

/**
 * @brief 
 * modify mode rendering
 */
void
game_mode_modify()
{
    coordinate_t pos = {LINES/2, COLS/2};
    key_t input;

    key_t temp = KEY_F(1);

    /* Move to initial position */
    curs_set(1);
    move(pos.row, pos.col);

    while (true) {
        /* get input */
        input = wgetch(stdscr);

        switch (input) {
            case KEY_UP:
                pos.row -= 1;
                if (is_valid_coordinate(pos)) move(pos.row, pos.col);
                else pos.row += 1;
                break;
            case KEY_DOWN:
                pos.row += 1;
                if (is_valid_coordinate(pos)) move(pos.row, pos.col);
                else pos.row -= 1;
                break;
            case KEY_RIGHT:
                pos.col += 2;
                if (is_valid_coordinate(pos)) move(pos.row, pos.col);
                else pos.col -= 2;
                break;
            case KEY_LEFT:
                pos.col -= 2;
                if (is_valid_coordinate(pos)) move(pos.row, pos.col);
                else pos.col += 2;
                break;
            case '\n': /* KEY_RETURN */
                curr_map = _cell_birth(curr_map, 
                            transform_curse2game_coordinate(pos));
                _rendering_cell_map(curr_map);
                move(pos.row, pos.col);
                break;
            case KEY_BACKSPACE:
                _erasing_cell_map(curr_map);
                curr_map = _cell_death(curr_map, 
                            transform_curse2game_coordinate(pos));
                _rendering_cell_map(curr_map);
                move(pos.row, pos.col);
                break;
            
            case 'q':      case KEY_F(1): case KEY_F(2):
            case KEY_F(3): case KEY_F(5): 
                curs_set(0);
                clear_message();
                print_message("Are you sure? Please press the button one more time!");
                return;
            default:
                break;
        }
    }
    curs_set(0);
}

/**
 * @brief 
 * Curse coordinate to game coordinate : implemented by linear transform
 */
coordinate_t
transform_curse2game_coordinate(coordinate_t origin)
{
    coordinate_t new;

    new.row = origin.row - LINES/2;
    new.col = (origin.col - COLS/2)/2;

    return new;
}

/**
 * @brief 
 * Game coordinate to curse coordinate : implemented by linear transform
 */
coordinate_t
transform_game2curse_coordinate(coordinate_t origin)
{
    coordinate_t new;

    new.row = origin.row + LINES/2;
    new.col = 2*origin.col + COLS/2;

    return new;
}

/**
 * @brief 
 * Checking valid curse coordinate
 */
bool
is_valid_coordinate(coordinate_t origin)
{
    if (origin.row < 3 || origin.row > LINES - 4) return false;
    if (origin.col < 2 || origin.col > COLS - 2) return false;
    return true;
}