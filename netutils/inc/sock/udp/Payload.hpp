#pragma once

#include <config_int.h>
#include <vector>

using CryptoPP::byte;

struct Payload {
    std::vector<byte> data;
    size_t packetNumber;
};
