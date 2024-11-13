#pragma once

#include <optional>

#include "FragmentBuffer.hpp"
#include "FragmentStateTracker.hpp"

class FragmentCollector {
    FragmentBuffer _fragmentBuffer;
    FragmentStateTracker _stateTracker;
    std::optional<Fragment> _prevFragment;
public:
    explicit FragmentCollector(const size_t fragmentDataSize);
    bool collectPreviousFragment();
    bool collectFragment(std::optional<Fragment>& optionalFragment);
    std::vector<Fragment> extractFragments();
private:
    bool processFragment(Fragment& fragment);
    bool processUninitializedStateTracker(Fragment& fragment);
    bool processOldFragment() const;
    bool processNewFragment(Fragment& fragment);
    bool processCurrentFragment(Fragment& fragment);

    bool isAllFragmentsReceived() const;
};
