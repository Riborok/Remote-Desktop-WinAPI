#include "../../../../inc/sock/udp/data-reassembler/DDDataReassembler.hpp"

#include <execution>

#include "../../../../inc/utils/compression/CompressionUtils.hpp"
#include "../../../../inc/utils/aes/AESToolkit.hpp"

DDDataReassembler::DDDataReassembler(const std::vector<byte>& key, const FragmentDescriptor& fragmentDescriptor):
    DataReassembler(fragmentDescriptor.reduceDataSize(
        CompressionUtils::METADATA_SIZE + AESToolkit::METADATA_SIZE)),
    _decryptor(key) { }

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
