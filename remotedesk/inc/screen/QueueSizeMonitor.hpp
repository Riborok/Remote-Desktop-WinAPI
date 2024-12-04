#pragma once
#include "../dt/ThreadSafeQueue.hpp"
#include "../utils/screen/FrameUtils.hpp"

template <typename T>
class QueueSizeMonitor {
    ThreadSafeQueue<T>& _queue;
    std::atomic<size_t> _maxFrames;
public:
    QueueSizeMonitor(ThreadSafeQueue<T>& queue, const int fps, const int maxDelayMs);
    void setMaxFrames(const int fps, const int maxDelayMs);
    void maintainQueueSize();

    ~QueueSizeMonitor() = default;
    QueueSizeMonitor(QueueSizeMonitor&&) = delete;
    QueueSizeMonitor& operator=(QueueSizeMonitor&&) = delete;
    QueueSizeMonitor(const QueueSizeMonitor&) = delete;
    QueueSizeMonitor& operator=(const QueueSizeMonitor&) = delete;
};

template <typename T>
QueueSizeMonitor<T>::QueueSizeMonitor(ThreadSafeQueue<T>& queue, const int fps, const int maxDelayMs):
    _queue(queue) {
    setMaxFrames(fps, maxDelayMs);
}

template <typename T>
void QueueSizeMonitor<T>::setMaxFrames(const int fps, const int maxDelayMs) {
    _maxFrames = static_cast<size_t>(FrameUtils::calcFrames(fps, maxDelayMs));
}

template <typename T>
void QueueSizeMonitor<T>::maintainQueueSize() {
    const size_t size = _queue.size();
    if (size > _maxFrames) {
        const size_t excess = size - _maxFrames;
        _queue.trimQueue(excess);
    }
}
