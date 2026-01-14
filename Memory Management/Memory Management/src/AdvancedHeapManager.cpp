#include "../include/AdvancedHeapManager.h"
#include <iostream>
#include <mutex>

AdvancedHeapManager::AdvancedHeapManager(size_t heapCount, size_t heapSize)
    : heapCount(heapCount), heapSize(heapSize), ptrMap(heapCount * 1024)
{
    heaps = new Heap * [heapCount];
    usedMemory = new size_t[heapCount];
    heapLocks = new std::mutex[heapCount];

    for (size_t i = 0; i < heapCount; i++) {
        heaps[i] = new Heap(heapSize);
        usedMemory[i] = 0;
    }
}

AdvancedHeapManager::~AdvancedHeapManager() {
    for (size_t i = 0; i < heapCount; i++)
        delete heaps[i];
    delete[] heaps;
    delete[] usedMemory;
    delete[] heapLocks;
}

size_t AdvancedHeapManager::GetLeastUsedHeap() const {
    size_t minIndex = 0;
    for (size_t i = 1; i < heapCount; i++) {
        if (usedMemory[i] < usedMemory[minIndex])
            minIndex = i;
    }
    return minIndex;
}

void* AdvancedHeapManager::Malloc(size_t size) {
    size_t idx;

    {   
        std::lock_guard<std::mutex> guard(globalLock);
        idx = GetLeastUsedHeap();
    }

    {
        std::lock_guard<std::mutex> heapGuard(heapLocks[idx]);
        void* ptr = heaps[idx]->Alloc(size);
        if (ptr) {
            std::lock_guard<std::mutex> globalGuard(globalLock);
            HeapAllocInfo info = { idx, size };
            ptrMap.Insert(ptr, info);
            usedMemory[idx] += size;
            return ptr;
        }
        else {
            std::cerr << "[AHM] Nema slobodne memorije u heapu: " << idx << "\n";
            return nullptr;
        }
    }
}

void AdvancedHeapManager::Free(void* ptr) {
    HeapAllocInfo info;
    {
        std::lock_guard<std::mutex> guard(globalLock);
        if (!ptrMap.Get(ptr, info)) {
            std::cerr << "[AHM] Free error: pokazivac nije pronadjen!\n";
            return;
        }
    }

    {   
        std::lock_guard<std::mutex> heapGuard(heapLocks[info.heapIndex]);
        heaps[info.heapIndex]->Free(ptr);
    }

    {   
        std::lock_guard<std::mutex> guard(globalLock);
        ptrMap.Remove(ptr);
        if (usedMemory[info.heapIndex] >= info.blockSize)
            usedMemory[info.heapIndex] -= info.blockSize;
        else
            usedMemory[info.heapIndex] = 0;
    }
}

void AdvancedHeapManager::PrintHeaps() const {
    std::lock_guard<std::mutex> guard(globalLock);
    std::cout << "\n===== AdvancedHeapManager State =====\n";
    for (size_t i = 0; i < heapCount; i++) {
        std::cout << "Heap[" << i << "] zauzeto: " << usedMemory[i] << " bajtova\n";
        heaps[i]->PrintState();
    }
}