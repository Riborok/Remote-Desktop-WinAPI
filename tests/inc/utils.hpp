#pragma once

#include <vector>
#include <gtest/gtest.h>

#include "types/byte.hpp"

std::vector<byte> readFileToBuffer(const std::string& filename);

class NetworkTestBase : public testing::Test {
protected:
    const static std::string IP;
    const static unsigned short PORT;
    
    void SetUp() override;
    void TearDown() override;
};
