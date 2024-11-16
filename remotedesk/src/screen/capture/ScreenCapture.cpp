#include "../../../inc/screen/capture/ScreenCapture.hpp"

#include "../../../inc/utils/factory/bitmap/BitmapFactory.hpp"
#include "../../../inc/utils/screen/ScreenUtils.hpp"

ScreenCapture::ScreenCapture(const SIZE& targetSize): _dc(nullptr),
        _originalSize(ScreenUtils::getScreenSize()),
        _scaledBitmap(BitmapFactory::createDDBitmap(_dc.getHDC(), targetSize)) {
    _scaledBitmap.enableHighQualityStretching();
}

std::vector<byte> ScreenCapture::capture() {
    _scaledBitmap.stretchFrom(_dc.getHDC(), _originalSize);
    if (const auto cursor = _cursorCapture.capture()) {
        _scaledBitmap.drawScaledIcon(*cursor, _originalSize);   
    }
    return _scaledBitmap.getDIBits();
}
