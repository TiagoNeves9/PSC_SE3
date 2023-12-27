#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "dictionary.h"


void free_key(gpointer key) {
    g_free(key);
}

Dictionary *dictionary_create() {
    Dictionary *dict = (Dictionary *)malloc(sizeof(Dictionary));
    if (dict == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o dicionário\n");
        exit(EXIT_FAILURE);  // ou outra ação apropriada
    }
    dict->hash_table = g_hash_table_new_full(g_str_hash, g_str_equal, free_key,NULL);
    return dict;
}

void dictionary_add(Dictionary *dictionary, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", filename);
        fclose(file);
        return;
    }

    char word[100];
    while (fscanf(file, "%99s", word) == 1) {
        g_hash_table_insert(dictionary->hash_table, g_strdup(word), NULL);
    }

    if (fclose(file) != 0) {
        fprintf(stderr, "Erro ao fechar o arquivo %s\n", filename);
    }
}


int dictionary_lookup(Dictionary *dictionary, const char *word) {
    // Estrutura de dados para armazenar a palavra durante a iteração
    struct {
        const char *word;
        int found;
    } lookup_data = { word, 0 };

    // Função de callback para a iteração
    void lookup_callback(gpointer key, gpointer value, gpointer user_data) {
        const char *current_word = (const char *)key;
        struct {
            const char *word;
            int found;
        } * data = user_data;

        if (strcmp(current_word, data->word) == 0) {
            // A palavra foi encontrada na tabela hash
            data->found = 1;  // Verdadeiro
        }
    }

    // Itera sobre a tabela hash usando a função de callback
    g_hash_table_foreach(dictionary->hash_table, lookup_callback, &lookup_data);

    return lookup_data.found;
}

void dictionary_destroy(Dictionary *dictionary) {
    // Libera os recursos associados à hash table
    g_hash_table_destroy(dictionary->hash_table);
    free(dictionary);
}

