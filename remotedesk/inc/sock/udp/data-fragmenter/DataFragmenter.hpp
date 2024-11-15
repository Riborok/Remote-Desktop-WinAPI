#pragma once

#include <vector>

#include "../FragmentDescriptor.hpp"
#include "../../../types/byte.hpp"
#include "../../../utils/sock/udp/UDPToolkit.hpp"

class DataFragmenter {
    FragmentDescriptor _fragmentDescriptor;
    size_t _id = 1;
public:
    explicit DataFragmenter(const FragmentDescriptor& fragmentDescriptor = UDPToolkit::MAX_FRAGMENT_DESCRIPTOR);
    std::vector<std::vector<byte>> createFragments(const std::vector<byte>& data);
    const FragmentDescriptor& getFragmentDescriptor() const;
    virtual ~DataFragmenter() = default;
    DataFragmenter(DataFragmenter&&) = default;
    DataFragmenter& operator=(DataFragmenter&&) = default;
    DataFragmenter(const DataFragmenter&) = default;
    DataFragmenter& operator=(const DataFragmenter&) = default;
protected:
    virtual std::vector<std::vector<byte>> splitDataIntoFragments(const std::vector<byte>& data);
    size_t getId() const;
private:
    std::vector<std::vector<byte>> createFragmentPayloads(const std::vector<byte>& data) const;
    std::vector<std::vector<byte>> createFragments(const std::vector<std::vector<byte>>& fragmentPayloads, const size_t totalSize) const;
};
