#include "../../../inc/utils/screen/Scaler.hpp"

Scaler::Scaler(const SIZE& originalSize, const SIZE& targetSize):
    _scaleX(static_cast<double>(targetSize.cx) / originalSize.cx),
    _scaleY(static_cast<double>(targetSize.cy) / originalSize.cy) { }

Scaler::Scaler(const POINT& originalPoint, const POINT& targetPoint):
    _scaleX(static_cast<double>(targetPoint.x) / originalPoint.x),
    _scaleY(static_cast<double>(targetPoint.y) / originalPoint.y) { }

SIZE Scaler::scaleSize(const SIZE& size) const {
    return { scaleByX(size.cx), scaleByY(size.cy) };
}

POINT Scaler::scalePoint(const POINT& point) const {
    return { scaleByX(point.x), scaleByY(point.y) };
}
