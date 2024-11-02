#include "../inc/utils.hpp"

#include <fstream>

#include "utils/WinSockUtils.hpp"

std::vector<byte> readFileToBuffer(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    return {(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()};
}

const std::string NetworkTestBase::IP = "127.0.0.1";
const unsigned short NetworkTestBase::PORT = 4040;

void NetworkTestBase::SetUp() {
    WinSockUtils::initializeWinSock();
}

void NetworkTestBase::TearDown() {
    WinSockUtils::cleanupWinSock();
}
