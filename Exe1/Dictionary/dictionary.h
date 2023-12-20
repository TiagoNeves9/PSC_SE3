#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <glib.h>

typedef struct {
    GHashTable *hash_table;
} Dictionary;

Dictionary *dictionary_create();
void dictionary_add(Dictionary *dictionary, const char *filename);
int dictionary_lookup(Dictionary *dictionary, const char *word);
void dictionary_destroy(Dictionary *dictionary);

#endif

