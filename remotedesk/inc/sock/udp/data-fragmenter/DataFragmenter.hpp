#pragma once

#include <config_int.h>
#include <vector>

#include "../FragmentDescriptor.hpp"
#include "../../../utils/sock/udp/UDPToolkit.hpp"

using CryptoPP::byte;

class DataFragmenter {
    FragmentDescriptor _fragmentDescriptor;
public:
    explicit DataFragmenter(const FragmentDescriptor& fragmentDescriptor = UDPToolkit::MAX_FRAGMENT_DESCRIPTOR);
    virtual std::vector<std::vector<byte>> createDataFragments(const std::vector<byte>& data);
    const FragmentDescriptor& getFragmentDescriptor() const;
    virtual ~DataFragmenter() = default;
    DataFragmenter(DataFragmenter&&) = default;
    DataFragmenter& operator=(DataFragmenter&&) = default;
    DataFragmenter(const DataFragmenter&) = default;
    DataFragmenter& operator=(const DataFragmenter&) = default;
protected:
    std::tuple<const byte*, size_t> getDataFragment(const std::vector<byte>& data, const size_t i) const;
};
