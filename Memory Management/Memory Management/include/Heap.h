#ifndef HEAP_H
#define HEAP_H

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include "MemoryBlock.h"

class Heap {
public:
	Heap(size_t heapSize);
	~Heap();

	void* Alloc(size_t size);
	void Free(void* ptr);

	void PrintState() const;

private:
	void* heapMemory;
	size_t totalSize;
	MemoryBlock* blockList;
};

#endif