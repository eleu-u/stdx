#ifndef JSONX_H
#define JSONX_H

#include <string.h>
#include <ctype.h>

/* this is a very early unstable development version of a json parser im working on */
/* many things will break and Not work as i optimize and add more things so please dont use this (yet) if you just stumbled upon this repo */
/* i only published this for Version control */

/* 

TODO:
    - add support for nested json objects
    - add support for json arrays (like with a json_array function that returns a malloced block of memory with the elements maybe)
*/

#define json_fastget(json, key, len) ({ \
    char buf[len]; \
    json_get(json, key, buf); \
    buf; \
})

int json_get(char* json, const char* key, char* buf) {
    /* creates a new string that starts where the key is */
    char* walk_start = strstr(json, key);
    if (walk_start == NULL) {
        return -1;
    }

	/* making it point to after the key name, quote and colon (like meow": "yes"} would turn to ` "yes"`)*/
	walk_start += strlen(key) + 2;

    /* we start iterating over walk_start and writing its contents to the buffer provided (like strcpy) until we either find a , or a } or a " */
    size_t buffer_size = 0;
    char is_string = 0; /* bool */

    for (int i = 0;; i++) {
        /* prevents leading whitespace from getting written to the buffer */
        if (buffer_size == 0 && isspace(walk_start[i])) continue;
        /* prevents the opening quote from a string getting written to the buffer */
        if (buffer_size == 0 && walk_start[i] == '\"') {is_string = 1; continue;}
        
        /* if the value is not a string and we run into these specific characters then we stop writing to buffer */
        /* EOF and \0 will never show up in a valid json string at this point but i just want to add it here for safety */
        if (!is_string) {
            if ((walk_start[i] == ',') || (walk_start[i] == '}') || (isspace(walk_start[i])) || (walk_start[i] == EOF) || (walk_start[i] == '\0')) break;
        }

        /* if this is a string and we run into an ending quote " */
        if (walk_start[i] == '\"') {
            /* if the character before wasn't a \, we stop writing to the buffer */
            /* this is to allow \" escape sequences */
            if ((walk_start[i-1] != '\\')) {
                break;
            }
        }

        /* writing character to char buffer */
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

    /* we finished constructing the string, so now we have to put a null character */
    buf[buffer_size] = '\0';

    return 1;
}

#endif /* JSONX_H */