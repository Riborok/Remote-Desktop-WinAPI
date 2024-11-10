#pragma once

#include <config_int.h>
#include <vector>

#include "Metadata.hpp"

using CryptoPP::byte;

struct Fragment: Metadata {
    std::vector<byte> data;
};
