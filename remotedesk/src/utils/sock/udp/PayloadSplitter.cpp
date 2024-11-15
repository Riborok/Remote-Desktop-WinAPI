#include "../../../../inc/utils/sock/udp/PayloadSplitter.hpp"

#include "../../../../inc/utils/array/ByteArrayUtils.hpp"

std::vector<std::vector<byte>> PayloadSplitter::splitIntoChunks(const std::vector<byte>& payload) {
    std::vector<std::vector<byte>> chunks;
    size_t offset = 0;
    while (offset < payload.size()) {
        const size_t chunkSize = ByteArrayUtils::getValueByByteIndex<Prefix>(payload, offset);
        offset += PayloadMerger::PREFIX_SIZE;
        chunks.emplace_back(extractChunk(payload, offset, chunkSize));
        offset += chunkSize;
    }
    return chunks;
}

std::vector<byte> PayloadSplitter::extractChunk(const std::vector<byte>& payload, const size_t offset,
    const size_t chunkSize) {
    std::vector<byte> chunk(chunkSize);
    std::memcpy(chunk.data(), &payload[offset], chunkSize);
    return chunk;
}
