#pragma once

#include <windows.h>

#include "TileSplitter.hpp"
#include "../sock/udp/Fragment.hpp"
#include "../utils/img/ImageUtils.hpp"

class ImageTileComposer {
    cv::Mat _image;
    TileSplitter _tileSplitter;
public:
    explicit ImageTileComposer(const SIZE& size, const int tileWidth = 128, const int tileHeight = 128);
    std::vector<byte> overlayTiles(const std::vector<Fragment>& tiles) const;
private:
    static cv::Mat decompressTile(const std::vector<byte>& compressedTile);
    void placeTile(const cv::Mat& tile, const cv::Rect& tileRegion) const;
};
