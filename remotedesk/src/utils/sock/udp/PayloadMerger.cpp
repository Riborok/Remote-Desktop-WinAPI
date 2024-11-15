#include "../../../../inc/utils/sock/udp/PayloadMerger.hpp"

#include "../../../../inc/utils/array/ByteArrayUtils.hpp"

PayloadMerger::Buffer::Buffer(const size_t payloadSize): _payloadSize(payloadSize) {
    _buffer.reserve(payloadSize);
}

bool PayloadMerger::Buffer::isFull(const size_t chunkSize) const {
    return _buffer.size() + PREFIX_SIZE + chunkSize > _payloadSize;
}

void PayloadMerger::Buffer::appendWithPrefix(const std::vector<byte>& chunk) {
    const size_t currentBufferSize = _buffer.size();
    _buffer.resize(currentBufferSize + PREFIX_SIZE + chunk.size());
    ByteArrayUtils::setValueByByteIndex<Prefix>(_buffer, currentBufferSize, chunk.size());
    std::memcpy(&_buffer[currentBufferSize + PREFIX_SIZE], chunk.data(), chunk.size());
}

void PayloadMerger::Buffer::reset() {
    _buffer.clear();
    _buffer.reserve(_payloadSize);
}

std::vector<byte>& PayloadMerger::Buffer::getBuffer() {
    return _buffer;
}

bool PayloadMerger::Buffer::isEmpty() const {
    return _buffer.empty();
}

PayloadMerger::MergedChunks::MergedChunks(const size_t capacity) {
    payloads.reserve(capacity);
    chunkCounts.reserve(capacity);
}

void PayloadMerger::MergedChunks::addPayload(std::vector<byte>&& buffer, const size_t chunkCount) {
    payloads.emplace_back(std::move(buffer));
    chunkCounts.push_back(chunkCount);
}

PayloadMerger::MergedChunks PayloadMerger::mergeIntoPayloads(const std::vector<std::vector<byte>>& chunks,
    const size_t payloadSize) {
    MergedChunks mergedChunks(chunks.size());
    Buffer buffer(payloadSize);
    size_t chunkCount = 0;
    for (const auto& chunk : chunks) {
        if (buffer.isFull(chunk.size())) {
            mergedChunks.addPayload(std::move(buffer.getBuffer()), chunkCount);
            reset(buffer, chunkCount);
        }
        buffer.appendWithPrefix(chunk);
        ++chunkCount;
    }
    if (!buffer.isEmpty()) {
        mergedChunks.addPayload(std::move(buffer.getBuffer()), chunkCount);
    }
    return mergedChunks;
}

void PayloadMerger::reset(Buffer& buffer, size_t& chunkCount) {
    buffer.reset();
    chunkCount = 0;
}
