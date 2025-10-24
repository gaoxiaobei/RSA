#include "BigInt.h"
#include "RSA.h"
#include "PrimeGenerator.h"
#include "KeyManager.h"
#include <iostream>
#include <string>
#include <limits>

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void generateNewKeys(RSA& rsa) {
    std::cout << "\n=== �����µ�RSA��Կ�� ===" << std::endl << std::endl;
    
    PrimeGenerator::displaySecurityLevels();
    std::cout << "\n��ѡ��ȫ���� (0-4): ";
    
    int level;
    std::cin >> level;
    clearInputBuffer();
    
    if (level < 0 || level > 4) {
        level = 1;
        std::cout << "��Чѡ��ʹ��Ĭ�ϼ��� 1" << std::endl;
    }
    
    std::cout << "\n����������Կ��..." << std::endl;
    
    auto primePair = PrimeGenerator::getSafePrimePair(level);
    rsa.initialize(primePair.first, primePair.second);
    
    std::cout << "? ��Կ�����ɳɹ���" << std::endl;
    std::cout << "\n��Կ��Ϣ��" << std::endl;
    std::cout << "���� p = " << primePair.first << std::endl;
    std::cout << "���� q = " << primePair.second << std::endl;
    std::cout << "ģ�� n = " << rsa.getPublicKey().second << std::endl;
    
    // ������Կ
    if (rsa.saveKeys()) {
        std::cout << "\n? ��Կ�ѱ��浽�ļ���" << std::endl;
        std::cout << "  - public_key.txt (��Կ)" << std::endl;
        std::cout << "  - private_key.txt (˽Կ)" << std::endl;
    } else {
        std::cout << "\n? ��Կ����ʧ�ܣ�" << std::endl;
    }
}

void encryptText(const RSA& rsa) {
    std::cout << "\n=== �����ı� ===" << std::endl;
    
    if (!rsa.hasPublicKey()) {
        std::cout << "? ����δ���ع�Կ��" << std::endl;
        std::cout << "��ʾ��������ѡ��˵�ѡ�� 6 �������˹�Կ" << std::endl;
        return;
    }
    
    std::cout << "\n������Ҫ���ܵ��ı���" << std::endl;
    std::string plaintext;
    std::getline(std::cin, plaintext);
    
    if (plaintext.empty()) {
        std::cout << "? ����Ϊ�գ�" << std::endl;
        return;
    }
    
    try {
        std::cout << "\n���ڼ���..." << std::endl;
        std::string ciphertext = rsa.encrypt(plaintext);
        
        std::cout << "\n? ���ܳɹ���" << std::endl;
        std::cout << "\nԭʼ�ı���" << plaintext << std::endl;
        std::cout << "\n���ģ������Ʊ��ܣ���" << std::endl;
        std::cout << ciphertext << std::endl;
    } catch (const std::exception& e) {
        std::cout << "? ����ʧ�ܣ�" << e.what() << std::endl;
    }
}

void decryptText(const RSA& rsa) {
    std::cout << "\n=== �����ı� ===" << std::endl;
    
    if (!rsa.hasPrivateKey()) {
        std::cout << "? ����δ����˽Կ��" << std::endl;
        return;
    }
    
    std::cout << "\n������Ҫ���ܵ����ģ��������У��ո�ָ�����" << std::endl;
    std::string ciphertext;
    std::getline(std::cin, ciphertext);
    
    if (ciphertext.empty()) {
        std::cout << "? ����Ϊ�գ�" << std::endl;
        return;
    }
    
    try {
        std::cout << "\n���ڽ���..." << std::endl;
        std::string plaintext = rsa.decrypt(ciphertext);
        
        std::cout << "\n? ���ܳɹ���" << std::endl;
        std::cout << "\n���ܺ���ı���" << plaintext << std::endl;
    } catch (const std::exception& e) {
        std::cout << "? ����ʧ�ܣ�" << e.what() << std::endl;
    }
}

void importPublicKey(RSA& rsa) {
    std::cout << "\n=== �������˹�Կ ===" << std::endl;
    std::cout << "\nע�⣺�������˹�Կ����ֻ��������������Ϣ��" << std::endl;
    std::cout << "      ����Ҫ�Է���˽Կ���ܽ��ܣ�ͨ������Ӧӵ�жԷ�˽Կ����" << std::endl;
    std::cout << "\n�����빫Կ�ļ�����Ĭ��Ϊ public_key.txt��: ";
    
    std::string filename;
    std::getline(std::cin, filename);
    
    if (filename.empty()) {
        filename = "public_key.txt";
    }
    
    if (rsa.loadPublicKey(filename)) {
        std::cout << "\n? ��Կ����ɹ���" << std::endl;
        auto publicKey = rsa.getPublicKey();
        std::cout << "\n����Ĺ�Կ��Ϣ��" << std::endl;
        std::cout << "��Կָ�� e = " << publicKey.first << std::endl;
        std::cout << "ģ�� n = " << publicKey.second << std::endl;
        std::cout << "\n����������ʹ�ô˹�Կ������Ϣ�ˡ�" << std::endl;
    } else {
        std::cout << "\n? ��Կ����ʧ�ܣ������ļ��Ƿ�����Ҹ�ʽ��ȷ��" << std::endl;
        std::cout << "�ļ���ʽӦΪ��" << std::endl;
        std::cout << "  ��һ�У���Կָ�� e" << std::endl;
        std::cout << "  �ڶ��У�ģ�� n" << std::endl;
    }
}

