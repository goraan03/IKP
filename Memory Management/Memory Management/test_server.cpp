#include <thread>
#include <vector>
#include <cstdlib>
#include "include/AdvancedHeapManager.h"
#include "Utils.h"

// Worker koji simulira server-side obradu poruka koristeci AHM
void ServerWorkerAHM(AdvancedHeapManager* ahm, size_t messagesPerClient) {
    for (size_t i = 0; i < messagesPerClient; ++i) {
        size_t sz = RandomSize(64, 4096);
        void* p = ahm->Malloc(sz);
        if (!p) {
            continue;
        }
        // simulacija obrade... (npr. upisivanje u bafer itd.)

        ahm->Free(p);
    }
}

// Ista simulacija ali sa malloc/free
void ServerWorkerMalloc(size_t messagesPerClient) {
    for (size_t i = 0; i < messagesPerClient; ++i) {
        size_t sz = RandomSize(64, 4096);
        void* p = std::malloc(sz);
        if (!p) continue;
        // simulacija obrade...

        std::free(p);
    }
}

// Ove funkcije ce pozivati tests/test_server_client.cpp

void RunServerWithAHM(size_t clientCount,
    size_t messagesPerClient,
    size_t heapCount,
    size_t heapSize,
    uint64_t& outTimeMs)
{
    AdvancedHeapManager ahm(heapCount, heapSize);
    std::vector<std::thread> clients;
    clients.reserve(clientCount);

    uint64_t t0 = GetTimeMs();

    for (size_t i = 0; i < clientCount; ++i) {
        clients.emplace_back(ServerWorkerAHM, &ahm, messagesPerClient);
    }

    for (auto& t : clients) {
        if (t.joinable()) t.join();
    }

    uint64_t t1 = GetTimeMs();
    outTimeMs = t1 - t0;
}

void RunServerWithMalloc(size_t clientCount,
    size_t messagesPerClient,
    uint64_t& outTimeMs)
{
    std::vector<std::thread> clients;
    clients.reserve(clientCount);

    uint64_t t0 = GetTimeMs();

    for (size_t i = 0; i < clientCount; ++i) {
        clients.emplace_back(ServerWorkerMalloc, messagesPerClient);
    }

    for (auto& t : clients) {
        if (t.joinable()) t.join();
    }

    uint64_t t1 = GetTimeMs();
    outTimeMs = t1 - t0;
}