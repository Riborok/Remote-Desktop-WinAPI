// ReSharper disable CppClangTidyBugproneNarrowingConversions CppClangTidyClangDiagnosticShorten64To32 CppClangTidyClangDiagnosticSignCompare
#include "../../../inc/utils/compression/Compressor.hpp"

#include <lz4.h>

#include "../../../inc/utils/array/ByteArrayUtils.hpp"
#include "../../../inc/utils/compression/CompressionToolkit.hpp"

std::vector<byte> Compressor::compress(const std::vector<byte>& data) {
    return compress(data.data(), data.size());
}

std::vector<byte> Compressor::compress(const byte* data, const size_t dataSize) {
    std::vector<byte> compressedData = createCompressedDataBuffer(dataSize);
    const int compressedSize = performCompression(data, dataSize, compressedData);
    if (isDataCompressed(compressedSize)) {
        compressedData.resize(compressedSize + CompressionToolkit::METADATA_SIZE); 
        return compressedData;
    }
    return createUncompressedData(data, dataSize);
}

bool Compressor::isDataCompressed(const int compressedSize) {
    return compressedSize != 0;
}

std::vector<byte> Compressor::createUncompressedData(const byte* data, const size_t dataSize) {
    std::vector<byte> uncompressedData = createDataBuffer(dataSize, CompressionToolkit::UNCOMPRESSED_DATA_INDICATOR);
    std::copy_n(data, dataSize, &uncompressedData[CompressionToolkit::METADATA_SIZE]);
    return uncompressedData;
}

std::vector<byte> Compressor::createCompressedDataBuffer(const int dataSize) {
    return createDataBuffer(dataSize, dataSize);
}

std::vector<byte> Compressor::createDataBuffer(const size_t dataSize, const int metadataValue) {
    std::vector<byte> buffer(dataSize + CompressionToolkit::METADATA_SIZE);
    ByteArrayUtils::setValue(buffer, 0, metadataValue);
    return buffer;
}

int Compressor::performCompression(const byte* data, const int dataSize, std::vector<byte>& compressedData) {
    const int maxCompressedSize = compressedData.size() - CompressionToolkit::METADATA_SIZE;
    return LZ4_compress_default(
        reinterpret_cast<const char*>(data),
        reinterpret_cast<char*>(compressedData.data() + CompressionToolkit::METADATA_SIZE),
        dataSize,
        maxCompressedSize
    );
}
