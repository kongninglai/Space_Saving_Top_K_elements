#ifndef FIND_FREQ_ITEMS_H
#define FIND_FREQ_ITEMS_H

#include "uthash.h"

#define WORD_SIZE 20
#define INT_MAX 2147483647
#define CAPACIRT 100


char* delim = " *—.,-_:?!/()\"\'";
// Sorted double linked list buckets, stores the count value of elemnts
struct Buckets {
    struct Buckets* next;
    struct Buckets* prev;
    struct Elements* element_head;

    int count;
};

struct Elements {
    struct Elements* next;
    struct Buckets* bucket;
    int error;
    char word[WORD_SIZE];
    UT_hash_handle hh;         /* makes this structure hashable */
};

struct Buckets* add_new_bucket(struct Buckets *prev_bucket);
void bucket_add_element(struct Buckets *bucket, struct Elements *element);
void bucket_delete_element(struct Buckets *bucket, struct Elements *element);
void delete_bucket(struct Buckets *bucket_to_delete);
void element_free(struct Elements *element);
void bucket_free(struct Buckets *bucket);
void free_all();
void increase_element_counter(struct Elements *element);
void add_element(char* element);
void print_buckets(struct Buckets *bucket);
void print_all_buckets();
void print_all_buckets_reverse();
#endif