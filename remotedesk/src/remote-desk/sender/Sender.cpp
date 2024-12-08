#include "../../../inc/remote-desk/sender/Sender.hpp"

#include "../../../inc/sock/udp/data-fragmenter/ImgCodecSecureFragmenter.hpp"

Sender::Sender(const SenderCfg& cfg) {
    _frames = SenderInitializer::createFrames();
    auto connection = SenderInitializer::createConnection(cfg.tcpServerPort);
    const u_short port = SenderInitializer::sendSizeReceivePort(*connection, cfg.targetSize);
    _udpSenderWorker = SenderInitializer::createUDPSenderWorker(*_frames, *connection, port, cfg.targetSize, cfg.imageConfig);
    _screenCaptureWorker = SenderInitializer::createScreenCaptureWorker(*_frames, cfg.targetSize, cfg.fps, cfg.maxDelayMs);
    _eventHandler = SenderInitializer::createRemoteEventExecutor(std::move(connection), cfg.targetSize);
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
