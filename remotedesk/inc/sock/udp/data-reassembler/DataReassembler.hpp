#pragma once

#include <vector>

#include "../FragmentDescriptor.hpp"
#include "../../../utils/sock/udp/UDPToolkit.hpp"
#include "../receiver/MaskedData.hpp"

class DataReassembler {
    FragmentDescriptor _fragmentDescriptor;
public:
    explicit DataReassembler(const FragmentDescriptor& fragmentDescriptor = UDPToolkit::MAX_FRAGMENT_DESCRIPTOR);
    virtual MaskedData reassembleData(std::vector<Fragment>& fragments);
    const FragmentDescriptor& getFragmentDescriptor() const;
    virtual ~DataReassembler() = default;
    DataReassembler(DataReassembler&&) = default;
    DataReassembler& operator=(DataReassembler&&) = default;
    DataReassembler(const DataReassembler&) = default;
    DataReassembler& operator=(const DataReassembler&) = default;
private:
    static void initializeMaskedData(MaskedData& maskedData, const std::vector<Fragment>& fragments);
    static void insertFragments(MaskedData& maskedData, const std::vector<Fragment>& fragments);
};
