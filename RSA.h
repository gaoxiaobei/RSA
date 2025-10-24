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

    // ʹ��Ԥ�����������ʼ��RSA (������ʾ)
    void initialize(const BigInt& p, const BigInt& q) {
        // ���� n = p * q
        n = p * q;
        
        // ����ŷ������ ��(n) = (p-1)(q-1)
        BigInt phi = (p - BigInt(1)) * (q - BigInt(1));
        
        // ѡ��Կָ�� e (ͨ��ѡ��65537)
        e = BigInt(65537);
        
        // ȷ�� e �� ��(n) ����
        while (BigInt::gcd(e, phi) != BigInt(1)) {
            e = e + BigInt(2);
        }
        
        // ����˽Կָ�� d = e^(-1) mod ��(n)
        d = BigInt::modInverse(e, phi);
    }

    // ���ù�Կ������ֻ���ܵĳ�����
    void setPublicKey(const BigInt& e_val, const BigInt& n_val) {
        e = e_val;
        n = n_val;
    }

    // ����˽Կ������ֻ���ܵĳ�����
    void setPrivateKey(const BigInt& d_val, const BigInt& n_val) {
        d = d_val;
        n = n_val;
    }

    // ������Կ���ļ�
    bool saveKeys(const std::string& publicKeyFile = "public_key.txt",
                  const std::string& privateKeyFile = "private_key.txt") const {
        bool pub_saved = KeyManager::savePublicKey(e, n, publicKeyFile);
        bool priv_saved = KeyManager::savePrivateKey(d, n, privateKeyFile);
        return pub_saved && priv_saved;
    }

    // ���ļ�������Կ
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

    // ֻ���ع�Կ
    bool loadPublicKey(const std::string& publicKeyFile = "public_key.txt") {
        return KeyManager::loadPublicKey(e, n, publicKeyFile);
    }

    // ֻ����˽Կ
    bool loadPrivateKey(const std::string& privateKeyFile = "private_key.txt") {
        return KeyManager::loadPrivateKey(d, n, privateKeyFile);
    }

    // �����ַ���
    std::string encrypt(const std::string& plaintext) const {
        if (e.isZero() || n.isZero()) {
            throw std::runtime_error("��Կδ���ã�");
        }

        std::vector<BigInt> encrypted;
        
        for (char c : plaintext) {
            BigInt m((long long)(unsigned char)c);
            BigInt cipher = BigInt::modPow(m, e, n);
            encrypted.push_back(cipher);
        }
        
        // �����ܽ��ת��Ϊ�ַ���
        std::string result;
        for (size_t i = 0; i < encrypted.size(); ++i) {
            if (i > 0) result += " ";
            result += encrypted[i].toString();
        }
        
        return result;
    }

    // �����ַ���
    std::string decrypt(const std::string& ciphertext) const {
        if (d.isZero() || n.isZero()) {
            throw std::runtime_error("˽Կδ���ã�");
        }

        std::vector<BigInt> encrypted;
        
        // ��������
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
        
        // ����
        std::string result;
        for (const BigInt& cipher : encrypted) {
            BigInt m = BigInt::modPow(cipher, d, n);
            result += char(std::stoll(m.toString()));
        }
        
        return result;
    }

    // ��ȡ��Կ
    std::pair<BigInt, BigInt> getPublicKey() const {
        return {e, n};
    }

    // ��ȡ˽Կ
    std::pair<BigInt, BigInt> getPrivateKey() const {
        return {d, n};
    }

    // ��ʾ��Կ��Ϣ
    void displayKeys() const {
        std::cout << "=== RSA ��Կ��Ϣ ===" << std::endl;
        std::cout << "��Կ (e, n):" << std::endl;
        std::cout << "  e = " << e << std::endl;
        std::cout << "  n = " << n << std::endl;
        std::cout << "˽Կ (d, n):" << std::endl;
        std::cout << "  d = " << d << std::endl;
        std::cout << "  n = " << n << std::endl;
        std::cout << "=====================" << std::endl;
    }

    // �����Կ�Ƿ�������
    bool hasPublicKey() const {
        return !e.isZero() && !n.isZero();
    }

    bool hasPrivateKey() const {
        return !d.isZero() && !n.isZero();
    }

private:
    BigInt e; // ��Կָ��
    BigInt d; // ˽Կָ��
    BigInt n; // ģ��
};
