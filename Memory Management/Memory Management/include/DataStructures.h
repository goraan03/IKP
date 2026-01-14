#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <cstddef>

struct HeapAllocInfo {
    size_t heapIndex;
    size_t blockSize;
};

struct HashEntry {
    void* key;
    HeapAllocInfo value;
    bool occupied;

    HashEntry() : key(nullptr), occupied(false) {}
};

class HashMap {
public:
    HashMap(size_t capacity = 1024);
    ~HashMap();

    bool Insert(void* key, HeapAllocInfo value);
    bool Remove(void* key);
    bool Get(void* key, HeapAllocInfo& outValue) const;

private:
    HashEntry* table;
    size_t capacity;
    size_t Hash(void* key) const;
};

#endif