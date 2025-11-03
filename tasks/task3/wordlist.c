#include "wordlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 16
#define CAPACITY_MULTIPLIER 2

WordList* wordlist_create(void) {
    WordList *list = malloc(sizeof(WordList));
    if (!list) return NULL;
    
    list->words = malloc(sizeof(char*) * INITIAL_CAPACITY);
    if (!list->words) {
        free(list);
        return NULL;
    }
    
    list->size = 0;
    list->capacity = INITIAL_CAPACITY;
    return list;
}

int wordlist_add(WordList *list, const char *word) {
    if (!list || !word) return 0;
    
    if (list->size >= list->capacity) {
        size_t new_capacity = list->capacity * CAPACITY_MULTIPLIER;
        char **new_words = realloc(list->words, sizeof(char*) * new_capacity);
        if (!new_words) return 0;
        
        list->words = new_words;
        list->capacity = new_capacity;
    }
    
    list->words[list->size] = malloc(strlen(word) + 1);
    if (!list->words[list->size]) return 0;
    
    strcpy(list->words[list->size], word);
    list->size++;
    return 1;
}

size_t wordlist_size(const WordList *list) {
    return list ? list->size : 0;
}

void wordlist_sort(WordList *list) {
    if (!list || list->size < 2) return;
    
    size_t i, j;
    for (i = 0; i < list->size - 1; i++) {
        for (j = 0; j < list->size - i - 1; j++) {
            if (strcmp(list->words[j], list->words[j + 1]) > 0) {
                char *temp = list->words[j];
                list->words[j] = list->words[j + 1];
                list->words[j + 1] = temp;
            }
        }
    }
}

void wordlist_print_elements(const WordList *list) {
    if (!list) return;
    
    size_t i;
    for (i = 0; i < list->size; i++) {
        printf("%s\n", list->words[i]);
    }
}

void wordlist_print_with_count(const WordList *list) {
    if (!list) return;
    
    printf("%zu\n", list->size);
    wordlist_print_elements(list);
}

void wordlist_destroy(WordList *list) {
    if (!list) return;
    
    size_t i;
    for (i = 0; i < list->size; i++) {
        free(list->words[i]);
    }
    free(list->words);
    free(list);
}

const char* find_most_common_special(const WordList *list) {
    if (!list || list->size == 0) return NULL;
    
    const char *special_symbols[] = {
        "||", "&&", ">>",
        "|", "&", ">", "<", ";", "(", ")", ".",
        NULL
    };
    
    SpecialSymbolCount counts[20];
    int num_symbols = 0;
    
    int i;
    for (i = 0; special_symbols[i] != NULL; i++) {
        counts[num_symbols].symbol = special_symbols[i];
        counts[num_symbols].count = 0;
        num_symbols++;
        if (num_symbols >= 20) break;
    }
    
    size_t j;
    for (j = 0; j < list->size; j++) {
        const char *word = list->words[j];
        
        for (i = 0; i < num_symbols; i++) {
            if (strcmp(word, counts[i].symbol) == 0) {
                counts[i].count++;
                break;
            }
        }
    }
    
    const char *most_common = NULL;
    int max_frequency = 0;
    
    for (i = 0; i < num_symbols; i++) {
        if (counts[i].count > max_frequency) {
            max_frequency = counts[i].count;
            most_common = counts[i].symbol;
        }
    }
    
    return most_common;
}
