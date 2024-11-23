#pragma once

#include "DataReassembler.hpp"
#include "../../../crypto/aes/AESDecryptor.hpp"
#include "../../../img/ImageTileComposer.hpp"

class ImgCodecSecureReassembler final : public DataReassembler {
    ImageTileComposer _imageTileComposer;
    AESDecryptor _decryptor;
public:
    explicit ImgCodecSecureReassembler(const SIZE& size, const std::vector<byte>& key, const FragmentDescriptor& fragmentDescriptor = UDPToolkit::MAX_FRAGMENT_DESCRIPTOR);
    std::vector<byte> reassembleData(std::vector<Fragment>& fragments) override;
private:
    void decryptFragmentPayloads(std::vector<Fragment>& fragments);
    static std::vector<Chunk> splitIntoChunks(const std::vector<Fragment>& fragments);
    static std::vector<Chunk> createChunks(const std::vector<Fragment>& fragments);
    static void addChunksOfFragment(std::vector<Chunk>& chunksOfFragments, const Fragment& fragment);
};
