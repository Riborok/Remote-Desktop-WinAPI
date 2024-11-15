#pragma once

#include <vector>

#include "Metadata.hpp"
#include "../../types/byte.hpp"

struct Fragment: Metadata {
    std::vector<byte> payload;

    Fragment() = default;
    Fragment(const Metadata& metadata, std::vector<byte>&& payload):
        Metadata(metadata), payload(std::move(payload)) { }
};
