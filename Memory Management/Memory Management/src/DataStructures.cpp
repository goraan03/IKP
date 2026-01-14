#include "../include/DataStructures.h"

HashMap::HashMap(size_t capacity)
    : capacity(capacity)
{
    table = new HashEntry[capacity];
}

HashMap::~HashMap() {
    delete[] table;
}

size_t HashMap::Hash(void* key) const {
    return (reinterpret_cast<size_t>(key) >> 3) % capacity;
}

bool HashMap::Insert(void* key, HeapAllocInfo value) {
    size_t idx = Hash(key);
    for (size_t i = 0; i < capacity; i++) {
        size_t probe = (idx + i) % capacity;
        if (!table[probe].occupied) {
            table[probe].key = key;
            table[probe].value = value;
            table[probe].occupied = true;
            return true;
        }
    }
    return false;
}

bool HashMap::Remove(void* key) {
    size_t idx = Hash(key);
    for (size_t i = 0; i < capacity; i++) {
        size_t probe = (idx + i) % capacity;
        if (table[probe].occupied && table[probe].key == key) {
            table[probe].occupied = false;
            table[probe].key = nullptr;
            return true;
        }
    }
    return false;
}

bool HashMap::Get(void* key, HeapAllocInfo& outValue) const {
    size_t idx = Hash(key);
    for (size_t i = 0; i < capacity; i++) {
        size_t probe = (idx + i) % capacity;
        if (!table[probe].occupied) continue;
        if (table[probe].key == key) {
            outValue = table[probe].value;
            return true;
        }
    }
    return false;
}