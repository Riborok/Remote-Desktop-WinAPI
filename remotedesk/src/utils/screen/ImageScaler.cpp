// ReSharper disable CppCStyleCast
#include "../../../inc/utils/screen/ImageScaler.hpp"

#include <opencv2/opencv.hpp>

namespace {
    cv::Mat createSrcImage(const std::vector<byte>& src, const SIZE& srcSize);
    cv::Mat createDestImage(const cv::Mat& srcImage, const SIZE& targetSize);
    cv::Size toCvSize(const SIZE& sz);
    std::vector<byte> toVector(const cv::Mat& image);
}

std::vector<byte> ImageScaler::scaleImage(const std::vector<byte>& src, const SIZE& srcSize, const SIZE& targetSize) {
    const cv::Mat srcImage = createSrcImage(src, srcSize);
    const cv::Mat destImage = createDestImage(srcImage, targetSize);
    return toVector(destImage);
}

namespace {
    cv::Mat createSrcImage(const std::vector<byte>& src, const SIZE& srcSize) {
        return {toCvSize(srcSize), CV_8UC4, const_cast<void*>(static_cast<const void*>(src.data()))};
    }

    cv::Mat createDestImage(const cv::Mat& srcImage, const SIZE& targetSize) {
        cv::Mat destImage;
        resize(srcImage, destImage, toCvSize(targetSize), 0, 0, cv::INTER_NEAREST);
        return destImage;
    }
    
    cv::Size toCvSize(const SIZE& sz) {
        return {sz.cx, sz.cy};
    }

    std::vector<byte> toVector(const cv::Mat& image) {
        std::vector<byte> dest(image.total() * image.elemSize());
        std::memcpy(dest.data(), image.data, dest.size());
        return dest;
    }
}