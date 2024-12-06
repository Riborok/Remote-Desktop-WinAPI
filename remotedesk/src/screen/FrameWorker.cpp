#include "../../inc/screen/FrameWorker.hpp"

#include "../../inc/utils/screen/FrameUtils.hpp"

FrameWorker::FrameWorker(ThreadSafeQueue<std::vector<byte>>& queue, const int fps, const int maxDelayMs):
        _frames(queue), _queueSizeMonitor(_frames, fps, maxDelayMs) {
    setFrameDuration(fps);
}

void FrameWorker::updateFPSAndMaxDelay(const int fps, const int maxDelayMs) {
    _queueSizeMonitor.setMaxFrames(fps, maxDelayMs);
    setFrameDuration(fps);
}

void FrameWorker::setFrameDuration(const int fps) {
    _frameDuration = static_cast<long long>(FrameUtils::calcFrameDuration(fps));
}

void FrameWorker::eventLoop() {
    while (_running) {
        _queueSizeMonitor.maintainQueueSize();
        auto frameDuration = std::chrono::milliseconds(_frameDuration);
        auto start = std::chrono::steady_clock::now();
        process();
        std::this_thread::sleep_until(start + frameDuration);
    }
}

ThreadSafeQueue<std::vector<byte>>& FrameWorker::getFrames() const {
    return _frames;
}
