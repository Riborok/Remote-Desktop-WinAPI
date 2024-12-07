#include "../../../inc/utils/remote-desk/MessageUtils.hpp"

bool MessageUtils::isKeyboardMessage(const UINT message) {
    return message >= WM_KEYFIRST && message <= WM_KEYLAST;
}

bool MessageUtils::isMouseMessage(const UINT message) {
    return message >= WM_MOUSEFIRST && message <= WM_MOUSELAST;
}
