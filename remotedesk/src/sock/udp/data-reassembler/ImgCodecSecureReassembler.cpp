#include "../../../../inc/sock/udp/data-reassembler/ImgCodecSecureReassembler.hpp"

#include "../../../../inc/utils/crypto/aes/AESToolkit.hpp"
#include "../../../../inc/utils/sock/udp/PayloadSplitter.hpp"

ImgCodecSecureReassembler::ImgCodecSecureReassembler(const SIZE& size, const std::vector<byte>& key, const FragmentDescriptor& fragmentDescriptor):
    DataReassembler(
        fragmentDescriptor
            .reducePayloadSize(AESToolkit::METADATA_SIZE)
            .reduceDataSize(PayloadMerger::PREFIX_SIZE)),
    _imageTileComposer(size),
    _decryptor(key) { }

std::vector<byte> ImgCodecSecureReassembler::reassembleData(std::vector<Fragment>& fragments) {
    decryptFragmentPayloads(fragments);
    splitIntoChunks(fragments);
    return _imageTileComposer.overlayTiles(fragments);
}

void ImgCodecSecureReassembler::decryptFragmentPayloads(std::vector<Fragment>& fragments) {
    for (auto& fragment : fragments) {
        fragment.payload = _decryptor.decrypt(fragment.payload);
    }
}

void ImgCodecSecureReassembler::splitIntoChunks(std::vector<Fragment>& fragments) {
    std::vector<Fragment> chunksOfFragments = createFragmentChunks(fragments);
    for (const auto& fragment : fragments) {
        addFragmentChunks(chunksOfFragments, fragment);
    }
    fragments = std::move(chunksOfFragments);
}

std::vector<Fragment> ImgCodecSecureReassembler::createFragmentChunks(const std::vector<Fragment>& fragments) {
    std::vector<Fragment> fragmentChunks;
    fragmentChunks.reserve(fragments.size() * 64);
    return fragmentChunks;
}

void ImgCodecSecureReassembler::addFragmentChunks(std::vector<Fragment>& chunksOfFragments, const Fragment& fragment) {
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
