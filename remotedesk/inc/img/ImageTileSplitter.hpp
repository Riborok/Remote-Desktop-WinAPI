#pragma once

#include <windows.h>
#include <vector>

#include "ImageConfig.hpp"
#include "TileSplitter.hpp"

class ImageTileSplitter {
    struct CompressionConfig {
        std::vector<int> compressionParams;
        std::string extension;
        std::vector<int> cloneCompressionParamsWithQuality(const int quality) const;
    };
    static const CompressionConfig COMPRESSION_CONFIGS[];

    SIZE _size;
    std::atomic<size_t> _ext;
    std::atomic<int> _quality;
    TileSplitter _tileSplitter;
public:
    explicit ImageTileSplitter(const SIZE& size, const ImageConfig &ic, const int tileWidth = 128, const int tileHeight = 128);
    ImageTileSplitter(const ImageTileSplitter& other);
    std::vector<std::vector<byte>> splitToTiles(const std::vector<byte>& imageBuffer) const;
    void updateImageConfig(const ImageConfig &ic);

    ~ImageTileSplitter() = default;
    
    ImageTileSplitter(ImageTileSplitter&&) = delete;
    ImageTileSplitter& operator=(ImageTileSplitter&&) = delete;
    ImageTileSplitter& operator=(const ImageTileSplitter&) = delete;
private:
    std::vector<byte> compressTile(const cv::Mat& tile) const;
};
