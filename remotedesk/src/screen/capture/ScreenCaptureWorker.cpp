#include "../../../inc/screen/capture/ScreenCaptureWorker.hpp"

ScreenCaptureWorker::ScreenCaptureWorker(ThreadSafeQueue<std::vector<byte>>& queue,
    const SIZE& targetSize, const int fps): Worker(fps), _queue(queue), _screenCapture(targetSize) { }

void ScreenCaptureWorker::process() {
    _queue.enqueue(std::make_unique<std::vector<byte>>(_screenCapture.capture()));
}
