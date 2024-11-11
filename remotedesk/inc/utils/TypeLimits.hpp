#pragma once

#include <limits>

#include "../types/byte.hpp"

class TypeLimits {
public:
    TypeLimits() = delete;
    
    static constexpr size_t MAX_SIZE_T_VALUE = (std::numeric_limits<size_t>::max)();
    static constexpr byte MAX_BYTE_VALUE = (std::numeric_limits<byte>::max)();

    static constexpr size_t MIN_SIZE_T_VALUE = (std::numeric_limits<size_t>::min)();
    static constexpr byte MIN_BYTE_VALUE = (std::numeric_limits<byte>::min)();
};
