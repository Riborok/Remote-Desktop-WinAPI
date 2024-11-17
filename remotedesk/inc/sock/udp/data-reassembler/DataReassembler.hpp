#pragma once

#include <vector>

#include "../FragmentDescriptor.hpp"
#include "../../../utils/sock/udp/UDPToolkit.hpp"

class DataReassembler {
    FragmentDescriptor _fragmentDescriptor;
public:
    explicit DataReassembler(const FragmentDescriptor& fragmentDescriptor = UDPToolkit::MAX_FRAGMENT_DESCRIPTOR);
    virtual std::vector<byte> reassembleData(std::vector<Fragment>& fragments);
    const FragmentDescriptor& getFragmentDescriptor() const;
    virtual ~DataReassembler() = default;
    DataReassembler(DataReassembler&&) = default;
    DataReassembler& operator=(DataReassembler&&) = default;
    DataReassembler(const DataReassembler&) = default;
    DataReassembler& operator=(const DataReassembler&) = default;
private:
    void insertIntoDataFragments(std::vector<byte> &data, const std::vector<Fragment>& fragments) const;
    void insertIntoDataFragment(std::vector<byte> &data, const Fragment& fragment) const;
};
