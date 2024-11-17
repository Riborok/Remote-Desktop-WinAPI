#include "../../../../inc/sock/udp/data-reassembler/DataReassembler.hpp"

DataReassembler::DataReassembler(const FragmentDescriptor& fragmentDescriptor):
    _fragmentDescriptor(fragmentDescriptor) { }

std::vector<byte> DataReassembler::reassembleData(std::vector<Fragment>& fragments) {
    if (fragments.empty()) {
        return {};
    }
    std::vector<byte> data(fragments[0].totalDataSize, 0);
    insertIntoDataFragments(data, fragments);
    return data;
}

const FragmentDescriptor& DataReassembler::getFragmentDescriptor() const { return _fragmentDescriptor; }

void DataReassembler::insertIntoDataFragments(std::vector<byte> &data, const std::vector<Fragment>& fragments) const {
    for (const auto& fragment : fragments) {
        insertIntoDataFragment(data, fragment);
    }
}

void DataReassembler::insertIntoDataFragment(std::vector<byte> &data, const Fragment& fragment) const {
    const size_t offset = fragment.fragmentNumber * _fragmentDescriptor.getDataSize();
    std::memcpy(&data[offset], fragment.payload.data(), fragment.payload.size());
}
