#include "../../../inc/screen/render/ScreenRender.hpp"

#include "../../../inc/screen/bitmap/BitmapFactory.hpp"
#include "../../../inc/utils/screen/ScreenUtils.hpp"

ScreenRender::ScreenRender(const HWND hWnd, Size& appSize, const Size& receivedSize):
        _dc(hWnd), _appSize(appSize),
        _diBitmap(BitmapFactory::createDIBitmap(_dc.getHDC(), receivedSize)) {
    ScreenUtils::enableHighQualityStretching(_dc.getHDC());
}

void ScreenRender::render(const MaskedData& maskedData) {
    _diBitmap.setDIBits(maskedData.getMask());
    _diBitmap.stretchTo(_dc.getHDC(), _appSize, SRCAND);
    _diBitmap.setDIBits(maskedData.getData());
    _diBitmap.stretchTo(_dc.getHDC(), _appSize, SRCINVERT);
}
