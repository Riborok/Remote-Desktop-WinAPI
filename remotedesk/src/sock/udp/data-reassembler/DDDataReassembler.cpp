#include "../../../../inc/sock/udp/data-reassembler/DDDataReassembler.hpp"

#include <execution>

#include "../../../../inc/utils/CompressionUtils.hpp"

DDDataReassembler::DDDataReassembler(std::vector<byte>&& key): _decryptor(std::move(key)) { }

MaskedData DDDataReassembler::reassembleData(std::vector<Fragment>& fragments) {
    decryptDataFragments(fragments);
    decompressDataFragments(fragments);
    return DataReassembler::reassembleData(fragments);
}

void DDDataReassembler::decryptDataFragments(std::vector<Fragment>& fragments) {
    for (auto& fragment : fragments) {
        fragment.data = _decryptor.decrypt(fragment.data);
    }
}

void DDDataReassembler::decompressDataFragments(std::vector<Fragment>& decryptedFragments) {
    std::for_each(std::execution::par, decryptedFragments.begin(), decryptedFragments.end(),
        [](Fragment& fragment) {
            fragment.data = CompressionUtils::decompress(fragment.data);
        }
    );
}
