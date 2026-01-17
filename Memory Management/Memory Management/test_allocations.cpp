#include <thread>
#include <vector>
#include <atomic>
#include <iostream>
#include <cstdlib>
#include "Utils.h"
#include "include/AdvancedHeapManager.h"

// Parametri za radnu nit koja koristi AHM
struct ThreadParams {
    AdvancedHeapManager* ahm;
    size_t totalBytesToProcess;
    std::atomic<size_t>* globalCounter;
};

// Worker za AHM
void WorkerAHM(ThreadParams params) {
    size_t processed = 0;
    const size_t MIN_SIZE = 16;
    const size_t MAX_SIZE = 4096;

    while (processed < params.totalBytesToProcess) {
        size_t sz = RandomSize(MIN_SIZE, MAX_SIZE);
        void* p = params.ahm->Malloc(sz);
        if (!p) {
            continue;
        }
        // simulacija rada sa memorijom...
        params.ahm->Free(p);

        processed += sz;
        if (params.globalCounter)
            (*params.globalCounter) += sz;
    }
}

// Pokretanje testa za AHM
void RunTestAHM(size_t threadCount) {
    const size_t HEAP_COUNT = 4;
    const size_t HEAP_SIZE = 64 * 1024 * 1024;           // 64 MB po heapu
    const size_t TOTAL_BYTES = 4ull * 1024 * 1024 * 1024;  // 4 GB ukupno

    AdvancedHeapManager ahm(HEAP_COUNT, HEAP_SIZE);

    size_t bytesPerThread = TOTAL_BYTES / threadCount;

    std::vector<std::thread> threads;
    threads.reserve(threadCount);
    std::atomic<size_t> globalCounter(0);

    uint64_t t0 = GetTimeMs();

    for (size_t i = 0; i < threadCount; ++i) {
        ThreadParams params{ &ahm, bytesPerThread, &globalCounter };
        threads.emplace_back(WorkerAHM, params);
    }

    for (auto& t : threads)
        if (t.joinable()) t.join();

    uint64_t t1 = GetTimeMs();
    std::cout << "[AHM]    Threads: " << threadCount
        << "  time: " << (t1 - t0) << " ms, processed: "
        << globalCounter.load() / (1024.0 * 1024 * 1024.0) << " GB\n";
}

// Worker koji koristi malloc/free
void WorkerMalloc(size_t totalBytesToProcess, std::atomic<size_t>* globalCounter) {
    size_t processed = 0;
    const size_t MIN_SIZE = 16;
    const size_t MAX_SIZE = 4096;

    while (processed < totalBytesToProcess) {
        size_t sz = RandomSize(MIN_SIZE, MAX_SIZE);
        void* p = std::malloc(sz);
        if (!p) continue;
        // simulacija rada...
        std::free(p);

        processed += sz;
        if (globalCounter)
            (*globalCounter) += sz;
    }
}

// Pokretanje testa sa malloc/free
void RunTestMalloc(size_t threadCount) {
    const size_t TOTAL_BYTES = 4ull * 1024 * 1024 * 1024;

    size_t bytesPerThread = TOTAL_BYTES / threadCount;
    std::vector<std::thread> threads;
    threads.reserve(threadCount);
    std::atomic<size_t> globalCounter(0);

    uint64_t t0 = GetTimeMs();

    for (size_t i = 0; i < threadCount; ++i) {
        threads.emplace_back(WorkerMalloc, bytesPerThread, &globalCounter);
    }

    for (auto& t : threads)
        if (t.joinable()) t.join();

    uint64_t t1 = GetTimeMs();
    std::cout << "[malloc] Threads: " << threadCount
        << "  time: " << (t1 - t0) << " ms, processed: "
        << globalCounter.load() / (1024.0 * 1024 * 1024.0) << " GB\n";
}

int main() {
    std::vector<size_t> threadCounts = { 1, 2, 5, 10, 50 };

    std::cout << "=== Test 1: sinteticke alokacije 4 GB ===\n\n";

    for (size_t tc : threadCounts)
        RunTestAHM(tc);

    std::cout << "-----------------------------------------\n";

    for (size_t tc : threadCounts)
        RunTestMalloc(tc);

    return 0;
}