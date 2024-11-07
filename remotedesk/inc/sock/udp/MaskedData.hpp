﻿#pragma once

#include <config_int.h>
#include <vector>

#include "Payload.hpp"

using CryptoPP::byte;

class MaskedData {
    std::vector<byte> _data;
    std::vector<byte> _mask;
public:
    void resize(const size_t size);
    void insertPayload(const Payload& payload);

    const std::vector<byte>& getData() const;
    const std::vector<byte>& getMask() const;
};