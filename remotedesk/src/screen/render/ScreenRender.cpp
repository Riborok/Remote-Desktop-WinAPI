#include "../../../inc/screen/render/ScreenRender.hpp"

#include "../../../inc/utils/factory/bitmap/BitmapFactory.hpp"
#include "../../../inc/utils/screen/ScreenUtils.hpp"
#include "../../../inc/utils/screen/SizeUtils.hpp"

ScreenRender::ScreenRender(const HWND hWnd, const SIZE& appSize, const SIZE& receivedSize):
        _dc(hWnd), _appSize(appSize),
        _bitmap(BitmapFactory::createDDBitmap(_dc.getHDC(), receivedSize)) {
    ScreenUtils::enableHighQualityStretching(_dc.getHDC());
}

void ScreenRender::render(const std::vector<byte>& screen) const {
    _bitmap.setDIBits(screen);
    _bitmap.stretchedCopyTo(_dc.getHDC(), _appSize);
}
