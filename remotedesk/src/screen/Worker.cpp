#include "../../inc/screen/Worker.hpp"

#include "../../inc/utils/screen/FrameUtils.hpp"

Worker::Worker(ThreadSafeQueue<std::vector<byte>>& queue, const int fps, const int maxDelayMs):
        _queue(queue), _queueSizeMonitor(_queue, fps, maxDelayMs) {
    setFrameDuration(fps);
}

void Worker::start() {
    _running = true;
    _workerThread = std::thread(&Worker::workLoop, this);
}

void Worker::stop() {
    _running = false;
    if (_workerThread.joinable()) {
        _workerThread.join();
    }
}

void Worker::updateFPSAndMaxDelay(const int fps, const int maxDelayMs) {
    _queueSizeMonitor.setMaxFrames(fps, maxDelayMs);
    setFrameDuration(fps);
}

void Worker::setFrameDuration(const int fps) {
    _frameDuration = static_cast<long long>(FrameUtils::calcFrameDuration(fps));
}

ThreadSafeQueue<std::vector<byte>>& Worker::getQueue() const {
    return _queue;
}

Worker::~Worker() {
    stop();
}

void Worker::workLoop() {
    while (_running) {
        _queueSizeMonitor.maintainQueueSize();
        auto frameDuration = std::chrono::milliseconds(_frameDuration);
        auto start = std::chrono::steady_clock::now();
        process();
        std::this_thread::sleep_until(start + frameDuration);
    }
}
