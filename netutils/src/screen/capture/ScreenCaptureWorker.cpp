#include "../../../inc/screen/capture/ScreenCaptureWorker.hpp"

ScreenCaptureWorker::ScreenCaptureWorker(ThreadSafeQueue<std::vector<BYTE>>& queue, const SIZE& targetSize,
        const int fps):
    _queue(queue), _targetSize(targetSize), _fps(fps) { }

void ScreenCaptureWorker::start() {
    _running = true;
    _workerThread = std::thread(&ScreenCaptureWorker::captureLoop, this);
}

void ScreenCaptureWorker::stop() {
    _running = false;
    if (_workerThread.joinable()) {
        _workerThread.join();
    }
}

ScreenCaptureWorker::~ScreenCaptureWorker() {
    stop();
}

void ScreenCaptureWorker::captureLoop() const {
    ScreenCapture screenCapture(_targetSize);
    const auto frameDuration = std::chrono::milliseconds(1000 / _fps);
    while (_running) {
        auto start = std::chrono::steady_clock::now();
        _queue.enqueue(std::make_unique<std::vector<BYTE>>(screenCapture.capture()));
        std::this_thread::sleep_until(start + frameDuration);
    }
}
