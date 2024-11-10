#pragma once

#include <config_int.h>
#include <vector>

#include "../../../sock/udp/Fragment.hpp"

using CryptoPP::byte;

class UDPToolkit {
    static constexpr size_t MAX_UDP_SIZE = 65535;
    static constexpr size_t MTU_SIZE = 1500;
public:
    UDPToolkit() = delete;

    static constexpr size_t METADATA_SIZE = sizeof(Metadata);
    static constexpr size_t FRAGMENT_SIZE = MAX_UDP_SIZE - 20 - 8;
    static constexpr size_t DATA_SIZE = FRAGMENT_SIZE - METADATA_SIZE;

    static size_t calcTotalFragments(const size_t dataSize);
    static std::vector<byte> createFragment(const std::vector<byte>& dataFragment, const Metadata& metadata);
    static void populateFragment(Fragment& fragment, const int bytesReceived);
private:
    static void addData(std::vector<byte>& fragment, const std::vector<byte>& data);
    static void addTotalSize(std::vector<byte>& fragment, const size_t totalSize);
    static void addFragmentNumber(std::vector<byte>& fragment, const size_t fragmentNumber);
    static void addId(std::vector<byte>& fragment, const size_t id);
    static void addSizeTToFragment(std::vector<byte>& fragment, const size_t value, const size_t startIdx);

    static size_t extractTotalSize(const std::vector<byte>& fragment, const int bytesReceived);
    static size_t extractFragmentNumber(const std::vector<byte>& fragment, const int bytesReceived);
    static size_t extractId(const std::vector<byte>& fragment, const int bytesReceived);
    static size_t extractSizeTFromFragment(const std::vector<byte>& fragment, const size_t startIdx);
};
