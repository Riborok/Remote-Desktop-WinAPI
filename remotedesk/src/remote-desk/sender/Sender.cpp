#include "../../../inc/remote-desk/sender/Sender.hpp"

Sender::Sender(const u_short tcpServerPort, const SIZE& targetSize, const int fps, const int maxDelayMs) {
    _frames = SenderInitializer::createFrames();
    auto connection = SenderInitializer::createConnection(tcpServerPort);
    const u_short port = SenderInitializer::sendSizeReceivePort(*connection, targetSize);
    _udpSenderWorker = SenderInitializer::createUDPSenderWorker(*_frames, *connection, port, targetSize);
    _screenCaptureWorker = SenderInitializer::createScreenCaptureWorker(*_frames, targetSize, fps, maxDelayMs);
    _eventHandler = SenderInitializer::createRemoteEventExecutor(std::move(connection), targetSize);
}

void Sender::run() const {
    _screenCaptureWorker->start();
    _udpSenderWorker->start();
    _eventHandler->start();
}

void Sender::stop() const {
    _screenCaptureWorker->stop();
    _udpSenderWorker->stop();
    _eventHandler->stop();
}
