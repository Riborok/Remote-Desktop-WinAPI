﻿#include "../../../inc/remote-desk/sender/Sender.hpp"

#include "../../../inc/sock/udp/data-fragmenter/ImgCodecSecureFragmenter.hpp"

Sender::Sender(const SenderConfig& config) {
    _frames = SenderInitializer::createFrames();
    auto connection = SenderInitializer::createConnection(config.tcpServerPort);
    const u_short port = SenderInitializer::sendSizeReceivePort(*connection, config.targetSize);
    _udpSenderWorker = SenderInitializer::createUDPSenderWorker(*_frames, *connection, port, config.targetSize, config.imageConfig);
    _screenCaptureWorker = SenderInitializer::createScreenCaptureWorker(*_frames, config.targetSize, config.fps, config.maxDelayMs);
    _eventHandler = SenderInitializer::createRemoteEventExecutor(std::move(connection), config.targetSize);
}

void Sender::setDisconnectCallback(std::function<void()>&& callback) const {
    _eventHandler->setDisconnectCallback(std::move(callback));
}

void Sender::run() const {
    _screenCaptureWorker->start();
    _udpSenderWorker->start();
    _eventHandler->start();
}

void Sender::stop() const {
    ThreadWorker* threads[] = {_udpSenderWorker.get(), _eventHandler.get(), _screenCaptureWorker.get()};
    for (const auto& thread : threads) {
        thread->stopRunning();
    }
    _frames->notifyAll();
    for (const auto& thread : threads) {
        thread->waitForThread();
    }
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
