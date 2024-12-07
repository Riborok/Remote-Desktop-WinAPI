#pragma once

#include <memory>

#include "../../remote-desk/receiver/EventSender.hpp"
#include "../../sock/udp/receiver/UDPReceiver.hpp"
#include "../../screen/render/ScreenRenderWorker.hpp"
#include "../../sock/udp/receiver/UDPReceiverWorker.hpp"

class ReceiverInitializer {
public:
    ReceiverInitializer() = delete;

    static std::unique_ptr<ThreadSafeQueue<std::vector<byte>>> createFrames();
    static std::unique_ptr<TCPConnection> createConnection(const sockaddr_in& addr);
    static SIZE sendPortReceiveSize(TCPConnection& connection, const u_short udpPort);
    static std::unique_ptr<UDPReceiverWorker> createUDPReceiverWorker(ThreadSafeQueue<std::vector<byte>>& frames,
        TCPConnection& connection, const u_short udpPort, const SIZE& receivedSize);
    static std::unique_ptr<ScreenRenderWorker> createScreenRenderWorker(ThreadSafeQueue<std::vector<byte>>& frames,
        const HWND hWnd, const SIZE& receivedSize, const int fps, const int maxDelayMs);
    static std::unique_ptr<EventSender> createEventSender(std::unique_ptr<TCPConnection>&& connection,
        const SIZE& receivedSize);
};
