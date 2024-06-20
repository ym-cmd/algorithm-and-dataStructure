#ifndef __HASH_SET__
#define __HASH_SET__

#include <func.h>
#include <stdint.h>

typedef int K;

typedef struct node {
    K key;
    struct node* next;
} Node;

typedef struct {
    Node** buckets;
    int size;
    int capacity;
    uint32_t hashseed;
} HashSet;

HashSet* hashsetCreate(void);
void hashsetDestroy(HashSet* set);
void hashsetInsert(HashSet* set, K key);
int hashsetSearch(HashSet* set, K Key);
void hashsetDelete(HashSet* set, K key);
void hashsetClear(HashSet* set);

#endif