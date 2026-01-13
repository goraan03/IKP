#include <iostream>
#include "include/Heap.h"

int main() {
    const size_t HEAP_SIZE = 1024 * 1024; // 1 MB heap
    Heap heap(HEAP_SIZE);

    std::cout << "=== Pocetno stanje ===";
    heap.PrintState();

    void* a = heap.Alloc(256);
    void* b = heap.Alloc(128);
    void* c = heap.Alloc(512);

    std::cout << "\n=== Posle alokacija ===";
    heap.PrintState();

    heap.Free(b);
    std::cout << "\n=== Nakon oslobadjanja bloka b ===";
    heap.PrintState();

    void* d = heap.Alloc(100);
    std::cout << "\n=== Nakon nove alokacije d ===";
    heap.PrintState();

    return 0;
}