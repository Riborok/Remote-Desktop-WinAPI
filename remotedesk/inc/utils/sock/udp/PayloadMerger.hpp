#pragma once

#include <vector>

#include "../../../types/byte.hpp"

class PayloadMerger {
    class Buffer {
        size_t _payloadSize;
        std::vector<byte> _buffer;
    public:
        explicit Buffer(const size_t payloadSize);
        bool isFull(const size_t chunkSize) const;
        void appendWithPrefix(const std::vector<byte>& chunk);
        void reset();
        std::vector<byte>& getBuffer();
        bool isEmpty() const;
    };
public:
    PayloadMerger() = delete;
    
    using Prefix = size_t;
    static constexpr size_t PREFIX_SIZE = sizeof(Prefix);

    struct MergedChunks {
        std::vector<std::vector<byte>> payloads;
        std::vector<size_t> chunkCounts;
        explicit MergedChunks(const size_t capacity);
        void addPayload(std::vector<byte>&& buffer, const size_t chunkCount);
    };
    static MergedChunks mergeIntoPayloads(const std::vector<std::vector<byte>>& chunks, const size_t payloadSize);
private:
    static void reset(Buffer& buffer, size_t& chunkCount);
};
