#pragma once
#include <vector>

#include "PayloadMerger.hpp"
#include "../../../types/byte.hpp"

class PayloadSplitter {
    using Prefix = PayloadMerger::Prefix;
public:
    PayloadSplitter() = delete;
    
    static std::vector<std::vector<byte>> splitIntoChunks(const std::vector<byte>& payload);
private:
    static std::vector<byte> extractChunk(const std::vector<byte>& payload, const size_t offset, const size_t chunkSize);
};
