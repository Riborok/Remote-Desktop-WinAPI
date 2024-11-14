#pragma once

#include <vector>

#include "../FragmentDescriptor.hpp"
#include "../../../types/byte.hpp"
#include "../../../utils/sock/udp/UDPToolkit.hpp"

class DataFragmenter {
    FragmentDescriptor _fragmentDescriptor;
public:
    explicit DataFragmenter(const FragmentDescriptor& fragmentDescriptor = UDPToolkit::MAX_FRAGMENT_DESCRIPTOR);
    virtual std::vector<std::vector<byte>> createFragmentPayloads(const std::vector<byte>& data);
    const FragmentDescriptor& getFragmentDescriptor() const;
    virtual ~DataFragmenter() = default;
    DataFragmenter(DataFragmenter&&) = default;
    DataFragmenter& operator=(DataFragmenter&&) = default;
    DataFragmenter(const DataFragmenter&) = default;
    DataFragmenter& operator=(const DataFragmenter&) = default;
};
