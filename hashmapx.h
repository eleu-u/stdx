#ifndef HASHMAPX_H
#define HASHMAPX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// hashmap type
typedef struct HASHMAP_PAIR {
	char* key;
	void* value;
	struct HASHMAP_PAIR* next;
} HASHMAP_PAIR;

typedef struct {
	HASHMAP_PAIR** pairs;
	size_t max_size;
} HASHMAP;

// creates a size_t interger from a string to use for hashing
size_t hashmap_hash(const char* key, size_t max_size) {
	size_t value = 0;

	for (size_t i = 0; key[i] != '\0'; i++) {
		value = value * 37 + key[i];
	}

	value = value % max_size;
	return value;
}

HASHMAP_PAIR* hashmap_new_pair(char* key, void* value) {
	HASHMAP_PAIR* pair = malloc(sizeof(HASHMAP_PAIR));
	pair->key = key;
	pair->value = value;

	/* default is NULL but may be set later on */
	pair->next = NULL;

	return pair;
}

HASHMAP* hashmap_new(size_t max_size) { 
	HASHMAP* to_return = malloc(sizeof(HASHMAP));

	/* allocate entries */
	to_return->pairs = malloc(sizeof(HASHMAP_PAIR*) * max_size);
	to_return->max_size = max_size;

	/* set all entries to null */
	for (int i = 0; i < max_size; i++) {
		to_return->pairs[i] = NULL;
	}

	return to_return;
}

void hashmap_set(HASHMAP* hashmap, char* key, void* value) {
	size_t slot = hashmap_hash(key, hashmap->max_size);

	/* try to look up a slot */
	HASHMAP_PAIR* pair = hashmap->pairs[slot];

	/* no slot means that its empty, so we immediately insert */
	if (pair == NULL) {
		hashmap->pairs[slot] = hashmap_new_pair(key, value);
		return;
	}

	/* if we made it here that means that the slot we looked up */
	/* isnt empty, so we check if the pair we're looking for exists already to replace it */

	HASHMAP_PAIR* prev;
	while(pair != NULL) {
		if (!strcmp(pair->key, key)) {
			pair->value = value;
			return;
		}

		prev = pair;
		pair = prev->next;
	}

	/* pair doesnt exist, so we insert it to the end of the list */
	prev->next = hashmap_new_pair(key, value);
}

void* hashmap_get(HASHMAP* hashmap, char* key) { 
	size_t slot = hashmap_hash(key, hashmap->max_size);

	/* search for slot */
	HASHMAP_PAIR* pair = hashmap->pairs[slot];

	/* no slot means nothing exists here */
	if (pair == NULL) {
		return NULL;
	}

	/* walk through each pair in the slot, if the key matches */
	/* we return its value*/
	for (; pair != NULL; pair = pair->next) {
		if (!strcmp(pair->key, key)) {
			return pair->value;
		}
	}
	
	/* reaching here means that some pairs did exist but the key didnt match */
	return NULL;
}

/* only works if every type value inside of the hashmap is a char* */
void hashmap_print(HASHMAP* hashmap) {
    for (int i = 0; i < hashmap->max_size; i++) {
        HASHMAP_PAIR* pair = hashmap->pairs[i];

        printf("slot[%d]: ", i);

		/* we dont pairs that dont exist of course */
		if (pair == NULL) { printf("\n"); continue; }

        for(; pair != NULL; pair = pair->next) {
            printf("%s=%s ", pair->key, (char*)pair->value);
        }

        printf("\n");
    }
}

void hashmap_free(HASHMAP* hashmap) {
	for (int i = 0; i < hashmap->max_size; i++) {
        HASHMAP_PAIR* pair = hashmap->pairs[i];
		if (pair == NULL) continue;

		HASHMAP_PAIR* tmp;
		while(pair != NULL) {
			tmp = pair;
			pair = pair->next;
            free(tmp);
        }
	}

	free(hashmap->pairs);
	free(hashmap);
}

#endif /* HASHMAPX_H */