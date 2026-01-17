#include <vector>
#include <cstddef>
#include "Utils.h"

// Ovo je samo ilustrativno – helper za generisanje "poruka" razlicitih velicina.

struct ClientMessage {
    size_t size;
    // mogli bi dodati i neki payload tipa std::vector<char> itd.
};

std::vector<ClientMessage> GenerateClientMessages(size_t count,
    size_t minSize,
    size_t maxSize)
{
    std::vector<ClientMessage> msgs;
    msgs.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        msgs.push_back({ RandomSize(minSize, maxSize) });
    }
    return msgs;
}