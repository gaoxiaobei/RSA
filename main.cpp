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
    std::cout << "\n=== 生成新的RSA密钥对 ===" << std::endl << std::endl;
    
    PrimeGenerator::displaySecurityLevels();
    std::cout << "\n请选择安全级别 (0-4): ";
    
    int level;
    std::cin >> level;
    clearInputBuffer();
    
    if (level < 0 || level > 4) {
        level = 1;
        std::cout << "无效选择，使用默认级别 1" << std::endl;
    }
    
    std::cout << "\n正在生成密钥对..." << std::endl;
    
    auto primePair = PrimeGenerator::getSafePrimePair(level);
    rsa.initialize(primePair.first, primePair.second);
    
    std::cout << "? 密钥对生成成功！" << std::endl;
    std::cout << "\n密钥信息：" << std::endl;
    std::cout << "素数 p = " << primePair.first << std::endl;
    std::cout << "素数 q = " << primePair.second << std::endl;
    std::cout << "模数 n = " << rsa.getPublicKey().second << std::endl;
    
    // 保存密钥
    if (rsa.saveKeys()) {
        std::cout << "\n? 密钥已保存到文件：" << std::endl;
        std::cout << "  - public_key.txt (公钥)" << std::endl;
        std::cout << "  - private_key.txt (私钥)" << std::endl;
    } else {
        std::cout << "\n? 密钥保存失败！" << std::endl;
    }
}

void encryptText(const RSA& rsa) {
    std::cout << "\n=== 加密文本 ===" << std::endl;
    
    if (!rsa.hasPublicKey()) {
        std::cout << "? 错误：未加载公钥！" << std::endl;
        std::cout << "提示：您可以选择菜单选项 6 导入他人公钥" << std::endl;
        return;
    }
    
    std::cout << "\n请输入要加密的文本：" << std::endl;
    std::string plaintext;
    std::getline(std::cin, plaintext);
    
    if (plaintext.empty()) {
        std::cout << "? 输入为空！" << std::endl;
        return;
    }
    
    try {
        std::cout << "\n正在加密..." << std::endl;
        std::string ciphertext = rsa.encrypt(plaintext);
        
        std::cout << "\n? 加密成功！" << std::endl;
        std::cout << "\n原始文本：" << plaintext << std::endl;
        std::cout << "\n密文（请妥善保管）：" << std::endl;
        std::cout << ciphertext << std::endl;
    } catch (const std::exception& e) {
        std::cout << "? 加密失败：" << e.what() << std::endl;
    }
}

void decryptText(const RSA& rsa) {
    std::cout << "\n=== 解密文本 ===" << std::endl;
    
    if (!rsa.hasPrivateKey()) {
        std::cout << "? 错误：未加载私钥！" << std::endl;
        return;
    }
    
    std::cout << "\n请输入要解密的密文（数字序列，空格分隔）：" << std::endl;
    std::string ciphertext;
    std::getline(std::cin, ciphertext);
    
    if (ciphertext.empty()) {
        std::cout << "? 输入为空！" << std::endl;
        return;
    }
    
    try {
        std::cout << "\n正在解密..." << std::endl;
        std::string plaintext = rsa.decrypt(ciphertext);
        
        std::cout << "\n? 解密成功！" << std::endl;
        std::cout << "\n解密后的文本：" << plaintext << std::endl;
    } catch (const std::exception& e) {
        std::cout << "? 解密失败：" << e.what() << std::endl;
    }
}

void importPublicKey(RSA& rsa) {
    std::cout << "\n=== 导入他人公钥 ===" << std::endl;
    std::cout << "\n注意：导入他人公钥后，您只能用它来加密消息。" << std::endl;
    std::cout << "      您需要对方的私钥才能解密（通常您不应拥有对方私钥）。" << std::endl;
    std::cout << "\n请输入公钥文件名（默认为 public_key.txt）: ";
    
    std::string filename;
    std::getline(std::cin, filename);
    
    if (filename.empty()) {
        filename = "public_key.txt";
    }
    
    if (rsa.loadPublicKey(filename)) {
        std::cout << "\n? 公钥导入成功！" << std::endl;
        auto publicKey = rsa.getPublicKey();
        std::cout << "\n导入的公钥信息：" << std::endl;
        std::cout << "公钥指数 e = " << publicKey.first << std::endl;
        std::cout << "模数 n = " << publicKey.second << std::endl;
        std::cout << "\n现在您可以使用此公钥加密消息了。" << std::endl;
    } else {
        std::cout << "\n? 公钥导入失败！请检查文件是否存在且格式正确。" << std::endl;
        std::cout << "文件格式应为：" << std::endl;
        std::cout << "  第一行：公钥指数 e" << std::endl;
        std::cout << "  第二行：模数 n" << std::endl;
    }
}

