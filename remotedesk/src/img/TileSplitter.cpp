#include "../../inc/img/TileSplitter.hpp"

#include "../../inc/utils/MathUtils.hpp"

TileSplitter::TileSplitter(const int tileWidth, const int tileHeight):
    _tileWidth(tileWidth), _tileHeight(tileHeight) { }

int TileSplitter::calcTotalTiles(const int imageWidth, const int imageHeight) const {
    const int numTilesX = MathUtils::ceil(imageWidth, _tileWidth);
    const int numTilesY = MathUtils::ceil(imageHeight, _tileHeight);
    return numTilesX * numTilesY;
}

cv::Rect TileSplitter::computeTileRegion(const int tileIndex, const int imageWidth, const int imageHeight) const {
    const int numTilesX = MathUtils::ceil(imageWidth, _tileWidth);
    const int tileY = tileIndex / numTilesX;
    const int tileX = tileIndex % numTilesX;
    int x = tileX * _tileWidth;
    int y = tileY * _tileHeight;
    int currentTileWidth = std::min(_tileWidth, imageWidth - x);
    int currentTileHeight = std::min(_tileHeight, imageHeight - y);
    return {x, y, currentTileWidth, currentTileHeight};
}
