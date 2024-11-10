#pragma once

#include <config_int.h>
#include <vector>

using CryptoPP::byte;

class CompressionUtils {
public:
    CompressionUtils() = delete;

    static constexpr size_t METADATA_SIZE = sizeof(size_t);
    
    static std::vector<byte> compress(const std::vector<byte>& data);
    static std::vector<byte> compress(const byte* data, const size_t dataSize);
    
    static std::vector<byte> decompress(const std::vector<byte>& compressedData);
    static std::vector<byte> decompress(const byte* compressedData, const size_t compressedDataSize);
private:
    static std::vector<byte> initializeCompressedBuffer(const size_t dataSize);
    static int performCompression(const byte* data, const size_t dataSize, std::vector<byte>& compressedData);
    static std::vector<byte> initializeDecompressedBuffer(const size_t originalSize);
    static void performDecompression(const byte* compressedData, const size_t compressedDataSize, std::vector<byte>& decompressedData);
};
