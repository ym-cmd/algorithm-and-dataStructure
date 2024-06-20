#include "HashSet.h"

int main() {
    HashSet* set = hashsetCreate();

    hashsetInsert(set, 5);
    hashsetInsert(set, 15);
    // hashsetInsert(set, 5);
    hashsetInsert(set, 20);

    printf("hahashsetSearch 5: %d\n",hashsetSearch(set, 5)); // Output: 1
    printf("hahashsetSearch 10: %d\n",hashsetSearch(set, 10)); // Output: 0
    printf("hahashsetSearch 15: %d\n",hashsetSearch(set, 15)); // Output: 1
    printf("hahashsetSearch 20: %d\n",hashsetSearch(set, 20)); // Output: 1

    hashsetClear(set);

    printf("hahashsetSearch 5: %d\n",hashsetSearch(set, 5)); // Output: 0
    printf("hahashsetSearch 10: %d\n",hashsetSearch(set, 10)); // Output: 0
    printf("hahashsetSearch 15: %d\n",hashsetSearch(set, 15)); // Output: 0
    printf("hahashsetSearch 20: %d\n",hashsetSearch(set, 20)); // Output: 0

    hashsetInsert(set, 5);

    printf("hahashsetSearch 5: %d\n",hashsetSearch(set, 5)); // Output: 1
    printf("hahashsetSearch 10: %d\n",hashsetSearch(set, 10)); // Output: 0
    printf("hahashsetSearch 15: %d\n",hashsetSearch(set, 15)); // Output: 0
    printf("hahashsetSearch 20: %d\n",hashsetSearch(set, 20)); // Output: 0

    hashsetDelete(set, 5);
    hashsetInsert(set, 20);

    printf("hahashsetSearch 5: %d\n",hashsetSearch(set, 5)); // Output: 0
    printf("hahashsetSearch 10: %d\n",hashsetSearch(set, 10)); // Output: 0
    printf("hahashsetSearch 15: %d\n",hashsetSearch(set, 15)); // Output: 0
    printf("hahashsetSearch 20: %d\n",hashsetSearch(set, 20)); // Output: 1

    hashsetDestroy(set);
    // // 段错误
    // printf("hahashsetSearch 5: %d\n",hashsetSearch(set, 5)); // Output: 0
    // printf("hahashsetSearch 10: %d\n",hashsetSearch(set, 10)); // Output: 0
    // printf("hahashsetSearch 15: %d\n",hashsetSearch(set, 15)); // Output: 1
    // printf("hahashsetSearch 20: %d\n",hashsetSearch(set, 20)); // Output: 1

    return 0;
}