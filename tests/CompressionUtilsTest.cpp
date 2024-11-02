#include "inc/utils.hpp"
#include "utils/CompressionUtils.hpp"
#include "gtest/gtest.h"

void performCompressionDecompressionTest(const std::string& filename) {
    const std::vector<byte> data(readFileToBuffer(filename));
    const std::vector<byte> compressedData = CompressionUtils::compress(data);
    const std::vector<byte> decompressedData = CompressionUtils::decompress(compressedData);
    
    EXPECT_GT(data.size(), compressedData.size());
    EXPECT_EQ(data.size(), decompressedData.size());
    EXPECT_EQ(data, decompressedData);
}

TEST(CompressionUtilsTest, CompressAndDecompressTXT) {
    performCompressionDecompressionTest("res/test.txt");
}

TEST(CompressionUtilsTest, CompressAndDecompressJPG) {
    performCompressionDecompressionTest("res/test.jpg");
}
