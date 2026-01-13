#ifndef MEMORYBLOCK_H
#define MEMORYBLOCK_H

#include <cstddef>

struct MemoryBlock {
	void* start;
	size_t size;
	bool free;
	MemoryBlock* next;

	MemoryBlock(void* s = nullptr, size_t sz = 0, bool isFree = true)
		: start(s), size(sz), free(isFree), next(nullptr) {}
};

#endif