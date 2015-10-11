/*
 * ============================================================================
 *
 *       Filename:  parsing.c
 *
 *    Description:  File responsible of the syntactic analysis of the json 
 *                  object
 *
 *        Version:  1.0
 *        Created:  09/15/2015 07:58:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Gustavo Pantuza, 
 *   Organization:  Software Community
 *
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "lexical.h"
#include "parsing.h"
#include "colors.h"
#include "tokens.h"


/*
 * Prints the json until the error and exits
 */
void
print_json_error (char json[])
{
    fprintf(stdout, "%s", json);
    exit(EXIT_FAILURE);
}



/*
 * Prints token value into the result json
 */
void
add_token (struct token *token, char json[])
{
    /* Always there is 2 colors in the string: token->color and NO_COLOR */ 
    int colors_size = 2 * COLOR_STR_SIZE;

    int size = strlen(token->value) + colors_size;
    char value[size];

    /* Formats the new value */
    snprintf(value, size, "%s%s%s", token->color, token->value, NO_COLOR);
    strncat(json, value, size);

    if(token->type == TOKEN_ERROR) {
        print_json_error(json);
    }
}



/*
 * Object parser
 */
void
parse_object (struct token *token)
{ 
    char value[4] = {OBJECT_OPEN, '\n', '\t', '\0'};
    update_token(token, OBJECT_OPEN, GRAY, value, 0, 1);
}



/*
 * Array parser 
 */
void
parse_array (struct token *token)
{
    char value[3] = {ARRAY_OPEN, ' ', '\0'};
    update_token(token, ARRAY_OPEN, GRAY, value, 0, 1);
}



/*
 * String parser
 */
void
parse_string (struct token *token, char already_read, FILE *file)
{
    char value[2] = {already_read, '\0'};
    char character = getc(file);

    unsigned int str_size = DEFAULT_VALUE_LENGTH;
    do {
        /* If a string is becoming very long we realloc the value variable */
        if(strlen(value) == str_size) {
            str_size += DEFAULT_VALUE_LENGTH;

            if(!realloc(value, str_size)) {
                fprintf(stderr, RED "Realloc string value error\n" NO_COLOR);
                exit(EXIT_FAILURE);
            }
        }
        sprintf(value, "%s%c", value, character);

        if(character == EOF) {
            fprintf(stdout, RED "Malformed string\n" NO_COLOR);
            exit(EXIT_FAILURE);
        }
        character = getc(file);

    } while (character != STRING_0 && character != STRING_1);

    // strncat(value, &character, 1); 
    sprintf(value, "%s%c", value, character);

    update_token(token, STRING_TOKEN, BLUE, value, 1, 4);
}



/*
 * Constant values parser
 */
void
constant ()
{

}



/*
 * Number parser
 */
void
parse_number (struct token *token, char character, FILE *file)
{
    char value[2] = {character, '\0'};
    char next_char = getc(file);

    while(isdigit(next_char)) {
        strncat(value, &next_char, sizeof(char));
        next_char = getc(file);
    }

    ungetc(next_char, file);
    update_token(token, NUMBER, GREEN, value, 0, 1);
}



/*
 * Separators parser
 */
void
parse_pair_separator (struct token *token)
{
    char value[3] = {PAIR_SEPARATOR, ' ', '\0'};
    update_token(token, PAIR_SEPARATOR, BROWN, value, 0, 1);
}



/*
 * Value separator parser
 */
void
parse_value_separator (struct token *token)
{
    char value[4] = {VALUE_SEPARATOR, '\n', '\t', '\0'};
    update_token(token, VALUE_SEPARATOR, BROWN, value, 1, 0);
}