void displayMenu() {
    std::cout << "\n==============================" << std::endl;
    std::cout << "    RSA 文字加解密系统" << std::endl;
    std::cout << "==============================" << std::endl;
    std::cout << "1. 生成新的RSA密钥对" << std::endl;
    std::cout << "2. 加密文本" << std::endl;
    std::cout << "3. 解密文本" << std::endl;
    std::cout << "4. 查看密钥信息" << std::endl;
    std::cout << "5. 重新加载密钥" << std::endl;
    std::cout << "6. 导入他人公钥（仅用于加密）" << std::endl;
    std::cout << "0. 退出程序" << std::endl;
    std::cout << "==============================" << std::endl;
    std::cout << "请选择操作 (0-6): ";
}

int main() {
    RSA rsa;
    bool keysLoaded = false;
    
    std::cout << "\n欢迎使用 RSA 文字加解密系统！" << std::endl;
    std::cout << "================================\n" << std::endl;
    
    // 检查是否已有密钥文件
    if (KeyManager::keysExist()) {
        std::cout << "检测到已保存的密钥文件。" << std::endl;
        std::cout << "是否加载已有密钥？(y/n): ";
        
        char choice;
        std::cin >> choice;
        clearInputBuffer();
        
        if (choice == 'y' || choice == 'Y') {
            if (rsa.loadKeys()) {
                std::cout << "? 密钥加载成功！" << std::endl;
                keysLoaded = true;
                
                auto publicKey = rsa.getPublicKey();
                std::cout << "\n当前密钥信息：" << std::endl;
                std::cout << "模数 n = " << publicKey.second << std::endl;
            } else {
                std::cout << "? 密钥加载失败！" << std::endl;
            }
        }
    }
    
    if (!keysLoaded) {
        std::cout << "\n首次使用需要生成RSA密钥对。" << std::endl;
        std::cout << "是否现在生成？(y/n): ";
        
        char choice;
        std::cin >> choice;
        clearInputBuffer();
        
        if (choice == 'y' || choice == 'Y') {
            generateNewKeys(rsa);
            keysLoaded = true;
        }
    }
    
    // 主循环
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
                    std::cout << "\n? 请先生成密钥、加载密钥或导入公钥！" << std::endl;
                }
                break;
                
            case 3:
                if (keysLoaded || rsa.hasPrivateKey()) {
                    decryptText(rsa);
                } else {
                    std::cout << "\n? 请先生成或加载密钥！" << std::endl;
                }
                break;
                
            case 4:
                if (keysLoaded || rsa.hasPublicKey() || rsa.hasPrivateKey()) {
                    std::cout << std::endl;
                    rsa.displayKeys();
                } else {
                    std::cout << "\n? 未加载密钥！" << std::endl;
                }
                break;
                
            case 5:
                if (rsa.loadKeys()) {
                    std::cout << "\n? 密钥重新加载成功！" << std::endl;
                    keysLoaded = true;
                } else {
                    std::cout << "\n? 密钥加载失败！" << std::endl;
                    std::cout << "可能原因：" << std::endl;
                    std::cout << "  1. 密钥文件不存在" << std::endl;
                    std::cout << "  2. 公钥和私钥的模数 n 不匹配（来自不同密钥对）" << std::endl;
                    std::cout << "\n提示：如果只想导入公钥用于加密，请选择菜单选项 6" << std::endl;
                }
                break;
                
            case 6:
                importPublicKey(rsa);
                break;
                
            case 0:
                std::cout << "\n感谢使用 RSA 加解密系统！再见！" << std::endl;
                return 0;
                
            default:
                std::cout << "\n? 无效的选择，请重新输入！" << std::endl;
                break;
        }
        
        std::cout << "\n按回车键继续...";
        std::cin.get();
    }
    
    return 0;
}
