#include "../../../inc/screen/capture/ScreenCaptureWorker.hpp"

ScreenCaptureWorker::ScreenCaptureWorker(ThreadSafeQueue<std::vector<byte>>& queue,
        const SIZE& targetSize, const int fps, const int maxDelayMs) :
    Worker(queue, fps, maxDelayMs), _screenCapture(targetSize) { }

void ScreenCaptureWorker::process() {
    getQueue().enqueue(std::make_unique<std::vector<byte>>(_screenCapture.capture()));
}
