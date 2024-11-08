#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <optional>
#include <string>

#include "inc/utils.hpp"
#include "sock/udp/UDPReceiver.hpp"
#include "sock/udp/UDPSender.hpp"

TEST_F(NetworkTestBase, ReceiveNoData_ReturnsEmpty) {
    const UDPReceiver receiver(PORT);
    const std::optional<Payload> receivedPayload = receiver.receivePayload();
    EXPECT_FALSE(receivedPayload.has_value());
}

TEST_F(NetworkTestBase, SendAndReceiveSinglePacket) {
    const std::vector<byte> testData = {1, 2, 3, 4, 5};

    const UDPReceiver receiver(PORT);
    UDPSender sender(IP, PORT);

    std::thread receiverThread([&] {
        const std::optional<Payload> receivedPayload = receiver.receivePayload();
        EXPECT_TRUE(receivedPayload.has_value());
        EXPECT_EQ(receivedPayload.value().fragmentNumber, 0);
        EXPECT_EQ(receivedPayload.value().totalDataSize, testData.size());
        EXPECT_EQ(receivedPayload.value().data, testData);
    });

    sender.send(testData);

    receiverThread.join();
}

void performSendAndReceiveTest(const std::string& filename, const std::string& ip, const u_short port) {
    const std::vector<byte> largeData(readFileToBuffer(filename));

    const UDPReceiver receiver(port);
    UDPSender sender(ip, port);

    std::thread receiverThread([&] {
        std::vector<byte> receivedData;
        size_t fragmentNumber = 0;

        while (receivedData.size() < largeData.size()) {
            std::optional<Payload> receivedPayload = receiver.receivePayload();
            EXPECT_TRUE(receivedPayload.has_value());
            EXPECT_EQ(receivedPayload.value().fragmentNumber, fragmentNumber++);
            EXPECT_EQ(receivedPayload.value().totalDataSize, largeData.size());
            const auto& data = receivedPayload.value().data;
            receivedData.insert(receivedData.end(), data.begin(), data.end());
        }

        EXPECT_EQ(receivedData, largeData);
    });

    sender.send(largeData);
    receiverThread.join();
}

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
    
    std::vector<std::vector<byte>> allData;
    for (const auto& filename : filenames) {
        allData.push_back(readFileToBuffer(filename));
    }

    UDPReceiver receiver(PORT);
    UDPSender sender(IP, PORT);

    std::thread receiverThread([&] {
        for (const auto& expectedData : allData) {
            const MaskedData maskedData = receiver.receiveMaskedData();

            EXPECT_EQ(maskedData.getData(), expectedData);
            for (const auto& maskValue : maskedData.getMask()) {
                EXPECT_EQ(maskValue, 0);
            }
        }
    });

    for (const auto& data : allData) {
        sender.send(data);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    receiverThread.join();
}