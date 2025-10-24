#pragma once
#include "BigInt.h"
#include <random>
#include <ctime>
#include <vector>

class PrimeGenerator {
public:
    // Ԥ����İ�ȫ�����ԣ����ڿ��ٳ�ʼ����
    static std::pair<BigInt, BigInt> getSafePrimePair(int level = 1) {
        switch (level) {
            case 1: // С�ͣ��ʺϿ��ٲ��ԣ�
                return {BigInt("1009"), BigInt("1013")};
            case 2: // ����
                return {BigInt("10007"), BigInt("10009")};
            case 3: // ����
                return {BigInt("100003"), BigInt("100019")};
            case 4: // ������
                return {BigInt("1000003"), BigInt("1000033")};
            default:
                return {BigInt("61"), BigInt("53")};
        }
    }

    // ����-�������Բ���
    static bool isProbablePrime(const BigInt& n, int iterations = 10) {
        if (n == BigInt(2) || n == BigInt(3)) return true;
        if (n < BigInt(2) || n.toString().back() % 2 == 0) return false;

        // �� n-1 д�� 2^r * d ����ʽ
        BigInt d = n - BigInt(1);
        int r = 0;
        while (d.toString().back() % 2 == 0) {
            d = d / BigInt(2);
            r++;
        }

        // ���ж�β���
        std::mt19937_64 gen(static_cast<unsigned>(std::time(nullptr)));
        
        for (int i = 0; i < iterations; i++) {
            // ��������� a �� [2, n-2]
            BigInt a = getRandomBigInt(2, n - BigInt(2), gen);
            BigInt x = BigInt::modPow(a, d, n);

            if (x == BigInt(1) || x == n - BigInt(1))
                continue;

            bool composite = true;
            for (int j = 0; j < r - 1; j++) {
                x = BigInt::modPow(x, BigInt(2), n);
                if (x == n - BigInt(1)) {
                    composite = false;
                    break;
                }
            }

            if (composite) return false;
        }

        return true;
    }

    // �����������������Χ�ڣ�
    static BigInt generatePrime(long long min, long long max) {
        std::mt19937_64 gen(static_cast<unsigned>(std::time(nullptr)));
        std::uniform_int_distribution<long long> dis(min, max);

        int attempts = 0;
        while (attempts < 10000) {
            long long candidate = dis(gen);
            if (candidate % 2 == 0) candidate++;
            
            BigInt n(candidate);
            if (isProbablePrime(n)) {
                return n;
            }
            attempts++;
        }
        
        // ����Ҳ���������Ԥ���������
        return BigInt("1009");
    }

    // ���������С����
    static BigInt generateSmallPrime(int min = 100, int max = 10000) {
        return generatePrime(min, max);
    }

    // ��ʾ�Ƽ��İ�ȫ����
    static void displaySecurityLevels() {
        std::cout << "��ȫ����ѡ�" << std::endl;
        std::cout << "0 - ��ʾ�� (p=61, q=53, n=3233)" << std::endl;
        std::cout << "1 - �ͼ� (p=1009, q=1013, n=1022117)" << std::endl;
        std::cout << "2 - �м� (p=10007, q=10009, n=100160063)" << std::endl;
        std::cout << "3 - �߼� (p=100003, q=100019, n=10002200057)" << std::endl;
        std::cout << "4 - ���߼� (p=1000003, q=1000033, n=1000036000099)" << std::endl;
    }

private:
    static BigInt getRandomBigInt(long long min, const BigInt& max, std::mt19937_64& gen) {
        long long max_val = 1000000LL;
        std::string max_str = max.toString();
        if (max_str.length() <= 9) {
            max_val = std::stoll(max_str);
        }
        
        std::uniform_int_distribution<long long> dis(min, std::min(max_val, 999999LL));
        return BigInt(dis(gen));
    }
};
