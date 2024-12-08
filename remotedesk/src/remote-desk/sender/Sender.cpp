#include "../../../inc/remote-desk/sender/Sender.hpp"

#include "../../../inc/sock/udp/data-fragmenter/ImgCodecSecureFragmenter.hpp"

Sender::Sender(const u_short tcpServerPort, const ImageConfig& ic, const int fps, const int maxDelayMs, const SIZE& targetSize) {
    _frames = SenderInitializer::createFrames();
    auto connection = SenderInitializer::createConnection(tcpServerPort);
    const u_short port = SenderInitializer::sendSizeReceivePort(*connection, targetSize);
    _udpSenderWorker = SenderInitializer::createUDPSenderWorker(*_frames, *connection, port, targetSize, ic);
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

void Sender::updateFPSAndMaxDelay(const int fps, const int maxDelayMs) const {
    _screenCaptureWorker->updateFPSAndMaxDelay(fps, maxDelayMs);
}

void Sender::updateImageConfig(const ImageConfig& ic) const {
    DataFragmenter& df = _udpSenderWorker->getDataFragmenter();
    ImgCodecSecureFragmenter* ptr = dynamic_cast<ImgCodecSecureFragmenter*>(&df);
    if (ptr != nullptr) {
        ptr->updateImageConfig(ic);
    }
}
