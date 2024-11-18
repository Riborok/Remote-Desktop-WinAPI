#include "../../../inc/utils/screen/ScreenUtils.hpp"

SIZE ScreenUtils::getScreenSize() {
    return { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
}
