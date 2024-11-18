#pragma once

#include <type_traits>
#include <windows.h>

class PointScaler {
    double _scaleX;
    double _scaleY;
public:
    PointScaler(const SIZE& originalSize, const SIZE& targetSize);
    PointScaler(const POINT& originalPoint, const POINT& targetPoint);
    SIZE scaleSize(const SIZE& size) const;
    POINT scalePoint(const POINT& point) const;
    
    template <typename T>
    auto scaleByX(T value) const -> std::enable_if_t<std::is_arithmetic_v<T>, T>;

    template <typename T>
    auto scaleByY(T value) const -> std::enable_if_t<std::is_arithmetic_v<T>, T>;
};

template <typename T>
auto PointScaler::scaleByX(T value) const -> std::enable_if_t<std::is_arithmetic_v<T>, T> {
    return static_cast<T>(value * _scaleX);
}

template <typename T>
auto PointScaler::scaleByY(T value) const -> std::enable_if_t<std::is_arithmetic_v<T>, T> {
    return static_cast<T>(value * _scaleY);
}
