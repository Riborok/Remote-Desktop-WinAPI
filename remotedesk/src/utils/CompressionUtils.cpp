#include "../../inc/utils/CompressionUtils.hpp"

std::vector<byte> CompressionUtils::compress(const std::vector<byte>& data) {
    return compress(data.data(), data.size());
}

std::vector<byte> CompressionUtils::compress(const byte* data, const size_t dataSize) {
    CryptoPP::ZlibCompressor compressor;
    return transform(compressor, data, dataSize);
}

std::vector<byte> CompressionUtils::decompress(const std::vector<byte>& compressedData) {
    return decompress(compressedData.data(), compressedData.size());
}

std::vector<byte> CompressionUtils::decompress(const byte* compressedData, const size_t compressedDataSize) {
    CryptoPP::ZlibDecompressor decompressor;
    return transform(decompressor, compressedData, compressedDataSize);
}

std::vector<byte> CompressionUtils::transform(CryptoPP::BufferedTransformation& bt, const byte* data, const size_t dataSize) {
    std::vector<byte> result;
    bt.Put(data, dataSize);
    bt.MessageEnd();
    result.resize(bt.MaxRetrievable());
    bt.Get(result.data(), result.size());
    return result;
}
