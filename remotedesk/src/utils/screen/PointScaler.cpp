#include "../../../inc/utils/screen/PointScaler.hpp"

PointScaler::PointScaler(const SIZE& originalSize, const SIZE& targetSize):
    _scaleX(static_cast<double>(targetSize.cx) / originalSize.cx),
    _scaleY(static_cast<double>(targetSize.cy) / originalSize.cy) { }

PointScaler::PointScaler(const POINT& originalPoint, const POINT& targetPoint):
    _scaleX(static_cast<double>(targetPoint.x) / originalPoint.x),
    _scaleY(static_cast<double>(targetPoint.y) / originalPoint.y) { }

SIZE PointScaler::scaleSize(const SIZE& size) const {
    return { scaleByX(size.cx), scaleByY(size.cy) };
}

POINT PointScaler::scalePoint(const POINT& point) const {
    return { scaleByX(point.x), scaleByY(point.y) };
}
