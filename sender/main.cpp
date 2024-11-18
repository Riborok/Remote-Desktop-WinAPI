#include <fstream>
#include <winsock2.h>
#include <sstream>

#include "dt/ThreadSafeQueue.hpp"
#include "screen/capture/ScreenCaptureWorker.hpp"
#include "sock/udp/sender/UDPSender.hpp"
#include "sock/udp/data-fragmenter/ComprEncrDataFragmenter.hpp"
#include "utils/array/ByteArrayUtils.hpp"
#include "utils/crypto/aes/AESToolkit.hpp"
#include "utils/sock/tcp/TCPUtils.hpp"
#include "utils/sock/WinSockUtils.hpp"

constexpr u_short DEFAULT_UDP_PORT = 4040;
constexpr u_short DEFAULT_TCP_PORT = 4041;
const std::string DEFAULT_IP = "127.0.0.1";
constexpr SIZE DEFAULT_SCREEN_SIZE = {1920, 1080};

struct Config {
    u_short udpPort;
    u_short tcpPort;
    std::string ip;
    SIZE screenResolution;
};

Config parseConfig(const std::string& filename) {
    Config config = {DEFAULT_UDP_PORT, DEFAULT_TCP_PORT, DEFAULT_IP, DEFAULT_SCREEN_SIZE};
    std::ifstream file(filename);
    if (!file.is_open()) {
        return config;
    }

    std::unordered_map<std::string, std::string> configMap;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string key, value;
        if (std::getline(lineStream, key, '=') && std::getline(lineStream, value)) {
            configMap[key] = value;
        }
    }
    
    if (configMap.find("UDP_PORT") != configMap.end()) {
        config.udpPort = static_cast<u_short>(std::stoi(configMap["UDP_PORT"]));
    }
    if (configMap.find("TCP_PORT") != configMap.end()) {
        config.tcpPort = static_cast<u_short>(std::stoi(configMap["TCP_PORT"]));
    }
    if (configMap.find("IP") != configMap.end()) {
        config.ip = configMap["IP"];
    }
    if (configMap.find("SCREEN_RESOLUTION") != configMap.end()) {
        const std::string& res = configMap["SCREEN_RESOLUTION"];
        size_t xPos = res.find('x');
        if (xPos != std::string::npos) {
            config.screenResolution.cx = std::stoi(res.substr(0, xPos));
            config.screenResolution.cy = std::stoi(res.substr(xPos + 1));
        }
    }

    return config;
}

void sendSize(const Socket& socket, const SIZE& size) {
    std::vector<byte> sizeBuffer(sizeof(int) * 2);
    ByteArrayUtils::setValue(sizeBuffer, 0, size.cx);
    ByteArrayUtils::setValue(sizeBuffer, 1, size.cy);
    socket.sendSocket(sizeBuffer);
}

int main() {
    WinSockUtils::initializeWinSock();

    auto [udpPort, tcpPort, ip, screenResolution] = parseConfig("sender_cfg.txt");
    const Socket socket = TCPUtils::connectToServer(ip, tcpPort);

    sendSize(socket, screenResolution);
    
    ThreadSafeQueue<std::vector<byte>> queue;
    ScreenCaptureWorker screenCaptureWorker(queue, screenResolution, 60);
    screenCaptureWorker.start();

    std::vector<byte> key(AESToolkit::MAX_KEY_LENGTH, 42);
    UDPSender sender(ip, udpPort, std::make_unique<ComprEncrDataFragmenter>(key));
    do {
        const auto data = queue.dequeue();
        sender.send(*data);
    } while(!GetAsyncKeyState(VK_ESCAPE));

    screenCaptureWorker.stop();
    WinSockUtils::cleanupWinSock();
    return 0;
}
