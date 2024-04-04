#ifndef FIND_FREQ_ITEMS_H
#define FIND_FREQ_ITEMS_H

#include "uthash.h"
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

    char name[10];
    int id;
    UT_hash_handle hh;         /* makes this structure hashable */
};

#endif