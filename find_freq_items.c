#include "find_freq_items.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int m = CAPACIRT;
struct Buckets *bucket_head = NULL;
struct Buckets *bucket_tail = NULL;
struct Elements *hash_head = NULL;

/**
 * This function add a new bucket after @prev_bucket
 * Input: prev_bucket, the bucket after which you want to insert a new bucket
 * Output: the pointer to new bucket
 * Side Effect: malloc a new bucket, need to free it. And the double linked list
 * is modified.
*/
struct Buckets* add_new_bucket(struct Buckets *prev_bucket){
    struct Buckets *new_bucket = (struct Buckets *)malloc(sizeof *bucket_head);
    new_bucket->count = 1;
    new_bucket->element_head = NULL;
    if (prev_bucket == NULL){
        bucket_head->prev = new_bucket;
        new_bucket->next = bucket_head;
        new_bucket->prev = NULL;
        bucket_head = new_bucket;
    }else{
        new_bucket->next = prev_bucket->next;
        new_bucket->prev = prev_bucket;
        if (prev_bucket->next != NULL){
            (prev_bucket->next)->prev = new_bucket;
        }else{
            bucket_tail = new_bucket;
        }
        prev_bucket->next = new_bucket;
    }
    return new_bucket;
}

/**
 * This function add a new element @element at the end of @bucket's element list
 * Input: - bucket, the bucket you want to insert the elemnet
 *        - element, the element you want to insert.
 * Output: None
 * Side Effect: element->next is set to NULL; The bucket's element list is modified.
*/
void bucket_add_element(struct Buckets *bucket, struct Elements *element){
    struct Elements *traverse;
    if (bucket == NULL) return;
    if (bucket->element_head == NULL){
        bucket->element_head = element;
    }else{
        traverse = bucket->element_head;
        while(traverse->next != NULL){
            traverse = traverse->next;
        }
        traverse->next = element;
    }
    element->bucket = bucket;
    element->next = NULL;
}

/**
 * This function delete an elemnt @element from given bucket @bucket.
 * Input: - bucket, the bucket from which you want to delete the element
 *        - element, the element you want to delete
 * Output: None
 * Side Effect: The bucket's element list is deleted
*/
void bucket_delete_element(struct Buckets *bucket, struct Elements *element){
    struct Elements *traverse;
    traverse = bucket->element_head;
    if(traverse == NULL) return;
    if (traverse == element){
         bucket->element_head = element->next;
         return;
    }
    while(traverse->next != NULL && traverse->next != element){
        traverse = traverse->next;
    }
    if (traverse == NULL) return;
    traverse->next = element->next;
}

/**
 * This function delete a bucket from the bucket list
 * Input : - bucket_to_delete, the bucket need to be deleted
 * Output: None
 * Side Effect: the bucket list is modified, and the bucket to delete may lost.
*/
void delete_bucket(struct Buckets *bucket_to_delete){
    if (bucket_to_delete == NULL) return;
    if (bucket_to_delete->prev == NULL){
        bucket_head = bucket_to_delete->next;
    }else{
        (bucket_to_delete->prev)->next = bucket_to_delete->next;
    }
    if(bucket_to_delete->next != NULL){
        (bucket_to_delete->next)->prev = bucket_to_delete->prev;
    }else{
        bucket_tail = bucket_to_delete->prev;
    }
}

/**
 * This function free a given element
*/
void element_free(struct Elements *element){
    if (element == NULL) return;
    HASH_DEL(hash_head, element);
    free(element);
}

/**
 * This function free a given bucket, as well as all elements in it
*/
void bucket_free(struct Buckets *bucket){
    if (bucket == NULL) return;
    // first we need to free all elements in bucket
    struct Elements *element_to_delete;
    struct Elements *temp_element;
    element_to_delete = bucket->element_head;
    while(element_to_delete!=NULL){
        temp_element = element_to_delete->next;
        element_free(element_to_delete);
        element_to_delete = temp_element;
    }
    // Then free bucket
    free(bucket);
}

