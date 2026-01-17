#ifndef UTILS_H
#define UTILS_H

#include <cstddef>
#include <cstdint> // Ensure uint64_t is defined

// Returns current time in milliseconds (high_resolution_clock)
uint64_t GetTimeMs();

// Returns a random size between minSize and maxSize
size_t RandomSize(size_t minSize, size_t maxSize);

#endif