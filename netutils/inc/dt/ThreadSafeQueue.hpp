#pragma once

#include <mutex>
#include <queue>

template<typename T>
class ThreadSafeQueue {
    std::queue<std::unique_ptr<T>> _queue;
    std::mutex _mutex;
    std::condition_variable _cond;
public:
    void enqueue(std::unique_ptr<T> item);
    std::unique_ptr<T> dequeue();
    void trimQueue(const size_t n);
    bool isEmpty();
private:
    std::unique_ptr<T> extractItem();
};

template <typename T>
void ThreadSafeQueue<T>::enqueue(std::unique_ptr<T> item) {
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push(std::move(item));
    _cond.notify_one();
}

template <typename T>
std::unique_ptr<T> ThreadSafeQueue<T>::dequeue() {
    std::unique_lock<std::mutex> lock(_mutex);
    _cond.wait(lock, [this] { return !_queue.empty(); });
    return extractItem();
}

template<typename T>
void ThreadSafeQueue<T>::trimQueue(const size_t n) {
    std::lock_guard<std::mutex> lock(_mutex);
    for (size_t i = 0; i < n && !_queue.empty(); ++i) {
        _queue.pop();
    }
}

template <typename T>
bool ThreadSafeQueue<T>::isEmpty() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _queue.empty();
}

template <typename T>
std::unique_ptr<T> ThreadSafeQueue<T>::extractItem() {
    auto item = std::move(_queue.front());
    _queue.pop();
    return item;
}