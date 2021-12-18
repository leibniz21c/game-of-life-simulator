/*
 * ============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Main file of the project
 *
 *        Created:  07/12/2021
 *       Compiler:  cc
 *
 *         Author:  Heesung Yang
 *   Organization:  Kyoungpook National University
 *
 * ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "args.h"
#include "messages.h"
#include "colors.h"
#include "game.h"
#include "renderer.h"

int
main (int argc, char* argv[])
{
    /* Read command line options */
    options_t options;
    options_parser(argc, argv, &options);

    if (options.new)                    /* -n|--new  */
        new_game(options.file_name);
    else if (options.load)              /* -l|--load */
        load_game(options.file_name);
    else 
        help();

    return EXIT_SUCCESS;
}

