#include "../../inc/img/ImageTileSplitter.hpp"

#include "../../inc/utils/img/ImageUtils.hpp"

const std::unordered_map<std::string, std::vector<int>> ImageTileSplitter::DEFAULT_PARAMS = {
    {".jpg", {cv::IMWRITE_JPEG_QUALITY, 100, cv::IMWRITE_JPEG_OPTIMIZE, true}},
    {".webp", {cv::IMWRITE_WEBP_QUALITY, 100}}
};

ImageTileSplitter::ImageTileSplitter(const SIZE& size, std::string ext,
            const int quality, const int tileWidth, const int tileHeight):
        _size(size),
        _tileSplitter(tileWidth, tileHeight),
        _ext(std::move(ext)),
        _quality(quality){
    setCompressionParams();
}

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

void ImageTileSplitter::setExtension(std::string ext) {
    _ext = std::move(ext);
    setCompressionParams();
}

void ImageTileSplitter::setQuality(const int quality) {
    _quality = quality;
    setCompressionParams();
}

void ImageTileSplitter::setCompressionParams() {
    const auto it = DEFAULT_PARAMS.find(_ext);
    if (it != DEFAULT_PARAMS.end()) {
        _compressionParams = it->second;
        _compressionParams[1] = _quality;
    } else {
        throw std::invalid_argument("Unsupported image format: " + _ext);
    }
}

std::vector<byte> ImageTileSplitter::compressTile(const cv::Mat& tile) const {
    std::vector<byte> encodedTile;
    cv::imencode(_ext, tile, encodedTile, _compressionParams);
    return encodedTile;
}
