#pragma once
#include "BigInt.h"
#include "KeyManager.h"
#include <string>
#include <vector>
#include <random>
#include <ctime>

class RSA {
public:
    RSA() : e(0), d(0), n(0) {}

    // 使用预定义的素数初始化RSA (用于演示)
    void initialize(const BigInt& p, const BigInt& q) {
        // 计算 n = p * q
        n = p * q;
        
        // 计算欧拉函数 φ(n) = (p-1)(q-1)
        BigInt phi = (p - BigInt(1)) * (q - BigInt(1));
        
        // 选择公钥指数 e (通常选择65537)
        e = BigInt(65537);
        
        // 确保 e 与 φ(n) 互质
        while (BigInt::gcd(e, phi) != BigInt(1)) {
            e = e + BigInt(2);
        }
        
        // 计算私钥指数 d = e^(-1) mod φ(n)
        d = BigInt::modInverse(e, phi);
    }

    // 设置公钥（用于只加密的场景）
    void setPublicKey(const BigInt& e_val, const BigInt& n_val) {
        e = e_val;
        n = n_val;
    }

    // 设置私钥（用于只解密的场景）
    void setPrivateKey(const BigInt& d_val, const BigInt& n_val) {
        d = d_val;
        n = n_val;
    }

    // 保存密钥到文件
    bool saveKeys(const std::string& publicKeyFile = "public_key.txt",
                  const std::string& privateKeyFile = "private_key.txt") const {
        bool pub_saved = KeyManager::savePublicKey(e, n, publicKeyFile);
        bool priv_saved = KeyManager::savePrivateKey(d, n, privateKeyFile);
        return pub_saved && priv_saved;
    }

    // 从文件加载密钥
    bool loadKeys(const std::string& publicKeyFile = "public_key.txt",
                  const std::string& privateKeyFile = "private_key.txt") {
        BigInt e_temp, d_temp, n_pub, n_priv;
        
        bool pub_loaded = KeyManager::loadPublicKey(e_temp, n_pub, publicKeyFile);
        bool priv_loaded = KeyManager::loadPrivateKey(d_temp, n_priv, privateKeyFile);
        
        if (pub_loaded && priv_loaded && n_pub == n_priv) {
            e = e_temp;
            d = d_temp;
            n = n_pub;
            return true;
        }
        return false;
    }

    // 只加载公钥
    bool loadPublicKey(const std::string& publicKeyFile = "public_key.txt") {
        return KeyManager::loadPublicKey(e, n, publicKeyFile);
    }

    // 只加载私钥
    bool loadPrivateKey(const std::string& privateKeyFile = "private_key.txt") {
        return KeyManager::loadPrivateKey(d, n, privateKeyFile);
    }

    // 加密字符串
    std::string encrypt(const std::string& plaintext) const {
        if (e.isZero() || n.isZero()) {
            throw std::runtime_error("公钥未设置！");
        }

        std::vector<BigInt> encrypted;
        
        for (char c : plaintext) {
            BigInt m((long long)(unsigned char)c);
            BigInt cipher = BigInt::modPow(m, e, n);
            encrypted.push_back(cipher);
        }
        
        // 将加密结果转换为字符串
        std::string result;
        for (size_t i = 0; i < encrypted.size(); ++i) {
            if (i > 0) result += " ";
            result += encrypted[i].toString();
        }
        
        return result;
    }

    // 解密字符串
    std::string decrypt(const std::string& ciphertext) const {
        if (d.isZero() || n.isZero()) {
            throw std::runtime_error("私钥未设置！");
        }

        std::vector<BigInt> encrypted;
        
        // 解析密文
        std::string num;
        for (char c : ciphertext) {
            if (c == ' ') {
                if (!num.empty()) {
                    encrypted.push_back(BigInt(num));
                    num.clear();
                }
            } else {
                num += c;
            }
        }
        if (!num.empty()) {
            encrypted.push_back(BigInt(num));
        }
        
        // 解密
        std::string result;
        for (const BigInt& cipher : encrypted) {
            BigInt m = BigInt::modPow(cipher, d, n);
            result += char(std::stoll(m.toString()));
        }
        
        return result;
    }

    // 获取公钥
    std::pair<BigInt, BigInt> getPublicKey() const {
        return {e, n};
    }

    // 获取私钥
    std::pair<BigInt, BigInt> getPrivateKey() const {
        return {d, n};
    }

    // 显示密钥信息
    void displayKeys() const {
        std::cout << "=== RSA 密钥信息 ===" << std::endl;
        std::cout << "公钥 (e, n):" << std::endl;
        std::cout << "  e = " << e << std::endl;
        std::cout << "  n = " << n << std::endl;
        std::cout << "私钥 (d, n):" << std::endl;
        std::cout << "  d = " << d << std::endl;
        std::cout << "  n = " << n << std::endl;
        std::cout << "=====================" << std::endl;
    }

    // 检查密钥是否已设置
    bool hasPublicKey() const {
        return !e.isZero() && !n.isZero();
    }

    bool hasPrivateKey() const {
        return !d.isZero() && !n.isZero();
    }

private:
    BigInt e; // 公钥指数
    BigInt d; // 私钥指数
    BigInt n; // 模数
};
