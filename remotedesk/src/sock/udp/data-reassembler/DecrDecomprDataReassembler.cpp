#include "../../../../inc/sock/udp/data-reassembler/DecrDecomprDataReassembler.hpp"

#include <execution>

#include "../../../../inc/utils/compression/Compressor.hpp"
#include "../../../../inc/utils/crypto/aes/AESToolkit.hpp"
#include "../../../../inc/utils/compression/CompressionToolkit.hpp"
#include "../../../../inc/utils/compression/Decompressor.hpp"
#include "../../../../inc/utils/sock/udp/PayloadSplitter.hpp"

DecrDecomprDataReassembler::DecrDecomprDataReassembler(const std::vector<byte>& key, const FragmentDescriptor& fragmentDescriptor):
    DataReassembler(
        fragmentDescriptor
            .reducePayloadSize(CompressionToolkit::METADATA_SIZE + AESToolkit::METADATA_SIZE)
            .reduceDataSize(PayloadMerger::PREFIX_SIZE)),
    _decryptor(key) { }

MaskedData DecrDecomprDataReassembler::reassembleData(std::vector<Fragment>& fragments) {
    decryptFragmentPayloads(fragments);
    splitIntoChunks(fragments);
    decompressFragmentPayloads(fragments);
    return DataReassembler::reassembleData(fragments);
}

void DecrDecomprDataReassembler::decryptFragmentPayloads(std::vector<Fragment>& fragments) {
    for (auto& fragment : fragments) {
        fragment.payload = _decryptor.decrypt(fragment.payload);
    }
}

void DecrDecomprDataReassembler::splitIntoChunks(std::vector<Fragment>& fragments) {
    std::vector<Fragment> chunksOfFragments = createFragmentChunks(fragments);
    for (const auto& fragment : fragments) {
        addFragmentChunks(chunksOfFragments, fragment);
    }
    fragments = std::move(chunksOfFragments);
}

std::vector<Fragment> DecrDecomprDataReassembler::createFragmentChunks(const std::vector<Fragment>& fragments) {
    std::vector<Fragment> fragmentChunks;
    fragmentChunks.reserve(fragments.size() * 8);
    return fragmentChunks;
}

void DecrDecomprDataReassembler::addFragmentChunks(std::vector<Fragment>& chunksOfFragments, const Fragment& fragment) {
    std::vector<std::vector<byte>> chunks = PayloadSplitter::splitIntoChunks(fragment.payload);
    size_t fragmentNumber = fragment.fragmentNumber;
    for (auto& chunk : chunks) {
        chunksOfFragments.emplace_back(
            Metadata{
                fragment.fragmentId,
                fragmentNumber++,
                fragment.totalDataSize
            },
            std::move(chunk)
        );
    }
}

void DecrDecomprDataReassembler::decompressFragmentPayloads(std::vector<Fragment>& decryptedFragments) {
    std::for_each(std::execution::par, decryptedFragments.begin(), decryptedFragments.end(),
        [](Fragment& fragment) {
            fragment.payload = Decompressor::decompress(fragment.payload);
        }
    );
}
