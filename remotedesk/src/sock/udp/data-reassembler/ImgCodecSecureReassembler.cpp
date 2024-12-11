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
    const std::vector<Chunk> chunks = splitFragmentsIntoChunks(fragments);
    return _imageTileComposer.overlayTiles(chunks);
}

void ImgCodecSecureReassembler::decryptFragmentPayloads(std::vector<Fragment>& fragments) {
    for (auto& fragment : fragments) {
        fragment.payload = _decryptor.decrypt(fragment.payload);
    }
}

std::vector<Chunk> ImgCodecSecureReassembler::splitFragmentsIntoChunks(const std::vector<Fragment>& fragments) {
    std::vector<Chunk> chunksOfFragments = createChunks(fragments);
    for (const auto& fragment : fragments) {
        addChunksOfFragment(chunksOfFragments, fragment);
    }
    return chunksOfFragments;
}

std::vector<Chunk> ImgCodecSecureReassembler::createChunks(const std::vector<Fragment>& fragments) {
    std::vector<Chunk> chunks;
    chunks.reserve(fragments.size() * 64);
    return chunks;
}

void ImgCodecSecureReassembler::addChunksOfFragment(std::vector<Chunk>& chunksOfFragments, const Fragment& fragment) {
    std::vector<std::vector<byte>> chunksOfFragment = PayloadSplitter::splitIntoChunks(fragment.payload);
    size_t fragmentNumber = fragment.fragmentNumber;
    for (auto& chunkOfFragment : chunksOfFragment) {
        chunksOfFragments.emplace_back(
            fragmentNumber++,
            std::move(chunkOfFragment)
        );
    }
}
