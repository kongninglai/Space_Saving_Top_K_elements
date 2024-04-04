#include "find_freq_items.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Elements *hash_head = NULL;
void add_element(char* element){
    struct Elements *e;
    HASH_FIND_STR(hash_head, element, e);
    if (e == NULL){
        e = (struct Elements *)malloc(sizeof *e);
        e->count = 1;
        strncpy(e->word, element, WORD_SIZE);
        HASH_ADD_STR( hash_head, word, e );
    }else{
        e->count ++;
    }
}

int main() {
    char* filename = "test_files/input.txt";
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file %s\n", filename);
        exit(1);
    }

    char scan_string[128];
    char word[WORD_SIZE];
    char* delim = " .,-_:!/()\"'";
    struct Elements *s;
    while (fscanf(file, "%s", scan_string) > 0) {
        char* token = strtok(scan_string, delim);
        while (token != NULL) {
            strncpy(word, token, WORD_SIZE);
            word[WORD_SIZE - 1] = '\0';
            for (size_t i = 0; i < WORD_SIZE; i++) {
                word[i] = tolower(word[i]);
            }
            printf("%s\n", word);
            add_element(word);
            token = strtok(NULL, delim);
        }
    }
    HASH_FIND_STR( hash_head, "this", s);
    if (s) printf("this's count is %d\n", s->count);
    fclose(file);
    return 0;
}

