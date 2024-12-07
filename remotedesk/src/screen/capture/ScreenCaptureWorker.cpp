#include "../../../inc/screen/capture/ScreenCaptureWorker.hpp"

ScreenCaptureWorker::ScreenCaptureWorker(ThreadSafeQueue<std::vector<byte>>& frames,
        const SIZE& targetSize, const int fps, const int maxDelayMs) :
    FrameWorker(frames, fps, maxDelayMs), _screenCapture(targetSize) { }

void ScreenCaptureWorker::process() {
    getFrames().enqueue(std::make_unique<std::vector<byte>>(_screenCapture.capture()));
}
