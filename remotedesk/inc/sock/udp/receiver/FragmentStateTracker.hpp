#pragma once

#include "../Fragment.hpp"

class FragmentStateTracker {
    static constexpr size_t MAX_OLD_FRAGMENTS_IN_ROW = 10;

    size_t _fragmentPayloadSize;
    bool _isInitialized = false;
    size_t _oldFragmentCountInRow = 0;
    size_t _prevId = 0;
    size_t _currentId = 0;
    size_t _totalFragments = 0;
public:
    explicit FragmentStateTracker(const size_t fragmentPayloadSize);
    void reset();
    bool initialize(const Fragment& fragment);
    
    bool isOldFragment(const Fragment& fragment);
    bool hasExceededOldFragmentCountInRow() const;
    bool isNewFragment(const Fragment& fragment) const;
    bool isInitialized() const;
    size_t getTotalFragments() const;
};
