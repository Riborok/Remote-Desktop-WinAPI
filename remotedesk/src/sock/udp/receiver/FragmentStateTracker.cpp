#include "../../../../inc/sock/udp/receiver/FragmentStateTracker.hpp"

#include "../../../../inc/utils/sock/udp/UDPToolkit.hpp"

void FragmentStateTracker::reset() {
    _isInitialized = false;
    _oldFragmentCountInRow = 0;
}

bool FragmentStateTracker::initialize(const Fragment& fragment) {
    if (_prevId < fragment.id) {
        _isInitialized = true;
        _prevId = _currentId;
        _currentId = fragment.id;
        _totalFragments = UDPToolkit::calcTotalFragments(fragment.totalDataSize);
        return true;
    }
    return false;
}

bool FragmentStateTracker::isOldFragment(const Fragment& fragment) {
    if (_currentId > fragment.id) {
        ++_oldFragmentCountInRow;
        return true;
    }
    _oldFragmentCountInRow = 0;
    return false;
}

bool FragmentStateTracker::hasExceededOldFragmentCountInRow() const {
    return _oldFragmentCountInRow >= MAX_OLD_FRAGMENTS_IN_ROW;
}

bool FragmentStateTracker::isNewFragment(const Fragment& fragment) const {
    return _currentId < fragment.id;
}

bool FragmentStateTracker::isInitialized() const { return _isInitialized; }

size_t FragmentStateTracker::getTotalFragments() const { return _totalFragments; }
