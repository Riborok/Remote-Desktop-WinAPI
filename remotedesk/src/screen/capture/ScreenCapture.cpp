#include "../../../inc/screen/capture/ScreenCapture.hpp"

#include "../../../inc/utils/factory/bitmap/BitmapFactory.hpp"
#include "../../../inc/utils/screen/ImageScaler.hpp"
#include "../../../inc/utils/screen/ScreenUtils.hpp"
#include "../../../inc/utils/screen/SizeUtils.hpp"

ScreenCapture::ScreenCapture(const SIZE& targetSize): _dc(nullptr),
        _desktopSize(ScreenUtils::getScreenSize()),
        _targetSize(targetSize),
        _bitmap(BitmapFactory::createDDBitmap(_dc.getHDC(), _desktopSize)) { }

std::vector<byte> ScreenCapture::capture() {
    _bitmap.copyFrom(_dc.getHDC());
    if (const auto cursor = _cursorCapture.capture()) {
        _bitmap.drawIcon(*cursor);   
    }
    return _desktopSize == _targetSize
        ? _bitmap.getDIBits()
        : ImageScaler::scaleImage(_bitmap.getDIBits(), _desktopSize, _targetSize);
}
