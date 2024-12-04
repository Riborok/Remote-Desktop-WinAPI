#include "../../../inc/utils/screen/FrameUtils.hpp"

double FrameUtils::calcFrames(const int fps, const int maxDelayMs) {
    return fps * maxDelayMs / 1000.0;
}

double FrameUtils::calcFrameDuration(const int fps) {
    return 1000.0 / fps;
}
