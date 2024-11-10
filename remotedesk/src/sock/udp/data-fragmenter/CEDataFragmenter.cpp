#include "../../../../inc/sock/udp/data-fragmenter/CEDataFragmenter.hpp"

#include <execution>

#include "../../../../inc/utils/MathUtils.hpp"
#include "../../../../inc/utils/CompressionUtils.hpp"

CEDataFragmenter::CEDataFragmenter(std::vector<byte>&& key): _encryptor(std::move(key)) { }

std::vector<std::vector<byte>> CEDataFragmenter::createDataFragments(const std::vector<byte>& data) {
    const std::vector<std::vector<byte>> compressedDFs = compressDataFragments(data);
    const std::vector<std::vector<byte>> encryptedDFs = encryptDataFragments(compressedDFs);
    return encryptedDFs;
}

std::vector<std::vector<byte>> CEDataFragmenter::compressDataFragments(const std::vector<byte>& data) {
    const size_t totalFragments = UDPToolkit::calcTotalFragments(data.size());
    std::vector<std::vector<byte>> compressedDFs(totalFragments);
    std::transform(std::execution::par, compressedDFs.begin(), compressedDFs.end(), compressedDFs.begin(),
        [&](const std::vector<byte>& fragment) {
            const size_t i = &fragment - compressedDFs.data();
            const auto [begin, len] = getDataFragment(data, i);
            return CompressionUtils::compress(begin, len);
        }
    );
    return compressedDFs;
}

std::vector<std::vector<byte>> CEDataFragmenter::encryptDataFragments(const std::vector<std::vector<byte>>& compressedDFs) {
    std::vector<std::vector<byte>> encryptedDFs(compressedDFs.size());
    for (size_t i = 0; i < compressedDFs.size(); ++i) {
        encryptedDFs[i] = _encryptor.encrypt(compressedDFs[i]);
    }
    return encryptedDFs;
}
