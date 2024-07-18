#include "HashTable.h" // unordered_map
#include <func.h>

int main() {
    HashMap* map = hashmapCreate();

    hashmapPut(map, 12, 3);
    hashmapPut(map, 2, 4);
    hashmapPut(map, 3, 3);
    hashmapPut(map, 7, 1);
    hashmapPut(map, 12, 8);
    hashmapPut(map, 7, 6);

    printf("hashmapGet(map, 12) = %d\n",hashmapGet(map, 12));
    printf("hashmapGet(map, 2) = %d\n",hashmapGet(map, 2));
    printf("hashmapGet(map, 3) = %d\n",hashmapGet(map, 3));
    printf("hashmapGet(map, 7) = %d\n",hashmapGet(map, 7));

    hashmapDelete(map, 12);
    hashmapDelete(map, 7);

    printf("hashmapGet(map, 12) = %d\n",hashmapGet(map, 12));
    printf("hashmapGet(map, 2) = %d\n",hashmapGet(map, 2));
    printf("hashmapGet(map, 3) = %d\n",hashmapGet(map, 3));
    printf("hashmapGet(map, 7) = %d\n",hashmapGet(map, 7));

    hashmapDestroy(map);
    return 0;
}