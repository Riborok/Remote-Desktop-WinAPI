#include "../../../../inc/sock/udp/receiver/FragmentBuffer.hpp"

#include "../../../../inc/utils/sock/udp/UDPToolkit.hpp"

void FragmentBuffer::reserve(const size_t totalFragments) {
    _fragments.reserve(totalFragments);
}

void FragmentBuffer::insertFragment(Fragment&& fragment) {
    _fragments.push_back(std::move(fragment));
}

bool FragmentBuffer::isEmpty() const { return _fragments.empty(); }

size_t FragmentBuffer::getSize() const { return _fragments.size(); }

std::vector<Fragment> FragmentBuffer::extractFragments() {
    std::vector<Fragment> extractedFragments = std::move(_fragments);
    _fragments.clear();
    return extractedFragments;
}
