#pragma once

#include <vector>

#include "../Fragment.hpp"
#include "../../../types/byte.hpp"

class MaskedData {
    size_t _fragmentPayloadSize;
    std::vector<byte> _data;
    std::vector<byte> _mask;
public:
    explicit MaskedData(const size_t fragmentPayloadSize);

    void resize(const size_t size);
    void insert(const Fragment& fragment);

    const std::vector<byte>& getData() const;
    const std::vector<byte>& getMask() const;
};
