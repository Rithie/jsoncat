/*
 * ============================================================================
 *
 *       Filename:  args.c
 *
 *    Description:  Command line options parser using GNU getopt
 *
 *        Version:  1.0
 *        Created:  09/15/2015 08:17:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Gustavo Pantuza, 
 *   Organization:  Software Community
 *
 * ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "args.h"
#include "colors.h"



/*
 * Sets the default jsoncat options
 */
static void set_default_options(options_t* options)
{
    options->help = false;
    options->version = false;
    options->use_colors = true;
}



/* TODO: Move this functions to a messages file */
void help () { }
void version () { }
void usage () { }



/* 
 * Finds the matching case of the current command line option
 */
void
switch_options (int arg, options_t* options)
{
    switch (arg)
    {
        case 'h':
            options->help = true;
            help();
            exit(EXIT_SUCCESS);
        
        case 'v':
            options->version = true;
            version();
            exit(EXIT_SUCCESS);

        case 0:
            options->use_colors = false;
            break;

        case '?':
            usage();
            exit(EXIT_FAILURE);

        default:
            usage();
            abort();
    }
}



/*
 * Tries to get the file name. Otherwise, prints an error message
 */
void
get_file_name (int argc, char* argv[], options_t* options)
{

    /* If there is more arguments, probably, it is an input file */
    if (optind < argc)
    {
        strncpy(options->file_name, argv[optind++], FILE_NAME_SIZE);
    } 

    else 
    {
        fprintf(stderr, RED "Missing the input file\n" NO_COLOR);
        usage();
        exit(EXIT_FAILURE);
    }
}



/*
 * Public function that loops until command line options were parsed
 */
void
jsoncat_options_parser (int argc, char* argv[], options_t* options)
{
    set_default_options(options);

    int arg; /* Current option */

    /* getopt allowed options */
    static struct option long_options[] =
    {
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {"no-colors", no_argument, 0, 0}
    };
    /* Abbreviated options  */
    static char abbrev[2] = "hv";

    while (true)
    {
        int option_index = 0;    
        arg = getopt_long(argc, argv, abbrev, long_options, &option_index);

        /* End of the options? */
        if (arg == -1) break;

        /* Find the matching case of the argument */
        switch_options(arg, options);
    }

    /* Gets the file name or exits with error */
    get_file_name(argc, argv, options);
}
