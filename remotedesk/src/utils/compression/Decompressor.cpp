// ReSharper disable CppClangTidyBugproneNarrowingConversions CppClangTidyClangDiagnosticShorten64To32 CppClangTidyClangDiagnosticSignCompare
#include "../../../inc/utils/compression/Decompressor.hpp"

#include <lz4.h>

#include "../../../inc/utils/array/ByteArrayUtils.hpp"
#include "../../../inc/utils/compression/CompressionToolkit.hpp"

std::vector<byte> Decompressor::decompress(const std::vector<byte>& compressedData) {
    return decompress(compressedData.data(), compressedData.size());
}

std::vector<byte> Decompressor::decompress(const byte* compressedData, const size_t compressedDataSize) {
    const int originalSize = ByteArrayUtils::getValue<OriginalSize>(compressedData, 0);
    if (isCompressedData(originalSize)) {
        std::vector<byte> decompressedData = createDecompressedDataBuffer(originalSize);
        performDecompression(compressedData, compressedDataSize, decompressedData);
        return decompressedData;
    }
    return extractUncompressedData(compressedData, compressedDataSize);
}

bool Decompressor::isCompressedData(const int originalSize) {
    return originalSize != CompressionToolkit::UNCOMPRESSED_DATA_INDICATOR;
}

std::vector<byte> Decompressor::extractUncompressedData(const byte* compressedData, const size_t compressedDataSize) {
    return {compressedData + CompressionToolkit::METADATA_SIZE, compressedData + compressedDataSize};
}

std::vector<byte> Decompressor::createDecompressedDataBuffer(const int originalSize) {
    return std::vector<byte>(originalSize);
}

void Decompressor::performDecompression(const byte* compressedData, const int compressedDataSize, std::vector<byte>& decompressedData) {
    LZ4_decompress_safe(
        reinterpret_cast<const char*>(compressedData + CompressionToolkit::METADATA_SIZE),
        reinterpret_cast<char*>(decompressedData.data()),
        compressedDataSize - CompressionToolkit::METADATA_SIZE,
        decompressedData.size()
    );
}