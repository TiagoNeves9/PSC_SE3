#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Função para verificar se uma palavra está no dicionário
int isInDictionary(char *word, char *dictionary[], int dictionary_size) {
    for (int i = 0; i < dictionary_size; i++) {
        if (strcmp(word, dictionary[i]) == 0) {
            return 1; // Encontrou a palavra no dicionário
        }
    }
    return 0; // A palavra não está no dicionário
}

// Função para verificar o texto e imprimir palavras não encontradas
void spellCheckText(char *filename, char *dictionary[], int dictionary_size) {
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

        if (!isInDictionary(buffer, dictionary, dictionary_size)) {
            // A palavra não está no dicionário, armazenar informações
            strcpy(words[words_count].word, buffer);
            words[words_count].coordinates[words[words_count].coordinates_count].line = line_count;
            words[words_count].coordinates[words[words_count].coordinates_count].column = ftell(file) - strlen(buffer) - 1;
            words[words_count].coordinates_count++;
            words_count++;
        }

        if (buffer[len - 1] == '\n') {
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

int main(int argc, char *argv[]) {
    char *filename = NULL;
    char *word = NULL;
    char *dictionary[MAX_DICTIONARY_WORDS];
    int dictionary_size = 0;

    // Processar argumentos de linha de comando
    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-t") == 0) {
            filename = argv[i + 1];
        } else if (strcmp(argv[i], "-w") == 0) {
            word = argv[i + 1];
        } else if (strcmp(argv[i], "-d") == 0) {
            // Adicionar arquivo ao dicionário
            FILE *dict_file = fopen(argv[i + 1], "r");
            if (dict_file == NULL) {
                perror("Erro ao abrir o arquivo do dicionário");
                exit(EXIT_FAILURE);
            }

            char dict_word[MAX_WORD_LENGTH];
            while (fscanf(dict_file, "%s", dict_word) == 1) {
                dictionary[dictionary_size] = malloc(strlen(dict_word) + 1);
                strcpy(dictionary[dictionary_size], dict_word);
                dictionary_size++;
            }

            fclose(dict_file);
        }
    }

    // Verificar se as opções estão corretas
    if ((filename == NULL && word == NULL) || (filename != NULL && word != NULL)) {
        fprintf(stderr, "Opções inválidas. Use -t OU -w.\n");
        exit(EXIT_FAILURE);
    }

    // Executar verificação ortográfica
    if (filename != NULL) {
        spellCheckText(filename, dictionary, dictionary_size);
    } else {
        // Verificar palavra isolada
        if (!isInDictionary(word, dictionary, dictionary_size)) {
            printf("Palavra não encontrada: %s\n", word);
        } else {
            printf("Palavra encontrada no dicionário: %s\n", word);
        }
    }

    // Liberar memória alocada para o dicionário
    for (int i = 0; i < dictionary_size; i++) {
        free(dictionary[i]);
    }

    return 0;
}

