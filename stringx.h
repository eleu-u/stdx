#pragma once

#include <string.h>
#include <stdarg.h>

// this is for making it easier to use strcmp!
#define strcmpx(s1,s2) strcmp(s1,s2) == 0

char* strdupx(const char* src) {
    char* dst = malloc(strlen(src) + 1);

    if (dst == NULL) return NULL;

    strcpy(dst, src);
    return dst;
}

char* string_convert(int to_convert, char* type){
	char* str;
	asprintf(&str, type, to_convert);
	return str;
}

char* string_remove_last(char* str) {
	str[strlen(str)-1] = '\0';
	return str;
}

char* string_format(char* fmt, ...){
	char* buff = malloc(2048);
	
	va_list args;
	va_start(args, fmt);
	
	vsprintf(buff, fmt, args);
	
	va_end(args);
	
	return buff;
}

char* string_trim(char *str) {
    char *ptr = NULL;
    while (*str == ' ') { 
		str++;
	}
    ptr = str + strlen(str) - 1;
    while (*ptr == ' ') { 
		*ptr = '\0' ; ptr--; 
	} 
    return str;  // return pointer to the modified start 
}

char* string_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep) {
        return NULL;
	}
    len_rep = strlen(rep);
    if (len_rep == 0) {
        return NULL;
	}
    if (!with) {
        with = "";
	}
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result) {
        return NULL;
	}

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

char* string_char_replace(char* str, char find, char replace) {
	char *current_pos = strchr(str,find);
	while (current_pos) {
		*current_pos = replace;
		current_pos = strchr(current_pos,find);
	}
	return str;
}

int string_find(char* str, char* toFind){
    int slen = strlen(str);
    int tFlen = strlen(toFind);
    int found = 0;

    if(slen >= tFlen) {  
        int s, t = 0;
        for(; s < slen; s++) {
            do{

                if( str[s] == toFind[t] )
                {
                    if( ++found == tFlen ) return 1;
                    s++;
                    t++;
                }
                else { s -= found; found=0; t=0; }

            } while(found);
        }
        return 0;
    }
    else 
        return 0;
}

int string_startswith(char* str, char* prefix){
    while(*prefix) {
        if(*prefix++ != *str++) {
            return 0;
		}
    }

    return 1;
}

int string_endswith(char* str, char* suffix) {
	size_t str_len = strlen(str);
	size_t suffix_len = strlen(suffix);

	return (str_len >= suffix_len) && (!memcmp(str + str_len - suffix_len, suffix, suffix_len));
}

int string_quoted(char* str) {
    return (string_startswith(str, "\"") && string_endswith(str, "\""));
}

char** string_split(char str[], char* delim, int* array_size) {
	char* token = strtok(str, delim);
	char** array = malloc(sizeof(char*) * 1); // array size

	int i;
	for (i = 0; token != NULL; i++) {
		array[i] = token;
		array = realloc(array, sizeof(char*) * (i + 2)); // resizing array based on amount of elementss

		token = strtok(NULL, delim); // next token
	}

    if (array_size != NULL) *array_size = i;
	return array;
}

char** string_split_quotes(char* str, char* delim, int* array_size, int add_quotes) {
	int quotstr = (*str == '\"'); // boolean
	char* currquot = strtok(str, "\"");
	char* next = NULL;

	char** array = malloc(sizeof(char*) * 1); // array size

	int i = 0;
	while (currquot != NULL) {
		if (quotstr) { // if surrounded in quotes
            if (add_quotes) { // if we should add quotes to the final string
                char* buf;
                asprintf(&buf, "\"%s\"", currquot); // this calls malloc

                array[i] = buf;
            }
            else { // if not just leave it alone
                array[i] = currquot;
            }

            // resizing return array
			array = realloc(array, sizeof(char*) * (i + 2));
			i++;
			quotstr = 0; // false
		}
		else { // if normal token delim
			next = strtok(next, "\0");

			char* currword = strtok(currquot, delim);
			while (currword) {
				array[i] = currword;

                // array resizing
				array = realloc(array, sizeof(char*) * (i + 2));
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
