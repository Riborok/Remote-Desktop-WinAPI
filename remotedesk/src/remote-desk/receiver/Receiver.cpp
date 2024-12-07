#include "../../../inc/remote-desk/receiver/Receiver.hpp"

Receiver::Receiver(const HWND hWnd, const sockaddr_in& addr, const u_short udpPort, const int fps,
    const int maxDelayMs) {
    _frames = ReceiverInitializer::createFrames();
    auto connection = ReceiverInitializer::createConnection(addr);
    const SIZE receivedSize = ReceiverInitializer::sendPortReceiveSize(*connection, udpPort);
    _udpReceiverWorker = ReceiverInitializer::createUDPReceiverWorker(*_frames, *connection, udpPort, receivedSize);
    _screenRenderWorker = ReceiverInitializer::createScreenRenderWorker(*_frames, hWnd, receivedSize, fps, maxDelayMs);
    _eventSender = ReceiverInitializer::createEventSender(std::move(connection), receivedSize);
}

void Receiver::run() const {
    _screenRenderWorker->start();
    _udpReceiverWorker->start();
}

void Receiver::stop() const {
    _screenRenderWorker->stop();
    _udpReceiverWorker->stop();
}

void Receiver::updateAppSize(const SIZE& appSize) const {
    _eventSender->updateAppSize(appSize);
    _screenRenderWorker->updateAppSize(appSize);
}

void Receiver::updateFPSAndMaxDelay(const int fps, const int maxDelayMs) const {
    _screenRenderWorker->updateFPSAndMaxDelay(fps, maxDelayMs);
}

void Receiver::sendEvent(const UINT message, const WPARAM wParam, const LPARAM lParam) const {
    _eventSender->sendEvent(message, wParam, lParam);
}
