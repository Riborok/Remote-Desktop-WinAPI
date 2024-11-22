#pragma once

#include <windows.h>
#include <opencv2/opencv.hpp>

class ImageUtils {
    static constexpr int RGBA_IMAGE_TYPE = CV_8UC4;
public:
    ImageUtils() = delete;

    static std::vector<byte> scaleImageBuffer(const std::vector<byte>& srcBuffer, const SIZE& srcSize, const SIZE& targetSize);
    static cv::Mat createBlackImage(const SIZE& size);
    static cv::Mat createImageFromBuffer(byte* buffer, const SIZE& size);
    static cv::Mat resizeImage(const cv::Mat& srcImage, const SIZE& targetSize);
    static std::vector<byte> resizeImageToBuffer(const cv::Mat& srcImage, const SIZE& targetSize);
    static std::vector<byte> copyToVector(const cv::Mat& image);
    static cv::Mat convertToRGBA(cv::Mat&& srcImage);
};
