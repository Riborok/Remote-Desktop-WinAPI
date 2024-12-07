#pragma once

#include "../../utils/remote-desk/ReceiverInitializer.hpp"

class Receiver {
    std::unique_ptr<ThreadSafeQueue<std::vector<byte>>> _frames;
    std::unique_ptr<UDPReceiverWorker> _udpReceiverWorker;
    std::unique_ptr<ScreenRenderWorker> _screenRenderWorker;
    std::unique_ptr<EventSender> _eventSender;
public:
    Receiver(const HWND hWnd, const sockaddr_in& addr, const u_short udpPort, const int fps, const int maxDelayMs);

    void run() const;
    void stop() const;
    void updateAppSize(const SIZE& appSize) const;
    void sendEvent(const UINT message, const WPARAM wParam, const LPARAM lParam) const;
};
