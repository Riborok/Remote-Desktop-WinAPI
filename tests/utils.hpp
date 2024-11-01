#pragma once

#include <config_int.h>
#include <fstream>
#include <vector>
#include <stdexcept>

using CryptoPP::byte;

inline std::vector<byte> readFileToBuffer(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    return {(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()};
}
