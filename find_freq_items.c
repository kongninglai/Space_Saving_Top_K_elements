#include "find_freq_items.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main() {
    char* filename = "test_files/input.txt";
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file %s\n", filename);
        exit(1);
    }

    char scan_string[128];
    char word[20];
    char* delim = " .,-_:!/()\"'";

    while (fscanf(file, "%s", scan_string) > 0) {
        char* token = strtok(scan_string, delim);
        while (token != NULL) {
            strncpy(word, token, sizeof(word));
            word[sizeof(word) - 1] = '\0';
            for (size_t i = 0; i < strlen(word); i++) {
                word[i] = tolower(word[i]);
            }
            printf("%s\n", word);
            token = strtok(NULL, delim);
        }
    }

    fclose(file);
    return 0;
}

