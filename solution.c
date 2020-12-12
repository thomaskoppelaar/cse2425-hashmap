#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define NEW_HASH
typedef struct Node {
    void* data;
    struct Node* next;
    char key [];
    
} Node;

typedef struct HashMap {
    size_t key_space;
    unsigned int (*hash_function)(char[]);
    Node* buckets[];

} HashMap;


unsigned int hash(char key[]) {
    if (key == NULL) return 0;

    size_t sum = 0;
    size_t len = strlen(key);
    for (size_t i = 0; i < len; i++) {
        sum += (size_t) *(key+i);
    }
    return sum;
}

struct HashMap* create_hashmap(size_t key_space) {
    if (key_space < 1) return NULL;

    HashMap* hm = malloc(sizeof(HashMap) + sizeof(Node)*key_space);
    if (hm == NULL) return NULL;
    
    hm->key_space = key_space;
    hm->hash_function = &hash;
    
    for (size_t i = 0; i < key_space; i++) {
        hm->buckets[i] = NULL;
    }

    return (struct HashMap*) hm;
}






void insert_data(HashMap* hm, char key[], void* data, void* resolve_collision(void* old_data, void* new_data)) {
    if (hm == NULL || key == NULL) return;

    Node* newNode = malloc(sizeof(Node) + sizeof(char[strlen(key)+1]));

    if (newNode == NULL) return;
    strcpy(newNode->key, key);
    newNode->data = data;
    newNode->next = NULL;

    size_t bucket_to_use = hm->hash_function(key) % hm->key_space;

    Node* node = hm->buckets[bucket_to_use];
    // base case: bucket is empty.
    if (node == NULL) {
        
        hm->buckets[bucket_to_use] = newNode;
        return;

    
    }
    if (strcmp(key, node->key) == 0) {
        hm->buckets[bucket_to_use]->data = resolve_collision(node->data, data);
        free(newNode);
        return;
    }
    else {
        
        Node* curr = node;
        while (curr->next != NULL) {
            // key collision found
            if (strcmp(key, curr->key) == 0) {
                curr->data = resolve_collision(curr->data, data);
                free(newNode);
                return;
            }

            // else, continue to look
            curr = curr->next;
        }

        newNode->next = hm->buckets[bucket_to_use];
        hm->buckets[bucket_to_use] = newNode;
        return;
    }

}

void* get_data(HashMap* hm, char key[]) {
    if (hm == NULL || key == NULL) return NULL;

     // Check which bucket to use
    size_t bucket_to_use = hm->hash_function(key) % hm->key_space;

    Node* node = hm->buckets[bucket_to_use];
    // Base case: bucket is empty
    if (node == NULL) return NULL;

    if (strcmp(node->key, key) == 0) {
        return node->data;
    }

    else {

        // iterative case: loop through the list, check keys along the way
        while (node->next != NULL) {
            if (strcmp(key, node->key) == 0) {
                return node->data;
            }
            node = node->next;
        }

        if (strcmp(key, node->key) == 0) return node->data;
        // final case: if key isnt found, it isnt in the list.
        return NULL;
    }
}

void iterate(HashMap* hm, void callback(char key[], void* data)) {
    if (hm == NULL || callback == NULL) return;

    Node* curr;
    for (size_t i = 0; i < hm->key_space; i++) {
        curr = hm->buckets[i];
        while (curr != NULL) {
            callback(curr->key, curr->data);
            curr = curr->next;
        }
    }
    
}

void remove_data(HashMap *hm, char key[], void destroy_data(void* data)) {
    if (hm == NULL || key == NULL) return;
    
    size_t bucket_to_use = hm->hash_function(key) % hm->key_space;
    Node* node = hm->buckets[bucket_to_use];

    // base case: bucket is empty, no data to remove
    if (node == NULL) return;
    // base case: key exists.
    if (strcmp(key, node->key) == 0) {
        // if the callback exists, call it
        if (destroy_data) {
            destroy_data(node->data);
        }
        
        Node* newStart = node->next;
        free(hm->buckets[bucket_to_use]);
        hm->buckets[bucket_to_use] = newStart;
        


        
    // iterative case: loop until we find the key or until we reach the end of the list.
    } 
    else {
        Node* prev = node;
        Node* curr = node->next;
        while (curr != NULL) {
            // key found
            if (strcmp(key, curr->key) == 0) {
                if (destroy_data) {
                    destroy_data(curr->data);
                }
                prev->next = curr->next;
                free(curr);
                return;
                
            }

            // else, continue to look
            prev = curr;
            curr = curr->next;
        }

        // end of the loop: key doesn't exist, nothing happens
    }
}

void delete_hashmap(HashMap* hm, void destroy_data(void* data)) {
    if (hm == NULL) return;
    Node* curr;

    if (destroy_data) {
        for (size_t i = 0; i < hm->key_space; i++) {
            curr = hm->buckets[i];
            while (curr != NULL) {
                Node* next = curr->next;
                destroy_data(curr->data);
                free(curr);
                curr = next;
            }
        }
    } else {
        for (size_t i = 0; i < hm->key_space; i++) {
            curr = hm->buckets[i];
            while (curr != NULL) {
                Node* next = curr->next;
                free(curr);
                curr = next;
            }
        }
    }

    free(hm);


    
}


void set_hash_function(HashMap* hm, unsigned int (*hash_func)(char key[])) {

    if (hm == NULL || hash_func == NULL || hash_func == hash) return;

    hm->hash_function = hash_func;

    if (hm->key_space == 1) return;
    

    // Rehash data where needed.
    Node* curr;
    for (size_t i = 0; i < hm->key_space; i++) {
        curr = hm->buckets[i];
        while (curr != NULL) {
            if (hm->hash_function(curr->key) != i) {
                insert_data(hm, curr->key, curr->data, NULL);
                Node* next = curr->next;
                free(curr);
                curr = next;
                continue;
            }
            curr = curr->next;
        }
    }

}