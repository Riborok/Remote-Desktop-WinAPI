﻿#include "inc/utils.hpp"
#include "aes/AESDecryptor.hpp"
#include "aes/AESEncryptor.hpp"
#include "gtest/gtest.h"
#include "utils/aes/AESToolkit.hpp"

void performEncryptionDecryptionTest(const std::string& filename) {
    const std::vector<byte> data(readFileToBuffer(filename));
    const std::vector<byte> key(AESToolkit::MAX_KEY_LENGTH, 42);
    AESEncryptor aesEncryptor(key);
    AESDecryptor aesDecryptor(key);

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
