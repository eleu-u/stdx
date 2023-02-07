#ifndef STRINGX_H
#define STRINGX_H

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

/* this is for making it easier to use strcmp! */
#define strcmpx(s1, s2) (strcmp(s1,s2) == 0)

/* this is for making it easier to use strstr to check if a substring exists in a string ! */
#define strfind(needle, haystack) (strstr(needle, haystack) != NULL)

/* creates an exact copy of a string on the heap */
/* (THIS FUNCTION ALLOCATES MEMORY AND THUS THE RETURN VALUE SHOULD BE FREED) */
char* strdupx(const char* src) {
    char* dst = (char*)malloc(strlen(src) + 1);

    if (dst == NULL) return NULL;

    strcpy(dst, src);
    return dst;
}

/* gets rid of leading whitespace, input has to be mutable */
char* strtrim(char *str) {
    while (*str == ' ') { 
		str++;
	}

    char* ptr = str + strlen(str) - 1;
    while (*ptr == ' ') { 
		*ptr = '\0'; 
		ptr--; 
	}

    return str;  // return pointer to the modified start 
}

/* replaces every instance of a character with another character, input has to be mutable */
void strchrepl(char* str, char find, char replace) {
	char *current_pos = strchr(str,find);
	while (current_pos) {
		*current_pos = replace;
		current_pos = strchr(current_pos,find);
	}
}

/* checks if a string starts with another string */
int strprefix(char* str, char* prefix){
    while(*prefix) {
        if(*prefix++ != *str++) {
            return 0;
		}
    }

    return 1;
}

/* checks if a string ends with another string */
int strsuffix(char* str, char* suffix) {
	size_t str_len = strlen(str);
	size_t suffix_len = strlen(suffix);

	return (str_len >= suffix_len) && (!memcmp(str + str_len - suffix_len, suffix, suffix_len));
}

/* checks if a string starts and ends with a double quote */
int strquot(char* str) {
    return (strprefix(str, "\"") && strsuffix(str, "\""));
}

/* returns an array of strings split by a token */
/* first argument has to be mutable */
/* (THIS FUNCTION ALLOCATES MEMORY AND THUS THE RETURN VALUE SHOULD BE FREED) */
char** strsplt(char str[], const char* delim, int* array_size) {
	char* token = strtok(str, delim);
	char** array = NULL; // array size

	int i;
	for (i = 0; token != NULL; i++) {
        array = (char**)realloc(array, sizeof(char*) * (i + 1)); // resizing array based on amount of elementss
		array[i] = token;

		token = strtok(NULL, delim); // next token
	}

    if (array_size != NULL) *array_size = i;
	return array;
}

/* returns an array of strings split by a token except for when it is enclosed with "double quotes", useful for writing interpreters */
/* first argument has to be mutable */
/* the last argument (add_quotes) determines whether quotes should be added to strings that were enclosed with quotes, this will however use malloc so make sure you dont forget to free the quoted strings */ 
/* if that doesnt make sense just pass false into it */
/* (THIS FUNCTION ALLOCATES MEMORY AND THUS THE RETURN VALUE SHOULD BE FREED) */
char** strspltq(char* str, const char* delim, int* array_size, int add_quotes) {
	int quotstr = (*str == '\"'); // boolean
	char* currquot = strtok(str, "\"");
	char* next = NULL;

	char** array = NULL;

	int i = 0;
	while (currquot != NULL) {
		if (quotstr) { // if surrounded in quotes
            // array resizing
            array = (char**)realloc(array, sizeof(char*) * (i + 1));

            if (add_quotes) { // if we should add quotes to the final string
                char* buf;
                asprintf(&buf, "\"%s\"", currquot); // this calls malloc

                array[i] = buf;
            }
            else { // if not just leave it alone
                array[i] = currquot;
            }

			i++;
			quotstr = 0; // false
		}
		else { // if normal token delim
			next = strtok(next, "\0");

			char* currword = strtok(currquot, delim);
			while (currword) {
                // array resizing
                array = (char**)realloc(array, sizeof(char*) * (i + 1));
				array[i] = currword;

				i++;
				currword = strtok(NULL, delim);
			}
			quotstr = 1; // true
		}

		currquot = strtok(next, "\"");
		next = NULL;
	}

	if (array_size != NULL) *array_size = i;
	return array;
}

#endif /* STRINGX_H */