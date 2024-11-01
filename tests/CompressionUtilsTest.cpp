#include "utils.hpp"
#include "utils/CompressionUtils.hpp"
#include "gtest/gtest.h"

TEST(CompressionUtilsTest, CompressAndDecompress) {
    const std::vector<byte> data(readFileToBuffer("test.txt"));
    const std::vector<byte> compressedData = CompressionUtils::compress(data);
    const std::vector<byte> decompressedData = CompressionUtils::decompress(compressedData);
    
    EXPECT_GT(data.size(), compressedData.size());
    EXPECT_EQ(data.size(), decompressedData.size());
    EXPECT_EQ(data, decompressedData);
}
