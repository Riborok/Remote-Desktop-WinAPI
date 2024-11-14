#include "../../../../inc/sock/udp/data-reassembler/DataReassembler.hpp"

DataReassembler::DataReassembler(const FragmentDescriptor& fragmentDescriptor):
    _fragmentDescriptor(fragmentDescriptor) { }

MaskedData DataReassembler::reassembleData(std::vector<Fragment>& fragments) {
    MaskedData maskedData(_fragmentDescriptor.getPayloadSize());
    if (fragments.empty()) {
        return maskedData;
    }
    initializeMaskedData(maskedData, fragments);
    insertFragments(maskedData, fragments);
    return maskedData;
}

const FragmentDescriptor& DataReassembler::getFragmentDescriptor() const { return _fragmentDescriptor; }

void DataReassembler::initializeMaskedData(MaskedData& maskedData, const std::vector<Fragment>& fragments) {
    maskedData.resize(fragments[0].totalDataSize);
}

void DataReassembler::insertFragments(MaskedData& maskedData, const std::vector<Fragment>& fragments) {
    for (const auto& fragment : fragments) {
        maskedData.insert(fragment);
    }
}