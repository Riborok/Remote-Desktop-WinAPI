#pragma once

#include <functional>
#include <memory>

#include "../../sock/tcp/TCPConnection.hpp"

class EventSender {
    std::unique_ptr<TCPConnection> _connection;
    SIZE _appSize;
    SIZE _receivedSize;
    std::function<void()> _disconnectCallback;
public:
    EventSender(std::unique_ptr<TCPConnection>&& connection, const SIZE& appSize, const SIZE& receivedSize);
    void sendEvent(const UINT message, const WPARAM wParam, const LPARAM lParam) const;
    void updateAppSize(const SIZE& appSize);
    void setDisconnectCallback(std::function<void()>&& callback);
private:
    void scaleMouseEventLParam(const UINT message, LPARAM& lParam) const;
    void sendData(const std::vector<byte>& buffer) const;
};
