#pragma once

#include <config_int.h>
#include <vector>

#include "../Fragment.hpp"

using CryptoPP::byte;

class MaskedData {
    std::vector<byte> _data;
    std::vector<byte> _mask;
public:
    void resize(const size_t size);
    void insert(const Fragment& fragment);

    const std::vector<byte>& getData() const;
    const std::vector<byte>& getMask() const;
};
