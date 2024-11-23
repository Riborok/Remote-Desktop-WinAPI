#pragma once

#include <config_int.h>
#include <vector>

struct Chunk {
    size_t chunkId;
    std::vector<CryptoPP::byte> data;

    Chunk() = default;
    Chunk(const size_t chunkId, std::vector<CryptoPP::byte>&& data):
        chunkId(chunkId), data(std::move(data)) { }
};
