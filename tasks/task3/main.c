#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "wordlist.h"

#define BUFFER_SIZE 256

int is_word_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
           (c >= '0' && c <= '9') || c == '$' || c == '_' || 
           c == '/' || c == ',';
}

int is_special_char(char c) {
    return c == '|' || c == '&' || c == ';' || c == '>' || 
           c == '<' || c == '(' || c == ')' || c == '.';
}

WordList* parse_line(const char *line) {
    WordList *list = wordlist_create();
    if (!list) return NULL;
    
    size_t len = strlen(line);
    size_t i = 0;
    
    while (i < len) {
        if (isspace(line[i])) {
            i++;
            continue;
        }
        
        if (i + 1 < len && line[i] == '|' && line[i + 1] == '|') {
            wordlist_add(list, "||");
            i += 2;
        }
        else if (i + 1 < len && line[i] == '&' && line[i + 1] == '&') {
            wordlist_add(list, "&&");
            i += 2;
        }
        else if (i + 1 < len && line[i] == '>' && line[i + 1] == '>') {
            wordlist_add(list, ">>");
            i += 2;
        }
        else if (line[i] == '|') {
            wordlist_add(list, "|");
            i++;
        }
        else if (line[i] == '&') {
            wordlist_add(list, "&");
            i++;
        }
        else if (line[i] == '>') {
            wordlist_add(list, ">");
            i++;
        }
        else if (line[i] == '<') {
            wordlist_add(list, "<");
            i++;
        }
        else if (line[i] == ';') {
            wordlist_add(list, ";");
            i++;
        }
        else if (line[i] == '(') {
            wordlist_add(list, "(");
            i++;
        }
        else if (line[i] == ')') {
            wordlist_add(list, ")");
            i++;
        }
        else if (line[i] == '.') {
            wordlist_add(list, ".");
            i++;
        }
        else {
            size_t start = i;
            
            while (i < len && !isspace(line[i]) && !is_special_char(line[i])) {
                i++;
            }
            
            if (i > start) {
                size_t word_len = i - start;
                char *word = malloc(word_len + 1);
                if (word) {
                    strncpy(word, line + start, word_len);
                    word[word_len] = '\0';
                    wordlist_add(list, word);
                    free(word);
                }
            }
        }
    }
    
    return list;
}

char* read_input_line(void) {
    size_t buffer_size = BUFFER_SIZE;
    size_t length = 0;
    char *buffer = malloc(buffer_size);
    
    if (!buffer) return NULL;
    
    int ch;
    while ((ch = getchar()) != EOF && ch != '\n') {
        if (length + 1 >= buffer_size) {
            buffer_size *= 2;
            char *new_buffer = realloc(buffer, buffer_size);
            if (!new_buffer) {
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
        }
        
        buffer[length++] = ch;
    }
    
    if (length == 0 && ch == EOF) {
        free(buffer);
        return NULL;
    }
    
    buffer[length] = '\0';
    return buffer;
}

int main(void) {
    
    
    while (1) {
       
        
        char *line = read_input_line();
        if (!line) break;
        
        if (strlen(line) == 0) {
            free(line);
            continue;
        }
        
        WordList *word_list = parse_line(line);
        if (!word_list) {
            fprintf(stderr, "Ошибка: не удалось создать список слов\n");
            free(line);
            continue;
        }
        
        printf("\n");
        wordlist_print_with_count(word_list);
        
        printf("\n");
        wordlist_sort(word_list);
        wordlist_print_elements(word_list);
        
        const char *most_common = find_most_common_special(word_list);
        if (most_common != NULL) {
            printf("Самый частый специальный символ: \"%s\"\n", most_common);
        } else {
            printf("Специальные символы не найдены\n");
        }
        
        wordlist_destroy(word_list);
        free(line);
        printf("\n");
    }
    
    printf("\n");
    return 0;
}
