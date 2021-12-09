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
#include "colors.h"
#include "renderer.h"

int
main (int argc, char* argv[])
{
    /* Read command line options */
    options_t options;
    options_parser(argc, argv, &options);

#ifdef DEBUG
    fprintf(stdout, BLUE "Command line options:\n" NO_COLOR);
    fprintf(stdout, BROWN "help: %d\n" NO_COLOR, options.help);
    fprintf(stdout, BROWN "version: %d\n" NO_COLOR, options.version);
    fprintf(stdout, BROWN "use colors: %d\n" NO_COLOR, options.use_colors);
    fprintf(stdout, BROWN "filename: %s\n" NO_COLOR, options.file_name);
#endif

    if (options.new) /* User select --new */
        new_cell_map(options.file_name);
    else if (options.load) {
        /* load cell map */
        options.load;
    }

    return EXIT_SUCCESS;
}

