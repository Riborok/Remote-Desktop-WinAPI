#include "../../inc/screen/Worker.hpp"

#include "../../inc/utils/screen/FrameUtils.hpp"

Worker::Worker(const int fps) {
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

void Worker::setFrameDuration(const int fps) {
    _frameDuration = static_cast<long long>(FrameUtils::calcFrameDuration(fps));
}

Worker::~Worker() {
    stop();
}

void Worker::workLoop() {
    while (_running) {
        auto frameDuration = std::chrono::milliseconds(_frameDuration);
        auto start = std::chrono::steady_clock::now();
        process();
        std::this_thread::sleep_until(start + frameDuration);
    }
}
