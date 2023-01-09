#pragma once

#include <string.h>
#include <ctype.h>

/* this is a very early unstable development version of a json parser im working on */
/* many things will break and Not work as i optimize and add more things so please dont use this (yet) if you just stumbled upon this repo */
/* i only published this for Version control */

/* 

TODO:
    - add support for , and } inside strings
    - make numbers with whitespace before the , Ignore the whitespce, like getting the key of num {"num": 50    , "test": "example"}
    - add support for nested json objects
    - add support for json arrays (like with a json_array function maybe)

*/

#define json_qget(json, key, len) ({ \
    char buf[len]; \
    json_get(json, key, buf); \
    buf; \
})

int json_get(char* json, char* key, char* buf) {
    /* creates a new string that starts where the key is */
    char* walk_start = strstr(json, key);
    if (walk_start == NULL) {
        return -1;
    }

	/* making it point to after the quote and colon (like meow": "yes"} would turn to ` "yes"`)*/
	walk_start += strlen(key) + 2;

    /* we start iterating over walk_start and writing its contents to the buffer provided (like strcpy) until we either find a , or a } */
    size_t buffer_size = 0;
    for (int i = 0;; i++) {
        /* prevents leading whitespace from getting written to the buffer */
        if (buffer_size == 0 && isspace(walk_start[i])) continue;
        /* prevents the opening quote from a string getting written to the buffer */
        if (buffer_size == 0 && walk_start[i] == '\"') continue;
        
        /* if we run into a , or a } we stop writing to buffer*/
        if ((walk_start[i] == ',') || (walk_start[i] == '}')) break;

        /* if we run into an ending quote " */
        if (walk_start[i] == '\"') {
            /* if the the last character wasn't a \, we stop writing to the buffer */
            /* this is to allow \" escape sequences */
            if ((walk_start[i-1] != '\\')) {
                break;
            }
        }

        /* handling \" escape sequence */
        /* if we run into a \" we replace it with a " */
        if (walk_start[i] == '\\' && walk_start[i+1] == '\"') {
            buf[buffer_size++] = '\"';
            /* we just replaced \ with " and so the next character that the loop will go through will be ", which will confuse it so we have to skip an iteration */
            i++; 
        }
        /* if its just like any other character we just write it */
        else {
            buf[buffer_size++] = walk_start[i];
        }
    }

    /* iterates through the string from the *end* to the start, removing any unnecessary tokens along the way (\n, \t and trailing quotes for strings) */
    /* also adds a \0 null terminator to the end */
    buf[buffer_size] = '\0';

    return 1;
}