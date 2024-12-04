#pragma once

#include <windows.h>
#include <vector>

#include "TileSplitter.hpp"

class ImageTileSplitter {
    static const std::unordered_map<std::string, std::vector<int>> DEFAULT_PARAMS;
    
    SIZE _size;
    TileSplitter _tileSplitter;
    std::string _ext;
    int _quality;
    std::vector<int> _compressionParams;
public:
    explicit ImageTileSplitter(const SIZE& size, std::string ext = ".jpg", const int quality = 65, const int tileWidth = 128, const int tileHeight = 128);
    std::vector<std::vector<byte>> splitToTiles(const std::vector<byte>& imageBuffer) const;
    void setExtension(std::string ext);
    void setQuality(const int quality);
private:
    void setCompressionParams();
    std::vector<byte> compressTile(const cv::Mat& tile) const;
};
