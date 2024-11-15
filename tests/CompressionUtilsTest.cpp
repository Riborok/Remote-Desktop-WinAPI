#include "inc/utils.hpp"
#include "utils/compression/Compressor.hpp"
#include "gtest/gtest.h"
#include "utils/compression/Decompressor.hpp"

namespace {
    void performCompressionDecompressionTest(const std::string& filename) {
        const std::vector<byte> data(readFileToBuffer(filename));
        const std::vector<byte> compressedData = Compressor::compress(data);
        const std::vector<byte> decompressedData = Decompressor::decompress(compressedData);
    
        EXPECT_EQ(data.size(), decompressedData.size());
        EXPECT_EQ(data, decompressedData);
    }
}

TEST(CompressionTest, CompressAndDecompressTXT) {
    performCompressionDecompressionTest("res/test.txt");
}

TEST(CompressionTest, CompressAndDecompressJPG) {
    performCompressionDecompressionTest("res/test.jpg");
}
