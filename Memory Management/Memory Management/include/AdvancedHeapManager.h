#ifndef ADVANCEDHEAPMANAGER_H
#define ADVANCEDHEAPMANAGER_H

#include <cstddef>
#include <mutex>
#include "Heap.h"
#include "DataStructures.h"

class AdvancedHeapManager {
public:
    AdvancedHeapManager(size_t heapCount, size_t heapSize);
    ~AdvancedHeapManager();

    void* Malloc(size_t size);
    void Free(void* ptr);
    void PrintHeaps() const;

private:
    Heap** heaps;
    size_t heapCount;
    size_t heapSize;
    HashMap ptrMap;
    size_t* usedMemory;

    mutable std::mutex globalLock;
    std::mutex* heapLocks;

    size_t GetLeastUsedHeap() const;
};

#endif