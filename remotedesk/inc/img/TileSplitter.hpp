#pragma once

#include <opencv2/opencv.hpp>

class TileSplitter {
    int _tileWidth;
    int _tileHeight;
public:
    TileSplitter(const int tileWidth, const int tileHeight);
    int calcTotalTiles(const int imageWidth, const int imageHeight) const;
    cv::Rect computeTileRegion(const int tileIndex, const int imageWidth, const int imageHeight) const;
};
