#pragma once

#include <vector>

#include "Metadata.hpp"
#include "../../types/byte.hpp"

struct Fragment: Metadata {
    std::vector<byte> data;
};
