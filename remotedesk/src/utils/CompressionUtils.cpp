// ReSharper disable CppClangTidyBugproneNarrowingConversions CppClangTidyClangDiagnosticShorten64To32
#include "../../inc/utils/CompressionUtils.hpp"

#include <lz4.h>

#include "../../inc/utils/ByteArrayUtils.hpp"

std::vector<byte> CompressionUtils::compress(const std::vector<byte>& data) {
    return compress(data.data(), data.size());
}

std::vector<byte> CompressionUtils::compress(const byte* data, const size_t dataSize) {
    std::vector<byte> compressedData = initializeCompressedBuffer(dataSize);
    const int compressedSize = performCompression(data, dataSize, compressedData);
    compressedData.resize(compressedSize + sizeof(size_t)); 
    return compressedData;
}

std::vector<byte> CompressionUtils::initializeCompressedBuffer(const size_t dataSize) {
    const int maxCompressedSize = LZ4_compressBound(dataSize);
    std::vector<byte> buffer(maxCompressedSize + sizeof(size_t));
    ByteArrayUtils::setValue(buffer, 0, dataSize);
    return buffer;
}

int CompressionUtils::performCompression(const byte* data, const size_t dataSize, std::vector<byte>& compressedData) {
    const int maxCompressedSize = compressedData.size() - sizeof(size_t);
    return LZ4_compress_default(
        reinterpret_cast<const char*>(data),
        reinterpret_cast<char*>(compressedData.data() + sizeof(size_t)),
        dataSize,
        maxCompressedSize
    );
}

std::vector<byte> CompressionUtils::decompress(const std::vector<byte>& compressedData) {
    return decompress(compressedData.data(), compressedData.size());
}

std::vector<byte> CompressionUtils::decompress(const byte* compressedData, const size_t compressedDataSize) {
    const size_t originalSize = ByteArrayUtils::getValue<size_t>(compressedData, 0);
    std::vector<byte> decompressedData = initializeDecompressedBuffer(originalSize);
    performDecompression(compressedData, compressedDataSize, decompressedData);
    return decompressedData;
}

std::vector<byte> CompressionUtils::initializeDecompressedBuffer(const size_t originalSize) {
    return std::vector<byte>(originalSize);
}

void CompressionUtils::performDecompression(const byte* compressedData, const size_t compressedDataSize, std::vector<byte>& decompressedData) {
    LZ4_decompress_safe(
        reinterpret_cast<const char*>(compressedData + sizeof(size_t)),
        reinterpret_cast<char*>(decompressedData.data()),
        compressedDataSize - sizeof(size_t),
        decompressedData.size()
    );
}