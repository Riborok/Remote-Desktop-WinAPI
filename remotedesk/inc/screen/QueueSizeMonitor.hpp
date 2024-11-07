#pragma once
#include "../dt/ThreadSafeQueue.hpp"

template <typename T>
class QueueSizeMonitor {
    ThreadSafeQueue<T>& _queue;
    int _fps;
    int _maxDelayMs;
    size_t _maxItems;
public:
    QueueSizeMonitor(ThreadSafeQueue<T>& queue, const int fps, const int maxDelayMs);
    void maintainQueueSize();

    ~QueueSizeMonitor() = default;
    QueueSizeMonitor(QueueSizeMonitor&&) = delete;
    QueueSizeMonitor& operator=(QueueSizeMonitor&&) = delete;
    QueueSizeMonitor(const QueueSizeMonitor&) = delete;
    QueueSizeMonitor& operator=(const QueueSizeMonitor&) = delete;
private:
    size_t calcMaxItems() const;
};

template <typename T>
QueueSizeMonitor<T>::QueueSizeMonitor(ThreadSafeQueue<T>& queue, const int fps, const int maxDelayMs):
    _queue(queue), _fps(fps), _maxDelayMs(maxDelayMs),
    _maxItems(calcMaxItems()) { }

template <typename T>
void QueueSizeMonitor<T>::maintainQueueSize() {
    const size_t size = _queue.size();
    if (size > _maxItems) {
        const size_t excess = size - _maxItems;
        _queue.trimQueue(excess);
    }
}

template <typename T>
size_t QueueSizeMonitor<T>::calcMaxItems() const {
    return static_cast<size_t>(_fps * _maxDelayMs / 1000.0);
}
