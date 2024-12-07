#pragma once

#include <windows.h>
#include <vector>

#include "ImageFormat.hpp"
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
    explicit ImageTileSplitter(const SIZE& size, const ImageFormat ext = ImageFormat::jpg, const int quality = 65, const int tileWidth = 128, const int tileHeight = 128);
    std::vector<std::vector<byte>> splitToTiles(const std::vector<byte>& imageBuffer) const;
    void setExtension(const ImageFormat ext);
    void setQuality(const int quality);
private:
    std::vector<byte> compressTile(const cv::Mat& tile) const;
};
