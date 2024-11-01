#pragma once

#include <vector>
#include <zlib.h>

using CryptoPP::byte;

class CompressionUtils {
public:
    static std::vector<byte> compress(const std::vector<byte>& data);
    static std::vector<byte> decompress(const std::vector<byte>& compressedData);
private:
    static std::vector<byte> transform(CryptoPP::BufferedTransformation& bt, const std::vector<byte>& data);
};
