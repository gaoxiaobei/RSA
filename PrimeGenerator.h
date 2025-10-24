#pragma once
#include "BigInt.h"
#include <random>
#include <ctime>
#include <vector>
#include <chrono>

class PrimeGenerator {
public:
    // ���ݰ�ȫ�ȼ��������������
    static std::pair<BigInt, BigInt> getSafePrimePair(int level = 1) {
        long long min_p, max_p, min_q, max_q;
        
        switch (level) {
            case 0: // ��ʾ����С������
                min_p = 50;
                max_p = 100;
                min_q = 50;
                max_q = 100;
                break;
            case 1: // �ͼ���Լ1000���ң�
                min_p = 900;
                max_p = 1100;
                min_q = 900;
                max_q = 1100;
                break;
            case 2: // �м���Լ10000���ң�
                min_p = 9000;
                max_p = 11000;
                min_q = 9000;
                max_q = 11000;
                break;
            case 3: // �߼���Լ100000���ң�
                min_p = 90000;
                max_p = 110000;
                min_q = 90000;
                max_q = 110000;
                break;
            case 4: // ���߼���Լ1000000���ң�
                min_p = 900000;
                max_p = 1100000;
                min_q = 900000;
                max_q = 1100000;
                break;
            default:
                min_p = 50;
                max_p = 100;
                min_q = 50;
                max_q = 100;
                break;
        }
        
        BigInt p = generatePrime(min_p, max_p);
        BigInt q = generatePrime(min_q, max_q);
        
        // ȷ�� p �� q �����
        while (p == q) {
            q = generatePrime(min_q, max_q);
        }
        
        return {p, q};
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

        // ���ж��ֲ���
        std::random_device rd;
        auto seed = rd() ^ static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
        std::mt19937_64 gen(seed);
        
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
        // ʹ�ø��õ�������ӣ����ʱ�������豸
        std::random_device rd;
        auto seed = rd() ^ (
            static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()) +
            static_cast<unsigned>(std::time(nullptr))
        );
        std::mt19937_64 gen(seed);
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
        
        // ���Ҳ�Ҳ���������Ԥ�������
        return BigInt("1009");
    }

    // ���������С����
    static BigInt generateSmallPrime(int min = 100, int max = 10000) {
        return generatePrime(min, max);
    }

    // ��ʾ�Ƽ��İ�ȫ����
    static void displaySecurityLevels() {
        std::cout << "��ȫ����ѡ�ÿ��������ɣ���" << std::endl;
        std::cout << "0 - ��ʾ�� (p,q �� [50, 100], n �� 10,000λ)" << std::endl;
        std::cout << "1 - �ͼ� (p,q �� [900, 1100], n �� 1,000,000λ)" << std::endl;
        std::cout << "2 - �м� (p,q �� [9000, 11000], n �� 100,000,000λ)" << std::endl;
        std::cout << "3 - �߼� (p,q �� [90000, 110000], n �� 10,000,000,000λ)" << std::endl;
        std::cout << "4 - ���߼� (p,q �� [900000, 1100000], n �� 1,000,000,000,000λ)" << std::endl;
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
