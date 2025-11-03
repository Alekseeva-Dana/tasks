#ifndef WORDLIST_H
#define WORDLIST_H

#include <stddef.h>

typedef struct {
    const char *symbol;
    int count;
} SpecialSymbolCount;

typedef struct {
    char **words;
    size_t size;
    size_t capacity;
} WordList;

WordList* wordlist_create(void);
int wordlist_add(WordList *list, const char *word);
size_t wordlist_size(const WordList *list);
void wordlist_sort(WordList *list);
void wordlist_print_elements(const WordList *list);
void wordlist_print_with_count(const WordList *list);
void wordlist_destroy(WordList *list);
const char* find_most_common_special(const WordList *list);

#endif