void displayMenu() {
    std::cout << "\n==============================" << std::endl;
    std::cout << "    RSA ���ּӽ���ϵͳ" << std::endl;
    std::cout << "==============================" << std::endl;
    std::cout << "1. �����µ�RSA��Կ��" << std::endl;
    std::cout << "2. �����ı�" << std::endl;
    std::cout << "3. �����ı�" << std::endl;
    std::cout << "4. �鿴��Կ��Ϣ" << std::endl;
    std::cout << "5. ���¼�����Կ" << std::endl;
    std::cout << "6. �������˹�Կ�������ڼ��ܣ�" << std::endl;
    std::cout << "0. �˳�����" << std::endl;
    std::cout << "==============================" << std::endl;
    std::cout << "��ѡ����� (0-6): ";
}

int main() {
    RSA rsa;
    bool keysLoaded = false;
    
    std::cout << "\n��ӭʹ�� RSA ���ּӽ���ϵͳ��" << std::endl;
    std::cout << "================================\n" << std::endl;
    
    // ����Ƿ�������Կ�ļ�
    if (KeyManager::keysExist()) {
        std::cout << "��⵽�ѱ������Կ�ļ���" << std::endl;
        std::cout << "�Ƿ����������Կ��(y/n): ";
        
        char choice;
        std::cin >> choice;
        clearInputBuffer();
        
        if (choice == 'y' || choice == 'Y') {
            if (rsa.loadKeys()) {
                std::cout << "? ��Կ���سɹ���" << std::endl;
                keysLoaded = true;
                
                auto publicKey = rsa.getPublicKey();
                std::cout << "\n��ǰ��Կ��Ϣ��" << std::endl;
                std::cout << "ģ�� n = " << publicKey.second << std::endl;
            } else {
                std::cout << "? ��Կ����ʧ�ܣ�" << std::endl;
            }
        }
    }
    
    if (!keysLoaded) {
        std::cout << "\n�״�ʹ����Ҫ����RSA��Կ�ԡ�" << std::endl;
        std::cout << "�Ƿ��������ɣ�(y/n): ";
        
        char choice;
        std::cin >> choice;
        clearInputBuffer();
        
        if (choice == 'y' || choice == 'Y') {
            generateNewKeys(rsa);
            keysLoaded = true;
        }
    }
    
    // ��ѭ��
    while (true) {
        displayMenu();
        
        int choice;
        std::cin >> choice;
        clearInputBuffer();
        
        switch (choice) {
            case 1:
                generateNewKeys(rsa);
                keysLoaded = true;
                break;
                
            case 2:
                if (keysLoaded || rsa.hasPublicKey()) {
                    encryptText(rsa);
                } else {
                    std::cout << "\n? ����������Կ��������Կ���빫Կ��" << std::endl;
                }
                break;
                
            case 3:
                if (keysLoaded || rsa.hasPrivateKey()) {
                    decryptText(rsa);
                } else {
                    std::cout << "\n? �������ɻ������Կ��" << std::endl;
                }
                break;
                
            case 4:
                if (keysLoaded || rsa.hasPublicKey() || rsa.hasPrivateKey()) {
                    std::cout << std::endl;
                    rsa.displayKeys();
                } else {
                    std::cout << "\n? δ������Կ��" << std::endl;
                }
                break;
                
            case 5:
                if (rsa.loadKeys()) {
                    std::cout << "\n? ��Կ���¼��سɹ���" << std::endl;
                    keysLoaded = true;
                } else {
                    std::cout << "\n? ��Կ����ʧ�ܣ�" << std::endl;
                    std::cout << "����ԭ��" << std::endl;
                    std::cout << "  1. ��Կ�ļ�������" << std::endl;
                    std::cout << "  2. ��Կ��˽Կ��ģ�� n ��ƥ�䣨���Բ�ͬ��Կ�ԣ�" << std::endl;
                    std::cout << "\n��ʾ�����ֻ�뵼�빫Կ���ڼ��ܣ���ѡ��˵�ѡ�� 6" << std::endl;
                }
                break;
                
            case 6:
                importPublicKey(rsa);
                break;
                
            case 0:
                std::cout << "\n��лʹ�� RSA �ӽ���ϵͳ���ټ���" << std::endl;
                return 0;
                
            default:
                std::cout << "\n? ��Ч��ѡ�����������룡" << std::endl;
                break;
        }
        
        std::cout << "\n���س�������...";
        std::cin.get();
    }
    
    return 0;
}
