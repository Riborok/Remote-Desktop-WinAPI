#include "../../inc/screen/Worker.hpp"

Worker::Worker(const int fps): _frameDuration(1000 / fps) {}

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

Worker::~Worker() {
    stop();
}

void Worker::workLoop() {
    while (_running) {
        auto start = std::chrono::steady_clock::now();
        process();
        std::this_thread::sleep_until(start + _frameDuration);
    }
}
