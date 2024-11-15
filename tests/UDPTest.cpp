#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <string>

#include "inc/utils.hpp"
#include "sock/udp/receiver/UDPReceiver.hpp"
#include "sock/udp/sender/UDPSender.hpp"
#include "sock/udp/data-fragmenter/ComprEncrDataFragmenter.hpp"
#include "sock/udp/data-reassembler/DecrDecomprDataReassembler.hpp"
#include "utils/crypto/aes/AESToolkit.hpp"

namespace {
    void performSendAndReceiveTest(const std::string& filename, const std::string& ip, const u_short port, std::tuple<UDPSender, UDPReceiver>& udpSenderReceiver);
    void performSendAndReceiveMultipleFilesTest(const std::vector<std::string>& filenames, const std::string& ip, const u_short port, std::tuple<UDPSender, UDPReceiver>& udpSenderReceiver);
    std::tuple<UDPSender, UDPReceiver> createSecureUDPSenderAndReceiver(const std::string& ip, const u_short port, const std::vector<byte>& key);
    std::tuple<UDPSender, UDPReceiver> createPlainUDPSenderAndReceiver(const std::string& ip, const u_short port);
    std::thread performReceiverTest(UDPReceiver& receiver, const std::vector<std::vector<byte>>& allData);
    std::thread performSenderTest(const UDPSender& sender, const std::vector<std::vector<byte>>& allData);
    void verifyReceivedData(const MaskedData& maskedData, const std::vector<byte>& expectedData);
}

TEST_F(NetworkTestBase, SendAndReceivePlainMultipleFragmentsJPG) {
    auto udpSenderReceiver = createPlainUDPSenderAndReceiver(IP, PORT);
    performSendAndReceiveTest("res/test.jpg", IP, PORT, udpSenderReceiver);
}

TEST_F(NetworkTestBase, SendAndReceivePlainMultipleFragmentsTXT) {
    auto udpSenderReceiver = createPlainUDPSenderAndReceiver(IP, PORT);
    performSendAndReceiveTest("res/test.txt", IP, PORT, udpSenderReceiver);
}

TEST_F(NetworkTestBase, SendAndReceivePlainFiveFiles) {
    const std::vector<std::string> filenames = {"res/test.txt", "res/test.jpg"};
    auto udpSenderReceiver = createPlainUDPSenderAndReceiver(IP, PORT);
    performSendAndReceiveMultipleFilesTest(filenames, IP, PORT, udpSenderReceiver);
}


TEST_F(NetworkTestBase, SendAndReceiveMultipleFragmentsJPG) {
    const std::vector<byte> key(AESToolkit::MAX_KEY_LENGTH, 42);
    auto udpSenderReceiver = createSecureUDPSenderAndReceiver(IP, PORT, key);
    performSendAndReceiveTest("res/test.jpg", IP, PORT, udpSenderReceiver);
}

TEST_F(NetworkTestBase, SendAndReceiveMultipleFragmentsTXT) {
    const std::vector<byte> key(AESToolkit::MAX_KEY_LENGTH, 42);
    auto udpSenderReceiver = createSecureUDPSenderAndReceiver(IP, PORT, key);
    performSendAndReceiveTest("res/test.txt", IP, PORT, udpSenderReceiver);
}

TEST_F(NetworkTestBase, SendAndReceiveFiveMaskedFiles) {
    const std::vector<std::string> filenames = {"res/test.txt", "res/test.jpg"};
    const std::vector<byte> key(AESToolkit::MAX_KEY_LENGTH, 42);
    auto udpSenderReceiver = createSecureUDPSenderAndReceiver(IP, PORT, key);
    performSendAndReceiveMultipleFilesTest(filenames, IP, PORT, udpSenderReceiver);
}

namespace {
    void performSendAndReceiveTest(const std::string& filename, const std::string& ip, const u_short port, std::tuple<UDPSender, UDPReceiver>& udpSenderReceiver) {
        const std::vector<byte> expectedData(readFileToBuffer(filename));
        auto& [sender, receiver] = udpSenderReceiver;
        std::thread receiverThread(performReceiverTest(receiver, {expectedData}));
        std::thread senderThread(performSenderTest(sender, {expectedData}));
        receiverThread.join();
        senderThread.join();
    }

    void performSendAndReceiveMultipleFilesTest(const std::vector<std::string>& filenames, const std::string& ip, const u_short port, std::tuple<UDPSender, UDPReceiver>& udpSenderReceiver) {
        std::vector<std::vector<byte>> allData;
        for (const auto& filename : filenames) {
            allData.emplace_back(readFileToBuffer(filename));
        }
        auto& [sender, receiver] = udpSenderReceiver;
        std::thread receiverThread(performReceiverTest(receiver, allData));
        std::thread senderThread(performSenderTest(sender, allData));
        receiverThread.join();
        senderThread.join();
    }

    std::tuple<UDPSender, UDPReceiver> createSecureUDPSenderAndReceiver(const std::string& ip, const u_short port, const std::vector<byte>& key) {
        auto dataFragmenter = std::make_unique<ComprEncrDataFragmenter>(key);
        auto dataReassembler = std::make_unique<DecrDecomprDataReassembler>(key);
        UDPSender sender(ip, port, std::move(dataFragmenter), 64 * MemoryUnits::MEGABYTE);
        UDPReceiver receiver(port, std::move(dataReassembler), 64 * MemoryUnits::MEGABYTE);
        return std::make_tuple(std::move(sender), std::move(receiver));
    }

    std::tuple<UDPSender, UDPReceiver> createPlainUDPSenderAndReceiver(const std::string& ip, const u_short port) {
        auto dataFragmenter = std::make_unique<DataFragmenter>();
        auto dataReassembler = std::make_unique<DataReassembler>();
        UDPSender sender(ip, port, std::move(dataFragmenter), 64 * MemoryUnits::MEGABYTE);
        UDPReceiver receiver(port, std::move(dataReassembler), 64 * MemoryUnits::MEGABYTE);
        return std::make_tuple(std::move(sender), std::move(receiver));
    }

    std::thread performReceiverTest(UDPReceiver& receiver, const std::vector<std::vector<byte>>& allData) {
        return std::thread([&] {
            for (const auto& expectedData : allData) {
                verifyReceivedData(receiver.receiveMaskedData(), expectedData);
            }
        });
    }

    std::thread performSenderTest(const UDPSender& sender, const std::vector<std::vector<byte>>& allData) {
        return std::thread([&] {
            for (const auto& data : allData) {
                sender.send(data);
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        });
    }

    void verifyReceivedData(const MaskedData& maskedData, const std::vector<byte>& expectedData) {
        EXPECT_EQ(maskedData.getData(), expectedData);
        EXPECT_EQ(maskedData.getMask(), std::vector<byte>(maskedData.getData().size(), 0));
    }
}