/**
 * This function free the entire bucket list
*/
void free_all(){
    struct Buckets *bucket;
    struct Buckets *bucket_temp;
    bucket = bucket_head;
    while(bucket != NULL){
        bucket_temp = bucket->next;
        bucket_free(bucket);
        bucket = bucket_temp;
    }
    printf("free all buckets and elements finished.\n");
}
/**
 * This function increase the given element's counter
 * It delete the element from the old bucket list, and add it into correct bucket
 * If the old bucket list is empty, delete the bucket
 * If there's no correct bucket(counter + 1), create a new bucket
*/
void increase_element_counter(struct Elements *element){
    struct Buckets *old_bucket;
    struct Buckets *temp_bucket;
    old_bucket = element->bucket;
    if (old_bucket->next == NULL || (old_bucket->next)->count != 1+old_bucket->count){
        temp_bucket = add_new_bucket(old_bucket);
        temp_bucket->count = 1+old_bucket->count;
    }else{
        temp_bucket = old_bucket->next;
    }
    bucket_delete_element(old_bucket, element);
    bucket_add_element(temp_bucket, element);
    if (old_bucket->element_head == NULL){
        delete_bucket(old_bucket);
    }
}

/**
 * This function takes an word and add it into bucket list
 * If this element is already in list,simply increase it
 * If not, if there's still capacity, add it;
 * If it has run out of capacity, replace the least count element
*/
void add_element(char* element){
    struct Elements *e;
    struct Elements *etraverse;
    struct Elements *etraverse_temp;
    struct Buckets *temp_bucket;
    HASH_FIND_STR(hash_head, element, e);
    if (bucket_head == NULL)
        return;
    if (e == NULL){
        // The element is not monitered
        e = (struct Elements *)malloc(sizeof *e);
        strncpy(e->word, element, WORD_SIZE);
        e->next = NULL;
        HASH_ADD_STR( hash_head, word, e );
        if (m>0){
            // if we still have space, add it
            m--;
            e->error = 0;
            // if the first bucket count is greater thatn 1, create a new bucket with count 1
            if (bucket_head->count != 1){
                temp_bucket = add_new_bucket(NULL);
                temp_bucket->count = 1; 
            }
            // add it to element list of the first bucket
            bucket_add_element(bucket_head, e);
        }else{
            // If we don't have space, replace the element with least hit
            etraverse = bucket_head->element_head;
            e->error = bucket_head->count;
            e->bucket = bucket_head;
            if (etraverse->next == NULL){
                bucket_head->element_head = e;
            }else{
                while((etraverse->next)->next != NULL){
                    etraverse = etraverse->next;
                }
                etraverse_temp = etraverse->next;
                etraverse->next = e;
                etraverse = etraverse_temp;
            }
            element_free(etraverse);
            increase_element_counter(e);
        }
    }else{
        increase_element_counter(e);
    }
}

/**
 * This function print a single bucket
*/
void print_buckets(struct Buckets *bucket){
    struct Elements *element;
    printf("bucket value = %d\n", bucket->count);
    element = bucket->element_head;
    while(element != NULL){
        printf("->(%s, %d)", element->word, element->error);
        element = element->next;
    }
    printf("\n");
}

/**
 * This funtion print the bucket list from head to tail, for debugging
*/
void print_all_buckets(){
    struct Buckets *bucket;
    bucket = bucket_head;
    while(bucket != NULL){
        print_buckets(bucket);
        bucket = bucket->next;
    }
}

/**
 * This funtion print the bucket list from tail to head, for debugging
*/
void print_all_buckets_reverse(){
    struct Buckets *bucket;
    bucket = bucket_tail;
    while(bucket != NULL){
        print_buckets(bucket);
        bucket = bucket->prev;
    }
}

/**
 * This function output the top k elements
*/
void find_top_k(int k){
    int i;
    struct Buckets *bucket;
    struct Elements *element;
    i = 0;
    bucket = bucket_tail;
    while(bucket != NULL){
        element = bucket->element_head;
        while(element != NULL){
            i++;
            if (i > k || i > CAPACIRT) break;
            printf("%d: %s, count = %d, error = %d\n", i, element->word, bucket->count, element->error);
            element = element->next;
        }
        if (i > k || i > CAPACIRT) break;
        bucket = bucket->prev;
    }
    
}

