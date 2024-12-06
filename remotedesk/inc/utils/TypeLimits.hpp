#pragma once

#include <limits>
#include <windows.h>

class TypeLimits {
public:
    TypeLimits() = delete;
    
    static constexpr size_t MAX_SIZE_T_VALUE = (std::numeric_limits<size_t>::max)();
    static constexpr byte MAX_BYTE_VALUE = (std::numeric_limits<byte>::max)();
    static constexpr DWORD MAX_DWORD_VALUE = (std::numeric_limits<DWORD>::max)();
    static constexpr WORD MAX_WORD_VALUE = (std::numeric_limits<WORD>::max)();
    
    static constexpr size_t MIN_SIZE_T_VALUE = (std::numeric_limits<size_t>::min)();
    static constexpr byte MIN_BYTE_VALUE = (std::numeric_limits<byte>::min)();
    static constexpr DWORD MIN_DWORD_VALUE = (std::numeric_limits<DWORD>::min)();
    static constexpr WORD MIN_WORD_VALUE = (std::numeric_limits<WORD>::min)();
};
