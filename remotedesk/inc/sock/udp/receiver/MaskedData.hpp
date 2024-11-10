﻿#pragma once

#include <config_int.h>
#include <vector>

#include "../Fragment.hpp"

using CryptoPP::byte;

class MaskedData {
    size_t _fragmentDataSize;
    std::vector<byte> _data;
    std::vector<byte> _mask;
public:
    explicit MaskedData(const size_t fragmentDataSize);

    void resize(const size_t size);
    void insert(const Fragment& fragment);

    const std::vector<byte>& getData() const;
    const std::vector<byte>& getMask() const;
};
