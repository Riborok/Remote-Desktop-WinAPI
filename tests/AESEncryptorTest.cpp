#include "inc/utils.hpp"
#include "aes/AESDecryptor.hpp"
#include "aes/AESEncryptor.hpp"
#include "gtest/gtest.h"

void performEncryptionDecryptionTest(const std::string& filename) {
    const std::vector<byte> data(readFileToBuffer(filename));
    const std::vector<byte> key(CryptoPP::AES::MAX_KEYLENGTH, 42);
    AESEncryptor aesEncryptor{std::vector<byte>(key)};
    AESDecryptor aesDecryptor{std::vector<byte>(key)};

    const std::vector<byte> ciphertext = aesEncryptor.encrypt(data);
    const std::vector<byte> decrypted = aesDecryptor.decrypt(ciphertext);
    
    EXPECT_EQ(data.size(), decrypted.size());
    EXPECT_EQ(data, decrypted);
}

TEST(AESEncryptorTest, EncryptionDecryptionTestTXT) {
    performEncryptionDecryptionTest("res/test.txt");
}

TEST(AESEncryptorTest, EncryptionDecryptionTestJPG) {
    performEncryptionDecryptionTest("res/test.jpg");
}
