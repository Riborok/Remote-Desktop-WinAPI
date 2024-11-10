#include <aes.h>
#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <optional>
#include <string>

#include "inc/utils.hpp"
#include "sock/udp/receiver/UDPReceiver.hpp"
#include "sock/udp/sender/UDPSender.hpp"
#include "sock/udp/data-fragmenter/CEDataFragmenter.hpp"
#include "sock/udp/data-reassembler/DDDataReassembler.hpp"

void performSendAndReceiveTest(const std::string& filename, const std::string& ip, const u_short port);
void performSendAndReceiveMultipleFilesTest(const std::vector<std::string>& filenames, const std::string& ip, const u_short port);
std::tuple<UDPSender, UDPReceiver> createUDPSenderAndReceiver(const std::string& ip, const u_short port, const std::vector<byte>& key);
std::thread performReceiverTest(UDPReceiver& receiver, const std::vector<std::vector<byte>>& allData);
std::thread performSenderTest(UDPSender& sender, const std::vector<std::vector<byte>>& allData);
void verifyReceivedData(const MaskedData& maskedData, const std::vector<byte>& expectedData);

TEST_F(NetworkTestBase, SendAndReceiveMultipleFragmentsJPG) {
    performSendAndReceiveTest("res/test.jpg", IP, PORT);
}

TEST_F(NetworkTestBase, SendAndReceiveMultipleFragmentsTXT) {
    performSendAndReceiveTest("res/test.txt", IP, PORT);
}

TEST_F(NetworkTestBase, SendAndReceiveFiveMaskedFiles) {
    std::vector<std::string> filenames;
    for (int i = 1; i <= 5; ++i) {
        filenames.emplace_back("res/test.txt");
        filenames.emplace_back("res/test.jpg");
    }
    performSendAndReceiveMultipleFilesTest(filenames, IP, PORT);
}

void performSendAndReceiveTest(const std::string& filename, const std::string& ip, const u_short port) {
    const std::vector<byte> expectedData(readFileToBuffer(filename));
    const std::vector<byte> key(CryptoPP::AES::MAX_KEYLENGTH, 42);
    auto [sender, receiver] = createUDPSenderAndReceiver(ip, port, key);
    std::thread receiverThread(performReceiverTest(receiver, {expectedData}));
    std::thread senderThread(performSenderTest(sender, {expectedData}));
    receiverThread.join();
    senderThread.join();
}

void performSendAndReceiveMultipleFilesTest(const std::vector<std::string>& filenames, const std::string& ip, const u_short port) {
    std::vector<std::vector<byte>> allData;
    for (const auto& filename : filenames) {
        allData.push_back(readFileToBuffer(filename));
    }
    const std::vector<byte> key(CryptoPP::AES::MAX_KEYLENGTH, 42);
    auto [sender, receiver] = createUDPSenderAndReceiver(ip, port, key);
    std::thread receiverThread(performReceiverTest(receiver, allData));
    std::thread senderThread(performSenderTest(sender, allData));
    receiverThread.join();
    senderThread.join();
}

std::tuple<UDPSender, UDPReceiver> createUDPSenderAndReceiver(const std::string& ip, const u_short port,
        const std::vector<byte>& key) {
    auto dataFragmenter = std::make_unique<CEDataFragmenter>(key);
    auto dataReassembler = std::make_unique<DDDataReassembler>(key);
    UDPSender sender(ip, port, std::move(dataFragmenter), 64*MemoryUnits::MEGABYTE);
    UDPReceiver receiver(port, std::move(dataReassembler), 64*MemoryUnits::MEGABYTE);
    return std::make_tuple(std::move(sender), std::move(receiver));
}

std::thread performReceiverTest(UDPReceiver& receiver, const std::vector<std::vector<byte>>& allData) {
    std::thread receiverThread([&] {
        for (const auto& expectedData : allData) {
            verifyReceivedData(receiver.receiveMaskedData(), expectedData);
        }
    });
    return receiverThread;
}

std::thread performSenderTest(UDPSender& sender, const std::vector<std::vector<byte>>& allData) {
    std::thread senderThread([&] {
        for (const auto& data : allData) {
           sender.send(data);
           std::this_thread::sleep_for(std::chrono::milliseconds(5));
       }
    });
    return senderThread;
}

void verifyReceivedData(const MaskedData& maskedData, const std::vector<byte>& expectedData) {
    EXPECT_EQ(maskedData.getData(), expectedData);
    EXPECT_EQ(maskedData.getMask(), std::vector<byte>(maskedData.getData().size(), 0));
}
