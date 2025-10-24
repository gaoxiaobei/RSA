#pragma once
#include "BigInt.h"
#include <fstream>
#include <sstream>

class KeyManager {
public:
    // 保存公钥到文件
    static bool savePublicKey(const BigInt& e, const BigInt& n, const std::string& filename = "public_key.txt") {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        file << e.toString() << std::endl;
        file << n.toString() << std::endl;
        file.close();
        return true;
    }

    // 保存私钥到文件
    static bool savePrivateKey(const BigInt& d, const BigInt& n, const std::string& filename = "private_key.txt") {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        file << d.toString() << std::endl;
        file << n.toString() << std::endl;
        file.close();
        return true;
    }

    // 加载公钥
    static bool loadPublicKey(BigInt& e, BigInt& n, const std::string& filename = "public_key.txt") {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        std::string e_str, n_str;
        std::getline(file, e_str);
        std::getline(file, n_str);
        file.close();

        if (e_str.empty() || n_str.empty()) {
            return false;
        }

        e = BigInt(e_str);
        n = BigInt(n_str);
        return true;
    }

    // 加载私钥
    static bool loadPrivateKey(BigInt& d, BigInt& n, const std::string& filename = "private_key.txt") {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        std::string d_str, n_str;
        std::getline(file, d_str);
        std::getline(file, n_str);
        file.close();

        if (d_str.empty() || n_str.empty()) {
            return false;
        }

        d = BigInt(d_str);
        n = BigInt(n_str);
        return true;
    }

    // 检查密钥文件是否存在
    static bool keysExist(const std::string& publicKeyFile = "public_key.txt", 
                         const std::string& privateKeyFile = "private_key.txt") {
        std::ifstream pubFile(publicKeyFile);
        std::ifstream privFile(privateKeyFile);
        return pubFile.good() && privFile.good();
    }
};
