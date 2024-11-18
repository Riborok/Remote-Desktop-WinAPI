﻿#pragma once

#include <vector>

#include "CompressionToolkit.hpp"
#include "../../types/byte.hpp"

class Decompressor {
    using OriginalSize = CompressionToolkit::OriginalSize;
public:
    Decompressor() = delete;

    static std::vector<byte> decompress(const std::vector<byte>& compressedData);
    static std::vector<byte> decompress(const byte* compressedData, const size_t compressedDataSize);
private:
    static bool isCompressedData(const int originalSize);
    static std::vector<byte> extractUncompressedData(const byte* compressedData, const size_t compressedDataSize);
    static std::vector<byte> createDecompressedDataBuffer(const int originalSize);
    static void performDecompression(const byte* compressedData, const int compressedDataSize, std::vector<byte>& decompressedData);
};