#pragma once

class FragmentDescriptor {
    size_t _size;
    size_t _dataSize;

    FragmentDescriptor(const size_t size, const size_t dataSize);
public:
    explicit FragmentDescriptor(const size_t size);
    size_t getSize() const;
    size_t getDataSize() const;
    FragmentDescriptor reduceDataSize(const size_t delta) const;
};
