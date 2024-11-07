#pragma once
#include "../dt/ThreadSafeQueue.hpp"

template <typename T>
class QueueSizeMonitor {
    ThreadSafeQueue<T>& _queue;
    size_t _maxItems;
public:
    QueueSizeMonitor(ThreadSafeQueue<T>& queue, const int fps, const int maxDelayMs);
    void maintainQueueSize();

    ~QueueSizeMonitor() = default;
    QueueSizeMonitor(QueueSizeMonitor&&) = delete;
    QueueSizeMonitor& operator=(QueueSizeMonitor&&) = delete;
    QueueSizeMonitor(const QueueSizeMonitor&) = delete;
    QueueSizeMonitor& operator=(const QueueSizeMonitor&) = delete;
};

template <typename T>
QueueSizeMonitor<T>::QueueSizeMonitor(ThreadSafeQueue<T>& queue, const int fps, const int maxDelayMs):
    _queue(queue), _maxItems(static_cast<size_t>(fps * maxDelayMs / 1000.0)) { }

template <typename T>
void QueueSizeMonitor<T>::maintainQueueSize() {
    const size_t size = _queue.size();
    if (size > _maxItems) {
        const size_t excess = size - _maxItems;
        _queue.trimQueue(excess);
    }
}
