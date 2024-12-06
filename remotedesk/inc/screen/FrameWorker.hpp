#pragma once

#include "../thread/ThreadWorker.hpp"
#include "QueueSizeMonitor.hpp"
#include "../types/byte.hpp"

class FrameWorker : public ThreadWorker {
    ThreadSafeQueue<std::vector<byte>>& _frames;
    QueueSizeMonitor<std::vector<byte>> _queueSizeMonitor;
    
    std::atomic<long long> _frameDuration;
protected:
    explicit FrameWorker(ThreadSafeQueue<std::vector<byte>>& queue, const int fps, const int maxDelayMs);
public:
    void updateFPSAndMaxDelay(const int fps, const int maxDelayMs);
protected:
    void eventLoop() override;
    virtual void process() = 0;
    ThreadSafeQueue<std::vector<byte>>& getFrames() const;
private:
    void setFrameDuration(const int fps);
};
