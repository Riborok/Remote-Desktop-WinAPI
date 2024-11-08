#pragma once

#include <vector>
#include <zlib.h>

using CryptoPP::byte;

class CompressionUtils {
public:
    CompressionUtils() = delete;
    
    static std::vector<byte> compress(const std::vector<byte>& data);
    static std::vector<byte> compress(const byte* data, const size_t dataSize);
    
    static std::vector<byte> decompress(const std::vector<byte>& compressedData);
    static std::vector<byte> decompress(const byte* compressedData, const size_t compressedDataSize);
private:
    static std::vector<byte> transform(CryptoPP::BufferedTransformation& bt, const byte* data, const size_t dataSize);
};
