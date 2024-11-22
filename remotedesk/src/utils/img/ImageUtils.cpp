#include "../../../inc/utils/img/ImageUtils.hpp"

std::vector<byte> ImageUtils::scaleImageBuffer(const std::vector<byte>& srcBuffer, const SIZE& srcSize, const SIZE& targetSize) {
    const cv::Mat srcImage = ImageUtils::createImageFromBuffer(const_cast<byte*>(srcBuffer.data()), srcSize);
    return ImageUtils::resizeImageToBuffer(srcImage, targetSize);
}

cv::Mat ImageUtils::createBlackImage(const SIZE& size) {
    return cv::Mat::zeros(size.cy, size.cx, RGBA_IMAGE_TYPE);
}

cv::Mat ImageUtils::createImageFromBuffer(byte* buffer, const SIZE& size) {
    return {size.cy, size.cx, RGBA_IMAGE_TYPE, buffer};
}

cv::Mat ImageUtils::resizeImage(const cv::Mat& srcImage, const SIZE& targetSize) {
    cv::Mat result;
    cv::resize(srcImage, result, {targetSize.cx, targetSize.cy}, 0, 0, cv::INTER_NEAREST);
    return result;
}

std::vector<byte> ImageUtils::resizeImageToBuffer(const cv::Mat& srcImage, const SIZE& targetSize) {
    std::vector<byte> result(srcImage.elemSize() * targetSize.cx * targetSize.cy);
    cv::Mat resultMat = ImageUtils::createImageFromBuffer(result.data(), targetSize);
    cv::resize(srcImage, resultMat, {targetSize.cx, targetSize.cy}, 0, 0, cv::INTER_NEAREST);
    return result;
}

std::vector<byte> ImageUtils::copyToVector(const cv::Mat& image) {
    return {image.datastart, image.dataend};
}

cv::Mat ImageUtils::convertToRGBA(cv::Mat&& srcImage) {
    if (srcImage.type() == RGBA_IMAGE_TYPE) {
        return std::move(srcImage);
    }

    cv::Mat result;
    if (srcImage.channels() == 3) {
        cv::cvtColor(srcImage, result, cv::COLOR_BGR2BGRA);
    } else if (srcImage.channels() == 1) {
        cv::cvtColor(srcImage, result, cv::COLOR_GRAY2BGRA);
    }
    return result;
}
