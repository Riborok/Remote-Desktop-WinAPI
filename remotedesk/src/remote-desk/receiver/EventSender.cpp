#include "../../../inc/remote-desk/receiver/EventSender.hpp"

#include "../../../inc/utils/remote-desk/MessageUtils.hpp"
#include "../../../inc/utils/array/ByteArrayUtils.hpp"
#include "../../../inc/utils/screen/PointScaler.hpp"

EventSender::EventSender(std::unique_ptr<TCPConnection>&& connection, const SIZE& appSize, const SIZE& receivedSize):
    _connection(std::move(connection)), _appSize(appSize), _receivedSize(receivedSize) { }

void EventSender::sendEvent(const UINT message, const WPARAM wParam, LPARAM lParam) const {
    scaleMouseEventLParam(message, lParam);
    std::vector<byte> eventBuffer(sizeof(UINT) + sizeof(WPARAM) + sizeof(LPARAM));
    ByteArrayUtils::setValueByByteIndex<UINT>(eventBuffer, 0, message);
    ByteArrayUtils::setValueByByteIndex<WPARAM>(eventBuffer, sizeof(UINT), wParam);
    ByteArrayUtils::setValueByByteIndex<LPARAM>(eventBuffer, sizeof(UINT) + sizeof(WPARAM), lParam);
    _connection->sendData(eventBuffer);
}

void EventSender::updateAppSize(const SIZE& appSize) {
    _appSize = appSize;
}

void EventSender::scaleMouseEventLParam(const UINT message, LPARAM& lParam) const {
    if (MessageUtils::isMouseMessage(message)) {
        const PointScaler ps {_appSize, _receivedSize};
        const WORD x = ps.scaleByX(LOWORD(lParam));
        const WORD y = ps.scaleByY(HIWORD(lParam));
        lParam = MAKELPARAM(x, y);
    }
}
