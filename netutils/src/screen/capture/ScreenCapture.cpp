#include "../../../inc/screen/capture/ScreenCapture.hpp"

#include "../../../inc/screen/bitmap/BitmapFactory.hpp"
#include "../../../inc/utils/screen/ScreenUtils.hpp"

ScreenCapture::ScreenCapture(const Size& targetSize): _dc(nullptr),
        _fullScreenBitmap(BitmapFactory::createDDBitmap(_dc.getHDC(), ScreenUtils::getScreenSize())),
        _scaledBitmap(BitmapFactory::createDDBitmap(_dc.getHDC(), targetSize)) {
    _scaledBitmap.enableHighQualityStretching();
}

std::vector<BYTE> ScreenCapture::capture() {
    _fullScreenBitmap.copyFrom(_dc.getHDC());
    _fullScreenBitmap.drawCursor(ScreenUtils::getCursor());
    _scaledBitmap.stretchFrom(_fullScreenBitmap);
    return _scaledBitmap.getDIBits();
}
