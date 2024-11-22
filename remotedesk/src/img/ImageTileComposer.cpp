// ReSharper disable CppClangTidyBugproneNarrowingConversions CppClangTidyClangDiagnosticShorten64To32
#include "../../inc/img/ImageTileComposer.hpp"

ImageTileComposer::ImageTileComposer(const SIZE& size, const int tileWidth, const int tileHeight):
    _image(ImageUtils::createBlackImage(size)),
    _tileSplitter(tileWidth, tileHeight) { }

std::vector<byte> ImageTileComposer::overlayTiles(const std::vector<Fragment>& tiles) const {
    const int imageWidth = _image.cols;
    const int imageHeight = _image.rows;
        
    cv::parallel_for_(cv::Range(0, tiles.size()), [&](const cv::Range& range) {
        for (int i = range.start; i < range.end; ++i) {
            cv::Rect tileRegion = _tileSplitter.computeTileRegion(tiles[i].fragmentNumber, imageWidth, imageHeight);
            cv::Mat tile = decompressTile(tiles[i].payload);
            placeTile(tile, tileRegion);
        }
    });
    return ImageUtils::copyToVector(_image);
}

cv::Mat ImageTileComposer::decompressTile(const std::vector<byte>& compressedTile) {
    cv::Mat tile = cv::imdecode(compressedTile, cv::IMREAD_COLOR);
    return ImageUtils::convertToRGBA(std::move(tile));
}

void ImageTileComposer::placeTile(const cv::Mat& tile, const cv::Rect& tileRegion) const {
    tile.copyTo(_image(tileRegion));
}
