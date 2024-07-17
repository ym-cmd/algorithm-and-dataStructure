#include"BitMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BITS_PER_WORD 32
#define BITMAP_SHIFT   5 
#define BITMAP_MASK 0x1F
//存储bits，需要多少个word
#define BITMAP_SIZE(bits) ((bits + BITS_PER_WORD - 1) >> BITMAP_SHIFT)
BitMap* bitmap_create(size_t bits) {
	BitMap* bm = malloc(sizeof(BitMap));
	bm->array = (Word*)calloc(BITMAP_SIZE(bits), sizeof(Word));
	bm->bits = bits;
	return bm;
}
void bitmap_destroy(BitMap* bm) {
	free(bm->array);
	free(bm);
}
// 扩充位图，至少能够存下bits位
void grow_capacity(BitMap* bm, size_t bits) {
	// 位图：内存紧凑的数据结构；
	// 扩容策略：需要多大，就申请多大的内存空间
	uint32_t* new_array = realloc(bm->array, BITMAP_SIZE(bits) * sizeof(Word));
	if (!new_array) {
		printf("Error: realloc failed in grow_capacity\n");
		exit(1);
	}
	bm->array = new_array;
	// 将扩容的部分置为0
	int bytes = (BITMAP_SIZE(bits) - BITMAP_SIZE(bm->bits)) * sizeof(Word);
	memset(bm->array + BITMAP_SIZE(bm->bits), 0, bytes);
}
//置1
void bitmap_set(BitMap* bm, size_t n) {// n is a bit index,set nth 1
	if (n >= bm->bits) {
		if (BITMAP_SIZE(n + 1) > BITMAP_SIZE(bm->bits)) {//看看字够不够放
			grow_capacity(bm, n + 1);
		}
		bm->bits = n + 1;//不用扩
	}
	//set nth
	size_t word = n >> BITMAP_SHIFT;//÷32
	size_t offset = n & BITMAP_MASK;
	bm->array[word] |= (0x1 << offset);
}
//置0
void bitmap_unset(BitMap* bm, size_t n) {
	if (n >= bm->bits) {
		return;
	}
	size_t word = n >> BITMAP_SHIFT;//÷32
	size_t offset = n & BITMAP_MASK;
	bm->array[word] &= ~(0x1 << offset);

}
//判断是否为0
bool bitmap_isset(BitMap* bm, size_t n) {
	if (n >= bm->bits) {
		return false;
	}
	size_t word = n >> BITMAP_SHIFT;
	size_t offset = n & BITMAP_MASK;
	return bm->array[word] & (0x1 << offset);
}
void bitmap_clear(BitMap* bm) {
	size_t bytes = BITMAP_SIZE(bm->bits) * sizeof(Word);
	memset(bm->array, 0, bytes);
}