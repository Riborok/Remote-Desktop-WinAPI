#include "../../../../inc/sock/udp/receiver/FragmentCollector.hpp"

FragmentCollector::FragmentCollector(const size_t fragmentPayloadSize): _stateTracker(fragmentPayloadSize) { }

bool FragmentCollector::collectPreviousFragment() {
    if (_prevFragment) {
        const bool isSingleFragmentPacket = processUninitializedStateTracker(*_prevFragment);
        _prevFragment.reset();
        return isSingleFragmentPacket;
    }
    return false;
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
bool FragmentCollector::collectFragment(std::optional<Fragment>& optionalFragment) {
    return (optionalFragment.has_value() && !processFragment(*optionalFragment)) || _fragmentBuffer.isEmpty();
}

bool FragmentCollector::processFragment(Fragment& fragment) {
    if (!_stateTracker.isInitialized()) {
        return processUninitializedStateTracker(fragment);
    }
    if (_stateTracker.isOldFragment(fragment)) {
        return processOldFragment();
    }
    if (_stateTracker.isNewFragment(fragment)) {
        return processNewFragment(fragment);
    }
    return processCurrentFragment(fragment);
}

bool FragmentCollector::processUninitializedStateTracker(Fragment& fragment) {
    if (_stateTracker.initialize(fragment)) {
        _fragmentBuffer.reserve(_stateTracker.getTotalFragments());
        _fragmentBuffer.insertFragment(std::move(fragment));
        return isAllFragmentsReceived();
    }
    return false;
}

bool FragmentCollector::processOldFragment() const {
    return _stateTracker.hasExceededOldFragmentCountInRow();
}

bool FragmentCollector::processNewFragment(Fragment& fragment) {
    _prevFragment = std::move(fragment);
    return true;
}

bool FragmentCollector::processCurrentFragment(Fragment& fragment) {
    _fragmentBuffer.insertFragment(std::move(fragment));
    return isAllFragmentsReceived();
}

bool FragmentCollector::isAllFragmentsReceived() const {
    return _stateTracker.getTotalFragments() == _fragmentBuffer.getSize();
}

std::vector<Fragment> FragmentCollector::extractFragments() {
    _stateTracker.reset();
    return _fragmentBuffer.extractFragments();
}
