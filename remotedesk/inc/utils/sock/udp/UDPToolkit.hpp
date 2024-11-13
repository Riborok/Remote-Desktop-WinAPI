#pragma once

#include <vector>

#include "../../../sock/udp/Fragment.hpp"
#include "../../../sock/udp/FragmentDescriptor.hpp"

class UDPToolkit {
    static constexpr size_t PROTOCOL_HEADER_SIZE = 20 + 8;
    static constexpr size_t MAX_FRAGMENT_SIZE = 65535 - PROTOCOL_HEADER_SIZE;
    static constexpr size_t MTU_FRAGMENT_SIZE = 1500 - PROTOCOL_HEADER_SIZE;
public:
    UDPToolkit() = delete;
    
    static constexpr size_t METADATA_SIZE = sizeof(Metadata);

    static const FragmentDescriptor MAX_FRAGMENT_DESCRIPTOR;
    static const FragmentDescriptor MTU_FRAGMENT_DESCRIPTOR;

    static size_t calcTotalFragments(const size_t dataSize, const size_t fragmentDataSize);
    static std::vector<byte> createFragment(const std::vector<byte>& dataFragment, const Metadata& metadata);
    static void populateFragment(Fragment& fragment, const int bytesReceived);
private:
    static void addData(std::vector<byte>& fragment, const std::vector<byte>& data);
    static void addTotalSize(std::vector<byte>& fragment, const size_t totalSize);
    static void addFragmentNumber(std::vector<byte>& fragment, const size_t fragmentNumber);
    static void addId(std::vector<byte>& fragment, const size_t id);
    static void addSizeTToFragment(std::vector<byte>& fragment, const size_t value, const size_t index);

    static size_t extractTotalSize(const std::vector<byte>& fragment, const int bytesReceived);
    static size_t extractFragmentNumber(const std::vector<byte>& fragment, const int bytesReceived);
    static size_t extractId(const std::vector<byte>& fragment, const int bytesReceived);
    static size_t extractSizeTFromFragment(const std::vector<byte>& fragment, const size_t index);
};
