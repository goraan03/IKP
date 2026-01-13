#include "../include/Heap.h"

Heap::Heap(size_t heapSize) {
    heapMemory = std::malloc(heapSize);
    totalSize = heapSize;
    blockList = new MemoryBlock(heapMemory, heapSize, true);
}

Heap::~Heap() {
    // brisemo listu blokova
    MemoryBlock* current = blockList;
    while (current) {
        MemoryBlock* next = current->next;
        delete current;
        current = next;
    }
    std::free(heapMemory);
}

void* Heap::Alloc(size_t size) {
    MemoryBlock* current = blockList;

    while (current) {
        if (current->free && current->size >= size) {
            // ako je blok veci od potrebnog, seci ga
            if (current->size > size + sizeof(MemoryBlock)) {
                void* newBlockStart = static_cast<char*>(current->start) + size;
                size_t remainingSize = current->size - size;

                // napravi novi slobodan blok
                MemoryBlock* newBlock = new MemoryBlock(newBlockStart, remainingSize, true);
                newBlock->next = current->next;
                current->next = newBlock;
                current->size = size;
            }

            current->free = false;
            return current->start;
        }
        current = current->next;
    }

    std::cerr << "Heap::Alloc error: nema slobodnog bloka od " << size << " bajtova.\n";
    return nullptr;
}

void Heap::Free(void* ptr) {
    MemoryBlock* current = blockList;

    while (current) {
        if (current->start == ptr) {
            current->free = true;

            // pokusaj spajanja sa sledecim slobodnim blokom
            if (current->next && current->next->free) {
                current->size += current->next->size;
                MemoryBlock* toDelete = current->next;
                current->next = current->next->next;
                delete toDelete;
            }

            return;
        }
        current = current->next;
    }

    std::cerr << "Heap::Free error: pokazivac nije pronadjen u ovom heapu.\n";
}

void Heap::PrintState() const {
    const MemoryBlock* current = blockList;
    std::cout << "\nHeap State:\n";
    while (current) {
        std::cout << "  Block @" << current->start
            << " size=" << current->size
            << " free=" << (current->free ? "true" : "false") << '\n';
        current = current->next;
    }
}