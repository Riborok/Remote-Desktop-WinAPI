#include "../../inc/thread/ThreadWorker.hpp"

void ThreadWorker::start() {
    if (!_running) {
        _running = true;
        _eventThread = std::thread(&ThreadWorker::eventLoop, this);
    }
}

void ThreadWorker::stop() {
    if (_running) {
        _running = false;
        if (_eventThread.joinable()) {
            _eventThread.join();
        }
    }
}

ThreadWorker::~ThreadWorker() {
    stop();
}
