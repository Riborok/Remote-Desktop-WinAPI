#pragma once

#include <windows.h>
#include <vector>

#include "TileSplitter.hpp"

class ImageTileSplitter {
    SIZE _size;
    TileSplitter _tileSplitter;
    std::vector<int> _compressionParams;
public:
    explicit ImageTileSplitter(const SIZE& size, const int jpegQuality = 90, const int tileWidth = 128, const int tileHeight = 128);
    std::vector<std::vector<byte>> splitToTiles(const std::vector<byte>& imageBuffer) const;
private:
    std::vector<byte> compressTile(const cv::Mat& tile) const;
};
