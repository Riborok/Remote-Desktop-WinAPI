#include "../../../inc/screen/capture/ScreenCaptureWorker.hpp"

ScreenCaptureWorker::ScreenCaptureWorker(ThreadSafeQueue<std::vector<BYTE>>& queue,
    const SIZE& targetSize, const int fps): Worker(fps), _queue(queue), _screenCapture(targetSize) { }

void ScreenCaptureWorker::process() {
    _queue.enqueue(std::make_unique<std::vector<BYTE>>(_screenCapture.capture()));
}
