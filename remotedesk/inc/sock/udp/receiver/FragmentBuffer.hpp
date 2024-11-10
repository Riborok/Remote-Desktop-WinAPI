#pragma once

#include "MaskedData.hpp"

class FragmentBuffer {
    std::vector<Fragment> _fragments;
public:
    void reserve(const size_t totalFragments);
    void insertFragment(Fragment&& fragment);
    
    bool isEmpty() const;
    size_t getSize() const;
    std::vector<Fragment>&& extractFragments();
};
