﻿#pragma once

#include <vector>

#include "CompressionToolkit.hpp"
#include "../../types/byte.hpp"

class Compressor {
    using OriginalSize = CompressionToolkit::OriginalSize;
public:
    Compressor() = delete;
    
    static std::vector<byte> compress(const std::vector<byte>& data);
    static std::vector<byte> compress(const byte* data, const size_t dataSize);
private:
    static bool isDataCompressed(const int compressedSize);
    static std::vector<byte> createUncompressedData(const byte* data, const size_t dataSize);
    static std::vector<byte> createCompressedDataBuffer(const int dataSize);
    static std::vector<byte> createDataBuffer(const size_t dataSize, const int metadataValue);
    static int performCompression(const byte* data, const int dataSize, std::vector<byte>& compressedData);
};