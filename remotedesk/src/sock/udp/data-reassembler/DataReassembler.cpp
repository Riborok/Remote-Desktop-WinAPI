#include "../../../../inc/sock/udp/data-reassembler/DataReassembler.hpp"

MaskedData DataReassembler::reassembleData(std::vector<Fragment>& fragments) {
    MaskedData maskedData;
    if (fragments.empty()) {
        return maskedData;
    }
    initializeMaskedData(maskedData, fragments);
    insertFragments(maskedData, fragments);
    return maskedData;
}

void DataReassembler::initializeMaskedData(MaskedData& maskedData, const std::vector<Fragment>& fragments) {
    maskedData.resize(fragments[0].totalDataSize);
}

void DataReassembler::insertFragments(MaskedData& maskedData, const std::vector<Fragment>& fragments) {
    for (const auto& fragment : fragments) {
        maskedData.insert(fragment);
    }
}