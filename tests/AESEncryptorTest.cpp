﻿#include "utils.hpp"
#include "aes/AESDecryptor.hpp"
#include "aes/AESEncryptor.hpp"
#include "gtest/gtest.h"

TEST(AESEncryptorTest, EncryptionDecryptionTest) {
    const std::vector<byte> data(readFileToBuffer("test.txt"));
    const std::vector<byte> key(CryptoPP::AES::MAX_KEYLENGTH, 42);
    const AESEncryptor aes_encryptor(key);
    const AESDecryptor aes_decryptor(key);

    const std::vector<byte> ciphertext = aes_encryptor.encrypt(data);
    const std::vector<byte> decrypted = aes_decryptor.decrypt(ciphertext);
    
    EXPECT_EQ(data.size(), decrypted.size());
    EXPECT_EQ(data, decrypted);
}
