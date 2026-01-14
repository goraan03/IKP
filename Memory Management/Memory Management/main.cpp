#include <iostream>
#include "include/AdvancedHeapManager.h"

int main() {
    const size_t HEAP_COUNT = 3;
    const size_t HEAP_SIZE = 1024 * 1024; // 1 MB heap
    
    AdvancedHeapManager ahm(HEAP_COUNT, HEAP_SIZE);

    void* p1 = ahm.Malloc(200);
    void* p2 = ahm.Malloc(400);
    void* p3 = ahm.Malloc(1000);
    void* p4 = ahm.Malloc(450);

    std::cout << "\n--- Nakon alokacije ---";
    ahm.PrintHeaps();

    ahm.Free(p2);
    std::cout << "\n--- Nakon Free(p2) ---";
    ahm.PrintHeaps();

    return 0;
}