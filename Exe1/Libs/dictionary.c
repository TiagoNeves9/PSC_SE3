#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "dictionary.h"



Dictionary *dictionary_create() {
    Dictionary *dict = (Dictionary *)malloc(sizeof(Dictionary));
    dict->hash_table = g_hash_table_new(g_str_hash, g_str_equal);
    return dict;
}

void dictionary_add(Dictionary *dictionary, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", filename);
        return;
    }

    char word[100];  // Ajuste o tamanho conforme necessário
    while (fscanf(file, "%s", word) != EOF) {
        // Adiciona a palavra à hash table
        g_hash_table_insert(dictionary->hash_table, g_strdup(word), NULL);
    }

    fclose(file);
}

int dictionary_lookup(Dictionary *dictionary, const char *word) {
    // Verifica se a palavra está na hash table
    return g_hash_table_contains(dictionary->hash_table, word);
}

void dictionary_destroy(Dictionary *dictionary) {
    // Libera os recursos associados à hash table
    g_hash_table_destroy(dictionary->hash_table);
    free(dictionary);
}

