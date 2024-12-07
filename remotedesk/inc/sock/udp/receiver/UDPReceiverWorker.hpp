#pragma once

#include "./UDPReceiver.hpp"
#include "../../../thread/ThreadSafeQueue.hpp"
#include "../../../thread/ThreadWorker.hpp"

class UDPReceiverWorker final : public ThreadWorker {
    ThreadSafeQueue<std::vector<byte>>& _buffer;
    UDPReceiver _receiver;
public:
    UDPReceiverWorker(ThreadSafeQueue<std::vector<byte>>& buffer, UDPReceiver &&receiver);
protected:
    void eventLoop() override;
};
