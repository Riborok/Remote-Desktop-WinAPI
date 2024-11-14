#pragma once

#include <vector>

#include "../../types/byte.hpp"

class ByteArrayUtils {
public:
    template <typename T>
    static void setValue(std::vector<byte>& byteArray, const size_t elementIndex, const T value);

    template <typename T>
    static void setValue(byte* byteArray, const size_t elementIndex, const T value);

    template <typename T>
    static T getValue(const std::vector<byte>& byteArray, const size_t elementIndex);

    template <typename T>
    static T getValue(const byte* byteArray, const size_t elementIndex);
    
    template <typename T>
    static void setValueByByteIndex(std::vector<byte>& byteArray, const size_t byteIndex, const T value);

    template <typename T>
    static void setValueByByteIndex(byte* byteArray, const size_t byteIndex, const T value);

    template <typename T>
    static T getValueByByteIndex(const std::vector<byte>& byteArray, const size_t byteIndex);

    template <typename T>
    static T getValueByByteIndex(const byte* byteArray, const size_t byteIndex);
};

template <typename T>
void ByteArrayUtils::setValue(std::vector<byte>& byteArray, const size_t elementIndex, const T value) {
    setValue<T>(byteArray.data(), elementIndex, value);
}

template <typename T>
void ByteArrayUtils::setValue(byte* byteArray, const size_t elementIndex, const T value) {
    *(reinterpret_cast<T*>(byteArray) + elementIndex) = value;
}

template <typename T>
T ByteArrayUtils::getValue(const std::vector<byte>& byteArray, const size_t elementIndex) {
    return getValue<T>(byteArray.data(), elementIndex);
}

template <typename T>
T ByteArrayUtils::getValue(const byte* byteArray, const size_t elementIndex) {
    return *(reinterpret_cast<const T*>(byteArray) + elementIndex);
}

template <typename T>
void ByteArrayUtils::setValueByByteIndex(std::vector<byte>& byteArray, const size_t byteIndex, const T value) {
    setValueByByteIndex<T>(byteArray.data(), byteIndex, value);
}

template <typename T>
void ByteArrayUtils::setValueByByteIndex(byte* byteArray, const size_t byteIndex, const T value) {
    *reinterpret_cast<T*>(byteArray + byteIndex) = value;
}

template <typename T>
T ByteArrayUtils::getValueByByteIndex(const std::vector<byte>& byteArray, const size_t byteIndex) {
    return getValueByByteIndex<T>(byteArray.data(), byteIndex);
}

template <typename T>
T ByteArrayUtils::getValueByByteIndex(const byte* byteArray, const size_t byteIndex) {
    return *reinterpret_cast<const T*>(byteArray + byteIndex);
}

