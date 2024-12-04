#pragma once

class FrameUtils {
public:
    FrameUtils() = delete;

    static double calcFrames(const int fps, const int maxDelayMs);
    static double calcFrameDuration(const int fps);
};
