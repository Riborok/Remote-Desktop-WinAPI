#include "../../../inc/screen/render/ScreenRender.hpp"

#include "../../../inc/utils/factory/bitmap/BitmapFactory.hpp"
#include "../../../inc/utils/img/ImageUtils.hpp"
#include "../../../inc/utils/screen/SizeUtils.hpp"

ScreenRender::ScreenRender(const HWND hWnd, const SIZE& appSize, const SIZE& originalSize):
        _dc(hWnd), _appSize(appSize), _originalSize(originalSize),
        _bitmap(BitmapFactory::createDDBitmap(_dc.getHDC(), _appSize)) { }

void ScreenRender::render(const std::vector<byte>& screen) {
    std::lock_guard<std::mutex> lock(_appSizeMutex);
    _bitmap.setDIBits(_originalSize == _appSize
        ? screen
        : ImageUtils::scaleImageBuffer(screen, _originalSize, _appSize)
    );
    _bitmap.copyTo(_dc.getHDC());
}

void ScreenRender::updateAppSize(const SIZE& appSize) {
    std::lock_guard<std::mutex> lock(_appSizeMutex);
    _appSize = appSize;
    _bitmap = BitmapFactory::createDDBitmap(_dc.getHDC(), _appSize);
}
