#include "../../../inc/screen/render/ScreenRender.hpp"

#include "../../../inc/utils/factory/bitmap/BitmapFactory.hpp"
#include "../../../inc/utils/screen/SizeUtils.hpp"

ScreenRender::ScreenRender(const HWND hWnd, const SIZE& appSize, const SIZE& receivedSize):
        _dc(hWnd), _appSize(appSize),
        _diBitmap(BitmapFactory::createDIBitmap(_dc.getHDC(), receivedSize)),
        _buffer(BitmapFactory::createDDBitmap(_dc.getHDC(), _appSize)) {
    _buffer.enableHighQualityStretching();
}

void ScreenRender::render(const MaskedData& maskedData) {
    ensureBufferMatchesAppSize();
    
    _buffer.copyFrom(_dc.getHDC());
    _diBitmap.setDIBits(maskedData.getMask());
    _diBitmap.stretchTo(_buffer, SRCAND);
    _diBitmap.setDIBits(maskedData.getData());
    _diBitmap.stretchTo(_buffer, SRCINVERT);
    _buffer.copyTo(_dc.getHDC());
}

void ScreenRender::ensureBufferMatchesAppSize() {
    if (_buffer.getSize() != _appSize) {
        _buffer = BitmapFactory::createDDBitmap(_dc.getHDC(), _appSize);
        _buffer.enableHighQualityStretching();
    }
}
