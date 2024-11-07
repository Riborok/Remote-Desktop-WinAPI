#include "../../inc/utils/CompressionUtils.hpp"

std::vector<byte> CompressionUtils::compress(const std::vector<byte>& data) {
    CryptoPP::ZlibCompressor compressor;
    return transform(compressor, data);
}

std::vector<byte> CompressionUtils::decompress(const std::vector<byte>& compressedData) {
    CryptoPP::ZlibDecompressor decompressor;
    return transform(decompressor, compressedData);
}

std::vector<byte> CompressionUtils::transform(CryptoPP::BufferedTransformation& bt, const std::vector<byte>& data) {
    std::vector<byte> result;
    bt.Put(data.data(), data.size());
    bt.MessageEnd();
    result.resize(bt.MaxRetrievable());
    bt.Get(result.data(), result.size());
    return result;
}
