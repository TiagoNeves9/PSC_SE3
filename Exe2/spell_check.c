#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"
#include <getopt.h>
#include <glib.h>

#define MAX_WORD_LENGTH 100
#define MAX_DICTIONARY_WORDS 1000
#define MAX_FILE_LINES 1000

// Estrutura para armazenar as coordenadas de uma palavra no texto
typedef struct {
    int line;
    int column;
} Coordinates;

// Estrutura para armazenar informações sobre uma palavra
typedef struct {
    char word[MAX_WORD_LENGTH];
    Coordinates coordinates[MAX_FILE_LINES];
    int coordinates_count;
} WordInfo;
													//TODO: Coordenadas erradas
void spellCheckText(char *filename, Dictionary *dict_array[], int dictionary_size) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_WORD_LENGTH];
    WordInfo words[MAX_DICTIONARY_WORDS];
    int words_count = 0;
    int line_count = 1;

    while (fscanf(file, "%s", buffer) == 1) {
        // Remover pontuações simples no final das palavras
        int len = strlen(buffer);
        if (len > 0 && (buffer[len - 1] == '.' || buffer[len - 1] == ',')) {
            buffer[len - 1] = '\0';
        }
        
        long int word_start_position = ftell(file) - len;

        int found_in_any_dictionary = 0; // Flag para verificar se a palavra foi encontrada em algum dicionário

        for (int i = 0; i < dictionary_size; i++) {
            if (dictionary_lookup(dict_array[i], buffer)) {
                found_in_any_dictionary = 1;
                break; // Se encontrou em algum dicionário, não precisa verificar os outros
            }
        }

        if (!found_in_any_dictionary) {
            // A palavra não está em nenhum dos dicionários, armazenar informações
            strcpy(words[words_count].word, buffer);
            words[words_count].coordinates[words[words_count].coordinates_count].line = line_count;
            words[words_count].coordinates[words[words_count].coordinates_count].column = word_start_position;
            words[words_count].coordinates_count++;
            words_count++;
        }

        if (strchr(buffer, '\n') != NULL) {
            line_count++;
        }
    }

    fclose(file);

    // Imprimir palavras não encontradas
    for (int i = 0; i < words_count; i++) {
        printf("Palavra: %s\n", words[i].word);
        printf("Coordenadas:");
        for (int j = 0; j < words[i].coordinates_count; j++) {
            printf(" (%d, %d)", words[i].coordinates[j].line, words[i].coordinates[j].column);
        }
        printf("\n\n");
    }
}




int main(int argc, char *argv[]){
	
	int spell_check = 0;
    char *dicionarios[10]; // Um array para armazenar os nomes dos dicionários
    char *text = NULL;
    char *word = NULL;
    Dictionary *dict_arr[10];
    
    int opt;
    while((opt = getopt(argc, argv, "d:t:w:")) != -1){
		switch(opt){
			case 'd':
				dicionarios[spell_check] = optarg;
				spell_check++;
				break;
			case 't':
				text = optarg;
				break;
			case 'w':
				word = optarg;
				break;
			default:
				fprintf(stderr, "Uso: %s -d dicionario1 -d dicionario2 (etc) -t arquivo OU -w palavra isolada\n", argv[0]);
                exit(EXIT_FAILURE);
       }         
	}
	
	if( (text != NULL && word != NULL) || (text == NULL && word == NULL)){
		fprintf(stderr, "Uso: %s -d dicionario1 -d dicionario2 (etc) -t arquivo OU -w palavra isolada\n", argv[0]);
        exit(EXIT_FAILURE);
    }    
	
	if (spell_check > 0) {
        printf("Ativando verificação ortográfica...\n");
        
        for (int i = 0; i < spell_check; i++) {
            printf("Usando o dicionário: %s\n", dicionarios[i]);
            dict_arr[i] = dictionary_create();
			if (dict_arr[i] == NULL) {
			fprintf(stderr, "Erro ao criar o dicionário\n");

            // Libertar os dicionários previamente criados em caso de erro
            for (int j = 0; j < i; j++) {
                dictionary_destroy(dict_arr[j]);
            }
            return EXIT_FAILURE;
        }
			dictionary_add(dict_arr[i], dicionarios[i]);
        }
    } else {
		fprintf(stderr, "Uso: %s -d dicionario1 -d dicionario2 (etc) -t arquivo OU -w palavra isolada\n", argv[0]);
        exit(EXIT_FAILURE);
	}
    
    if (text != NULL) {
        printf("Processando o arquivo de texto: %s...\n", text);
        spellCheckText(text,dict_arr,spell_check);
        
    }
    
    if(word != NULL){
		printf("Processando a palavra: %s...\n", word);
		for(int z=0; z < spell_check; z++){
			if(dictionary_lookup(dict_arr[z],word)){
				printf("A palavra está no dicionário %s .\n", dicionarios[z]);
			} else {
				printf("A palavra não está no dicionário %s .\n", dicionarios[z]);	
			}
		} 
	}	
	
	printf("A libertar os dicionários...\n");
	for(int k=0; k < spell_check; k++){
		dictionary_destroy(dict_arr[k]);
	}
	
	return 1;
}
