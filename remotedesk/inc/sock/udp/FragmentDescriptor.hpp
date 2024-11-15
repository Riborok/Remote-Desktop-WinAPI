#pragma once

class FragmentDescriptor {
    size_t _size;
    size_t _payloadSize;
    size_t _dataSize;
    
    FragmentDescriptor(const size_t size, const size_t payloadSize, const size_t dataSize);
    FragmentDescriptor(const size_t size, const size_t payloadSize);
public:
    explicit FragmentDescriptor(const size_t size);
    size_t getSize() const;
    size_t getPayloadSize() const;
    size_t getDataSize() const;
    FragmentDescriptor reducePayloadSize(const size_t delta) const;
    FragmentDescriptor reduceDataSize(const size_t delta) const;
};
