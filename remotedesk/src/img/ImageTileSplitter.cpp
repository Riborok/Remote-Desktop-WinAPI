#include "../../inc/img/ImageTileSplitter.hpp"

#include "../../inc/utils/img/ImageUtils.hpp"

ImageTileSplitter::ImageTileSplitter(const SIZE& size, const int jpegQuality, const int tileWidth, const int tileHeight):
    _size(size),
    _tileSplitter(tileWidth, tileHeight),
    _compressionParams{cv::IMWRITE_JPEG_QUALITY, jpegQuality, cv::IMWRITE_JPEG_OPTIMIZE, true} { }

std::vector<std::vector<byte>> ImageTileSplitter::splitToTiles(const std::vector<byte>& imageBuffer) const {
    const cv::Mat image = ImageUtils::createImageFromBuffer(const_cast<byte*>(imageBuffer.data()), _size);
    const int imageWidth = image.cols;
    const int imageHeight = image.rows;
    const int totalTiles = _tileSplitter.calcTotalTiles(imageWidth, imageHeight);
    
    std::vector<std::vector<byte>> compressedTiles(totalTiles);
    cv::parallel_for_(cv::Range(0, totalTiles), [&](const cv::Range& range) {
        for (int i = range.start; i < range.end; ++i) {
            cv::Rect tileRegion = _tileSplitter.computeTileRegion(i, imageWidth, imageHeight);
            cv::Mat tile = image(tileRegion);
            compressedTiles[i] = compressTile(tile);
        }
    });
    return compressedTiles;
}

std::vector<byte> ImageTileSplitter::compressTile(const cv::Mat& tile) const {
    std::vector<byte> encodedTile;
    cv::imencode(".jpg", tile, encodedTile, _compressionParams);
    return encodedTile;
}
