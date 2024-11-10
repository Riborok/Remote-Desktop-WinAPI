#pragma once

#include <config_int.h>
#include <vector>

using CryptoPP::byte;

class DataFragmenter {
public:
    DataFragmenter() = default;
    virtual std::vector<std::vector<byte>> createDataFragments(const std::vector<byte>& data);
    virtual ~DataFragmenter() = default;
    DataFragmenter(DataFragmenter&&) = default;
    DataFragmenter& operator=(DataFragmenter&&) = default;
    DataFragmenter(const DataFragmenter&) = default;
    DataFragmenter& operator=(const DataFragmenter&) = default;
protected:
    static std::tuple<const byte*, size_t> getDataFragment(const std::vector<byte>& data, const size_t i);
};
