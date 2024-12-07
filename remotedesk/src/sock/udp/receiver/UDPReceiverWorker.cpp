#include "../../../../inc/sock/udp/receiver/UDPReceiverWorker.hpp"

UDPReceiverWorker::UDPReceiverWorker(ThreadSafeQueue<std::vector<byte>>& buffer, UDPReceiver&& receiver):
    _buffer(buffer), _receiver(std::move(receiver)) { }

void UDPReceiverWorker::eventLoop() {
    while (_running) {
        _buffer.enqueue(std::make_unique<std::vector<byte>>(_receiver.receive()));
    }
}
