#pragma once

#include <config_int.h>
#include <limits>

using CryptoPP::byte;

class TypeLimits {
public:
    TypeLimits() = delete;
    
    static constexpr size_t MAX_SIZE_T = (std::numeric_limits<size_t>::max)();
    static constexpr byte MAX_BYTE = (std::numeric_limits<byte>::max)();

    static constexpr size_t MIN_SIZE_T = (std::numeric_limits<size_t>::min)();
    static constexpr byte MIN_BYTE = (std::numeric_limits<byte>::min)();
};
