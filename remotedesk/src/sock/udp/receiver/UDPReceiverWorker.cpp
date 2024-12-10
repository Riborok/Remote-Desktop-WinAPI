#include "../../../../inc/sock/udp/receiver/UDPReceiverWorker.hpp"

UDPReceiverWorker::UDPReceiverWorker(ThreadSafeQueue<std::vector<byte>>& buffer, UDPReceiver&& receiver):
    _buffer(buffer), _receiver(std::move(receiver)) { }

DataReassembler& UDPReceiverWorker::getDataReassembler() const {
    return _receiver.getDataReassembler();
}

void UDPReceiverWorker::eventLoop() {
    while (_running) {
        if (auto buffer = _receiver.receive(); !buffer.empty()) {
            _buffer.enqueue(std::make_unique<std::vector<byte>>(std::move(buffer)));
        }
    }
}
