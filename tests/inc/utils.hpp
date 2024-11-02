#pragma once

#include <config_int.h>
#include <vector>
#include <gtest/gtest.h>

using CryptoPP::byte;

std::vector<byte> readFileToBuffer(const std::string& filename);

class NetworkTestBase : public testing::Test {
protected:
    const static std::string IP;
    const static unsigned short PORT;
    
    void SetUp() override;
    void TearDown() override;
};
