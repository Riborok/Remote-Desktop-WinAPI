#pragma once

#include "../../img/ImageConfig.hpp"
#include "../../utils/remote-desk/SenderInitializer.hpp"
#include "../../utils/screen/ScreenUtils.hpp"
#include "../../../src/remote-desk/sender/SenderCfg.hpp"

class Sender {
    std::unique_ptr<ThreadSafeQueue<std::vector<byte>>> _frames;
    std::unique_ptr<UDPSenderWorker> _udpSenderWorker;
    std::unique_ptr<ScreenCaptureWorker> _screenCaptureWorker;
    std::unique_ptr<RemoteEventExecutor>  _eventHandler;
public:
    Sender(const SenderCfg& cfg);

    void run() const;
    void stop() const;
    void updateFPSAndMaxDelay(const int fps, const int maxDelayMs) const;
    void updateImageConfig(const ImageConfig &ic) const;
};
