#pragma once

#include "../../utils/remote-desk/SenderInitializer.hpp"

class Sender {
    std::unique_ptr<ThreadSafeQueue<std::vector<byte>>> _frames;
    std::unique_ptr<UDPSenderWorker> _udpSenderWorker;
    std::unique_ptr<ScreenCaptureWorker> _screenCaptureWorker;
    std::unique_ptr<RemoteEventExecutor>  _eventHandler;
public:
    Sender(const u_short tcpServerPort, const SIZE& targetSize, const int fps, const int maxDelayMs);

    void run() const;
    void stop() const;
};
