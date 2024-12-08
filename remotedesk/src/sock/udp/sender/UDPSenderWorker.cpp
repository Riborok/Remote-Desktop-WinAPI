#include "../../../../inc/sock/udp/sender/UDPSenderWorker.hpp"

UDPSenderWorker::UDPSenderWorker(ThreadSafeQueue<std::vector<byte>>& buffer, UDPSender&& sender):
    _buffer(buffer), _sender(std::move(sender)) { }

DataFragmenter& UDPSenderWorker::getDataFragmenter() const {
    return _sender.getDataFragmenter();
}

void UDPSenderWorker::eventLoop() {
    while (_running) {
        _sender.send(*_buffer.dequeue());
    }
}
