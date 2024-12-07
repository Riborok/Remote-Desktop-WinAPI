#pragma once

#include "./UDPSender.hpp"
#include "../../../thread/ThreadSafeQueue.hpp"
#include "../../../thread/ThreadWorker.hpp"

class UDPSenderWorker final : public ThreadWorker {
    ThreadSafeQueue<std::vector<byte>>& _buffer;
    UDPSender _sender;
public:
    UDPSenderWorker(ThreadSafeQueue<std::vector<byte>>& buffer, UDPSender &&sender);
protected:
    void eventLoop() override;
};
