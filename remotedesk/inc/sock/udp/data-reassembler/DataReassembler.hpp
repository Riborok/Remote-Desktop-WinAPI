#pragma once

#include <config_int.h>
#include <vector>

#include "../receiver/MaskedData.hpp"

using CryptoPP::byte;

class DataReassembler {
public:
    DataReassembler() = default;
    virtual MaskedData reassembleData(std::vector<Fragment>& fragments);
    virtual ~DataReassembler() = default;
    DataReassembler(DataReassembler&&) = default;
    DataReassembler& operator=(DataReassembler&&) = default;
    DataReassembler(const DataReassembler&) = default;
    DataReassembler& operator=(const DataReassembler&) = default;
private:
    static void initializeMaskedData(MaskedData& maskedData, const std::vector<Fragment>& fragments);
    static void insertFragments(MaskedData& maskedData, const std::vector<Fragment>& fragments);
};