/**
 * This funciton output top k' elements, exactly like what is described in paper
 * k' is almost the same as k, but will guarantee the correctness.
*/
void find_top_k_more(int k){
    int i,  min_guar_freq, count_next, diff;
    uint8_t order, guaranteed, continue_loop;
    struct Buckets *bucket;
    struct Elements *element;
    i = 0;
    order = 1;
    guaranteed = 0;
    continue_loop = 0;
    min_guar_freq = INT_MAX;

    bucket = bucket_tail;
    while(bucket != NULL){
        element = bucket->element_head;
        while(element != NULL){
            i++;
            if (i > k || i > CAPACIRT) break;
            printf("%d: %s, count = %d, error = %d\n", i, element->word, bucket->count, element->error);
            diff = bucket->count - element->error;
            if (element->next == NULL){
                if (bucket->prev== NULL){
                    count_next = 0;
                }else{
                    count_next = (bucket->prev)->count;
                }
            }else{
                count_next = bucket->count;
            }
            if (diff < min_guar_freq){
                min_guar_freq = diff;
            }
            if (diff < count_next){
                order = 0;
            }
            element = element->next;
        }
        if (i > k || i > CAPACIRT) break;
        bucket = bucket->prev;
    }
    if (bucket == NULL) return;
    continue_loop = 1;
    diff = bucket->count - element->error;
    if (bucket->count <= min_guar_freq){
        guaranteed = 1;
    }else{
        printf("%d: %s, count = %d, error = %d\n", i, element->word, bucket->count, element->error);
        while(bucket != NULL){
            if (!continue_loop){
                element = bucket->element_head;
            }
            while(element != NULL){
                i++;
                if (i > k || i > CAPACIRT) break;
                if (diff < min_guar_freq){
                    min_guar_freq = diff;
                }
                if (bucket->count <= min_guar_freq){
                    guaranteed = 1;
                    break;
                }
                printf("%d: %s, count = %d, error = %d\n", i, element->word, bucket->count, element->error);
                
                diff = bucket->count - element->error;
                element = element->next;
            }
            if (i > k || i > CAPACIRT) break;
            bucket = bucket->prev;
        }
    }
    if (guaranteed){
        printf("guaranteed is true. ");
    }else{
        printf("guaranteed is false. ");
    }
    if (order){
        printf("order is true\n");
    }else{
        printf("order is false\n");
    }
}

int main(int argc, char *argv[]) {

    int cap = CAPACIRT;
    if (argc != 3) {
        printf("Usage: %s <file_path> <k>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    int k = atoi(argv[2]);

    // char* filename = "test_files/example.txt";
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file %s\n", filename);
        exit(1);
    }
    printf("running with capacity = %d\n", cap);
    // Initialize bucket head
    bucket_head = (struct Buckets *)malloc(sizeof *bucket_head);
    bucket_head->count = 1;
    bucket_head->next = NULL;
    bucket_head->prev = NULL;
    bucket_head->element_head = NULL;

    bucket_tail = bucket_head;
    char scan_string[128];
    char word[WORD_SIZE];
    // struct Elements *s;
    while (fscanf(file, "%s", scan_string) > 0) {
        char* token = strtok(scan_string, delim);
        while (token != NULL) {
            strncpy(word, token, WORD_SIZE);
            word[WORD_SIZE - 1] = '\0';
            for (size_t i = 0; i < WORD_SIZE; i++) {
                word[i] = tolower(word[i]);
            }
            // printf("%s\n", word);
            add_element(word);
            token = strtok(NULL, delim);
        }
    }
    // HASH_FIND_STR( hash_head, "test", s);
    // if (s) printf("test's count is %d\n", (s->bucket)->count);
    // print_all_buckets_reverse();
    find_top_k(k);
    fclose(file);
    free_all();
    return 0;
}

