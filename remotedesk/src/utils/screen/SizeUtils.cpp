#include "../../../inc/utils/screen/SizeUtils.hpp"

bool operator!=(const SIZE& lhs, const SIZE& rhs) {
    return !(lhs == rhs);
}

bool operator==(const SIZE& lhs, const SIZE& rhs) {
    return lhs.cx == rhs.cx && lhs.cy == rhs.cy;
}
