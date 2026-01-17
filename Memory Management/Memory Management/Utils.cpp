#include "Utils.h"
#include <chrono>
#include <random>

uint64_t GetTimeMs() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

size_t RandomSize(size_t minSize, size_t maxSize) {
    static thread_local std::mt19937_64 gen(std::random_device{}());
    std::uniform_int_distribution<size_t> dist(minSize, maxSize);
    return dist(gen);
}