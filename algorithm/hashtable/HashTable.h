#ifndef __HASH_TABLE__
#define __HASH_TABlE__
#include <func.h>
#include <stdint.h>

typedef int K;
typedef int V;

typedef struct node {
    K key;
    V val;
    struct node* next;
} Node;

typedef struct {
    Node** table; // 拉链法
    int size;
    int capacity;
    uint32_t hashseed; //unsigned int
} HashMap;

HashMap* hashmapCreate(void);
void hashmapDestroy(HashMap* map);

void hashmapInsert(HashMap* map, K key, V val);
V hashmapSearch(HashMap* map, K key);
void hashmapDelete(HashMap* map, K key);

#endif
