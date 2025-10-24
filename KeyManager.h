#pragma once
#include "BigInt.h"
#include <fstream>
#include <sstream>

class KeyManager {
public:
    // ���湫Կ���ļ�
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

    // ����˽Կ���ļ�
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

    // ���ع�Կ
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

    // ����˽Կ
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

    // �����Կ�ļ��Ƿ����
    static bool keysExist(const std::string& publicKeyFile = "public_key.txt", 
                         const std::string& privateKeyFile = "private_key.txt") {
        std::ifstream pubFile(publicKeyFile);
        std::ifstream privFile(privateKeyFile);
        return pubFile.good() && privFile.good();
    }
};
