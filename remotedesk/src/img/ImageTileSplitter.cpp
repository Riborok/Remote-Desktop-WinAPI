#include "../../inc/img/ImageTileSplitter.hpp"

#include "../../inc/utils/img/ImageUtils.hpp"

std::vector<int> ImageTileSplitter::CompressionConfig::cloneCompressionParamsWithQuality(const int quality) const {
    std::vector<int> result = compressionParams;
    result[1] = quality;
    return result;
}

const ImageTileSplitter::CompressionConfig ImageTileSplitter::COMPRESSION_CONFIGS[] = {
    { {cv::IMWRITE_JPEG_QUALITY, 100, cv::IMWRITE_JPEG_OPTIMIZE, 1}, ".jpg" },
    { {cv::IMWRITE_WEBP_QUALITY, 100}, ".webp" }
};

ImageTileSplitter::ImageTileSplitter(const SIZE& size, const ImageFormat ext, const int quality,
                                     const int tileWidth, const int tileHeight):
    _size(size),
    _ext(static_cast<size_t>(ext)),
    _quality(quality),
    _tileSplitter(tileWidth, tileHeight) { }

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

void ImageTileSplitter::setExtension(const ImageFormat ext) {
    _ext = static_cast<size_t>(ext);
}

void ImageTileSplitter::setQuality(const int quality) {
    _quality = quality;
}

std::vector<byte> ImageTileSplitter::compressTile(const cv::Mat& tile) const {
    std::vector<byte> encodedTile;
    const CompressionConfig& cc = COMPRESSION_CONFIGS[_ext];
    cv::imencode(cc.extension, tile, encodedTile, cc.cloneCompressionParamsWithQuality(_quality));
    return encodedTile;
}
