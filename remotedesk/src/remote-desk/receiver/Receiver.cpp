#include "../../../inc/remote-desk/receiver/Receiver.hpp"

#include "../../../inc/utils/sock/SockaddrUtils.hpp"

Receiver::Receiver(const HWND hWnd, const ReceiverConfig& config) {
    _frames = ReceiverInitializer::createFrames();
    auto connection = ReceiverInitializer::createConnection(SockaddrUtils::createAddr(config.serverIp, config.serverPort));
    const SIZE receivedSize = ReceiverInitializer::sendPortReceiveSize(*connection, config.udpPort);
    _udpReceiverWorker = ReceiverInitializer::createUDPReceiverWorker(*_frames, *connection, config.udpPort, receivedSize);
    _screenRenderWorker = ReceiverInitializer::createScreenRenderWorker(*_frames, hWnd, receivedSize, config.fps, config.maxDelayMs);
    _eventSender = ReceiverInitializer::createEventSender(std::move(connection), receivedSize);
}

void Receiver::run() const {
    _screenRenderWorker->start();
    _udpReceiverWorker->start();
}

void Receiver::stop() const {
    _udpReceiverWorker->stop();
    _screenRenderWorker->stop();
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
