#include "../../../../inc/sock/udp/data-reassembler/DecrDecomprDataReassembler.hpp"

#include <execution>

#include "../../../../inc/utils/compression/Compressor.hpp"
#include "../../../../inc/utils/crypto/aes/AESToolkit.hpp"
#include "../../../../inc/utils/compression/CompressionToolkit.hpp"
#include "../../../../inc/utils/compression/Decompressor.hpp"

DecrDecomprDataReassembler::DecrDecomprDataReassembler(const std::vector<byte>& key, const FragmentDescriptor& fragmentDescriptor):
    DataReassembler(fragmentDescriptor.reducePayloadSize(
        CompressionToolkit::METADATA_SIZE + AESToolkit::METADATA_SIZE)),
    _decryptor(key) { }

MaskedData DecrDecomprDataReassembler::reassembleData(std::vector<Fragment>& fragments) {
    decryptFragmentPayloads(fragments);
    decompressFragmentPayloads(fragments);
    return DataReassembler::reassembleData(fragments);
}

void DecrDecomprDataReassembler::decryptFragmentPayloads(std::vector<Fragment>& fragments) {
    for (auto& fragment : fragments) {
        fragment.payload = _decryptor.decrypt(fragment.payload);
    }
}

void DecrDecomprDataReassembler::decompressFragmentPayloads(std::vector<Fragment>& decryptedFragments) {
    std::for_each(std::execution::par, decryptedFragments.begin(), decryptedFragments.end(),
        [](Fragment& fragment) {
            fragment.payload = Decompressor::decompress(fragment.payload);
        }
    );
}
