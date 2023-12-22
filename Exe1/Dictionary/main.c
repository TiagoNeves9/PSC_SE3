#include <stdio.h>
#include "dictionary.h"
#include <glib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <arquivo_dicionario> <palavra>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *dictionary_file = argv[1];
    const char *word_to_lookup = argv[2];

    Dictionary *dict = dictionary_create();
	if (dict == NULL) {
		fprintf(stderr, "Erro ao criar o dicionário\n");
		return EXIT_FAILURE;
	}
	
	dictionary_add(dict, dictionary_file);
   
	if (dictionary_lookup(dict, word_to_lookup)) {
        printf("A palavra está no dicionário.\n");
    } else {
        printf("A palavra não está no dicionário.\n");
    }

	dictionary_destroy(dict);

    return EXIT_SUCCESS;
}

