#pragma once

#include <integer.h>

using CryptoPP::byte;

class IntegerUtils {
public:
    IntegerUtils() = delete;
    
    static std::vector<byte> toVector(const CryptoPP::Integer& value);
    static CryptoPP::SecByteBlock toSecByteBlock(const CryptoPP::Integer& value);
    static std::string toHexString(const CryptoPP::Integer& value);
};
