#pragma once

#include <vector>

template <typename T>
class ArrayChunkSplitter {
    const std::vector<T>& _data;
    size_t _chunkSize;
public:
    ArrayChunkSplitter(const std::vector<T>& data, const size_t chunkSize);
    std::tuple<const T*, size_t> getChunk(size_t chunkIndex) const;
    ~ArrayChunkSplitter() = default;
    
    ArrayChunkSplitter(ArrayChunkSplitter&&) = delete;
    ArrayChunkSplitter& operator=(ArrayChunkSplitter&&) = delete;
    ArrayChunkSplitter(const ArrayChunkSplitter&) = delete;
    ArrayChunkSplitter& operator=(const ArrayChunkSplitter&) = delete;
};

template <typename T>
ArrayChunkSplitter<T>::ArrayChunkSplitter(const std::vector<T>& data, const size_t chunkSize): _data(data), _chunkSize(chunkSize) { }

template <typename T>
std::tuple<const T*, size_t> ArrayChunkSplitter<T>::getChunk(const size_t chunkIndex) const {
    const size_t offset = chunkIndex * _chunkSize;
    const size_t chunkLength = std::min(_chunkSize, _data.size() - offset);
    return std::make_tuple(_data.data() + offset, chunkLength);
}
