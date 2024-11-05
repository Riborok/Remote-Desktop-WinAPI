#pragma once

class MemoryUnits {
public:
    MemoryUnits() = delete;

    static constexpr size_t KILOBYTE = 1024;
    static constexpr size_t MEGABYTE = 1024 * KILOBYTE;
    static constexpr size_t GIGABYTE = 1024 * MEGABYTE;
    static constexpr size_t TERABYTE = 1024 * GIGABYTE;
};
