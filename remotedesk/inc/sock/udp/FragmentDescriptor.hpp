#pragma once

class FragmentDescriptor {
    size_t _size;
    size_t _payloadSize;

    FragmentDescriptor(const size_t size, const size_t payloadSize);
public:
    explicit FragmentDescriptor(const size_t size);
    size_t getSize() const;
    size_t getPayloadSize() const;
    FragmentDescriptor reducePayloadSize(const size_t delta) const;
};
