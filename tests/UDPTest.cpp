#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <string>

#include "inc/utils.hpp"
#include "sock/udp/UDPReceiver.hpp"
#include "sock/udp/UDPSender.hpp"

TEST_F(NetworkTestBase, SendAndReceiveSinglePacket) {
    const std::vector<byte> testData = {1, 2, 3, 4, 5};

    const UDPReceiver receiver(PORT);
    UDPSender sender(IP, PORT);

    std::thread receiverThread([&] {
        const auto receivedPayload = receiver.receivePayload();
        EXPECT_EQ(receivedPayload.packetNumber, 0);
        EXPECT_EQ(receivedPayload.totalDataSize, testData.size());
        EXPECT_EQ(receivedPayload.data, testData);
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
        size_t packetNumber = 0;

        while (receivedData.size() < largeData.size()) {
            auto receivedPayload = receiver.receivePayload();
            EXPECT_EQ(receivedPayload.packetNumber, packetNumber++);
            EXPECT_EQ(receivedPayload.totalDataSize, largeData.size());
            receivedData.insert(receivedData.end(), receivedPayload.data.begin(), receivedPayload.data.end());
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
