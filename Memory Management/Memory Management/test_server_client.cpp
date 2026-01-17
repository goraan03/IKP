#include <iostream>

void RunServerWithAHM(size_t clientCount,
    size_t messagesPerClient,
    size_t heapCount,
    size_t heapSize,
    uint64_t& outTimeMs);

void RunServerWithMalloc(size_t clientCount,
    size_t messagesPerClient,
    uint64_t& outTimeMs);

int main() {
    const size_t clientCount = 5;                  // broj "klijenata"
    const size_t messagesPerClient = 100000;             // poruka po klijentu
    const size_t heapCount = 4;
    const size_t heapSize = 64 * 1024 * 1024;   // 64 MB po heapu

    uint64_t ahmTime = 0;
    uint64_t mallocTime = 0;

    RunServerWithAHM(clientCount, messagesPerClient,
        heapCount, heapSize, ahmTime);

    RunServerWithMalloc(clientCount, messagesPerClient, mallocTime);

    std::cout << "=== Test 2: server/klijent simulacija ===\n";
    std::cout << "Klijenata: " << clientCount
        << ", poruka po klijentu: " << messagesPerClient << "\n\n";

    std::cout << "AHM    : " << ahmTime << " ms\n";
    std::cout << "malloc : " << mallocTime << " ms\n";

    return 0;
}