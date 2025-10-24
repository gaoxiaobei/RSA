#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

class BigInt {
public:
    BigInt() : digits(1, 0), negative(false) {}
    BigInt(const std::string& num) { fromString(num); }
    BigInt(long long num) { fromLongLong(num); }

    // 加法
    BigInt operator+(const BigInt& other) const {
        if (negative == other.negative) {
            BigInt result = addAbs(*this, other);
            result.negative = negative;
            return result;
        } else {
            if (absGreaterOrEqual(*this, other)) {
                BigInt result = subAbs(*this, other);
                result.negative = negative;
                return result;
            } else {
                BigInt result = subAbs(other, *this);
                result.negative = other.negative;
                return result;
            }
        }
    }

    // 减法
    BigInt operator-(const BigInt& other) const {
        if (negative != other.negative) {
            BigInt result = addAbs(*this, other);
            result.negative = negative;
            return result;
        } else {
            if (absGreaterOrEqual(*this, other)) {
                BigInt result = subAbs(*this, other);
                result.negative = negative;
                return result;
            } else {
                BigInt result = subAbs(other, *this);
                result.negative = !other.negative;
                return result;
            }
        }
    }

    // 乘法
    BigInt operator*(const BigInt& other) const {
        BigInt result;
        result.digits.clear();
        result.digits.resize(digits.size() + other.digits.size(), 0);
        
        for (size_t i = 0; i < digits.size(); ++i) {
            int carry = 0;
            for (size_t j = 0; j < other.digits.size() || carry; ++j) {
                long long cur = result.digits[i + j] + 
                    (long long)digits[i] * (j < other.digits.size() ? other.digits[j] : 0) + carry;
                result.digits[i + j] = cur % 10;
                carry = cur / 10;
            }
        }
        
        result.negative = (negative != other.negative);
        return result.trim();
    }

    // 除法
    BigInt operator/(const BigInt& other) const {
        if (other == BigInt(0)) throw std::runtime_error("Division by zero");
        return divMod(other).first;
    }

    // 取模
    BigInt operator%(const BigInt& other) const {
        if (other == BigInt(0)) throw std::runtime_error("Division by zero");
        BigInt result = divMod(other).second;
        if (result.negative && !result.isZero()) {
            result = other - result;
            result.negative = false;
        }
        return result;
    }

    // 比较运算符
    bool operator==(const BigInt& other) const {
        return negative == other.negative && digits == other.digits;
    }

    bool operator!=(const BigInt& other) const {
        return !(*this == other);
    }

    bool operator<(const BigInt& other) const {
        if (negative != other.negative) return negative;
        if (digits.size() != other.digits.size()) 
            return negative ? digits.size() > other.digits.size() : digits.size() < other.digits.size();
        for (size_t i = digits.size(); i-- > 0;) {
            if (digits[i] != other.digits[i])
                return negative ? digits[i] > other.digits[i] : digits[i] < other.digits[i];
        }
        return false;
    }

    bool operator<=(const BigInt& other) const {
        return *this < other || *this == other;
    }

    bool operator>(const BigInt& other) const {
        return !(*this <= other);
    }

    bool operator>=(const BigInt& other) const {
        return !(*this < other);
    }

    // 模幂运算 (a^b mod m)
    static BigInt modPow(const BigInt& base, const BigInt& exp, const BigInt& mod) {
        if (mod == BigInt(1)) return BigInt(0);
        
        BigInt result(1);
        BigInt b = base % mod;
        BigInt e = exp;
        
        while (e > BigInt(0)) {
            if (e.digits[0] % 2 == 1) {
                result = (result * b) % mod;
            }
            e = e / BigInt(2);
            b = (b * b) % mod;
        }
        
        return result;
    }

    // 最大公约数
    static BigInt gcd(BigInt a, BigInt b) {
        a.negative = false;
        b.negative = false;
        while (b != BigInt(0)) {
            BigInt temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    // 扩展欧几里得算法
    static BigInt modInverse(const BigInt& a, const BigInt& m) {
        BigInt m0 = m;
        BigInt x0(0), x1(1);
        
        if (m == BigInt(1)) return BigInt(0);
        
        BigInt aa = a;
        BigInt mm = m;
        
        while (aa > BigInt(1)) {
            BigInt q = aa / mm;
            BigInt t = mm;
            
            mm = aa % mm;
            aa = t;
            
            t = x0;
            x0 = x1 - q * x0;
            x1 = t;
        }
        
        if (x1 < BigInt(0)) x1 = x1 + m0;
        
        return x1;
    }

    // 转换为字符串
    std::string toString() const {
        std::string result;
        if (negative && !(digits.size() == 1 && digits[0] == 0)) result += "-";
        for (size_t i = digits.size(); i-- > 0;) result += char('0' + digits[i]);
        return result;
    }

    bool isZero() const {
        return digits.size() == 1 && digits[0] == 0;
    }

    friend std::ostream& operator<<(std::ostream& os, const BigInt& num) {
        os << num.toString();
        return os;
    }

private:
    std::vector<int> digits; // 逆序存储，每位0-9
    bool negative;

    void fromString(const std::string& num) {
        digits.clear();
        negative = false;
        size_t i = 0;
        if (num.empty()) {
            digits.push_back(0);
            return;
        }
        if (num[0] == '-') {
            negative = true;
            i = 1;
        }
        for (size_t j = num.size(); j > i; --j) {
            if (isdigit(num[j - 1])) digits.push_back(num[j - 1] - '0');
        }
        if (digits.empty()) digits.push_back(0);
        trim();
    }

    void fromLongLong(long long num) {
        digits.clear();
        negative = num < 0;
        if (num == 0) {
            digits.push_back(0);
            return;
        }
        if (negative) num = -num;
        while (num > 0) {
            digits.push_back(num % 10);
            num /= 10;
        }
    }

    static BigInt addAbs(const BigInt& a, const BigInt& b) {
        BigInt result;
        result.digits.clear();
        int carry = 0;
        size_t i = 0;
        while (i < a.digits.size() || i < b.digits.size() || carry) {
            int sum = carry;
            if (i < a.digits.size()) sum += a.digits[i];
            if (i < b.digits.size()) sum += b.digits[i];
            result.digits.push_back(sum % 10);
            carry = sum / 10;
            ++i;
        }
        return result.trim();
    }

    static BigInt subAbs(const BigInt& a, const BigInt& b) {
        BigInt result;
        result.digits.clear();
        int borrow = 0;
        size_t i = 0;
        while (i < a.digits.size()) {
            int diff = a.digits[i] - borrow;
            if (i < b.digits.size()) diff -= b.digits[i];
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result.digits.push_back(diff);
            ++i;
        }
        return result.trim();
    }

    static bool absGreaterOrEqual(const BigInt& a, const BigInt& b) {
        if (a.digits.size() != b.digits.size()) return a.digits.size() > b.digits.size();
        for (size_t i = a.digits.size(); i-- > 0;) {
            if (a.digits[i] != b.digits[i]) return a.digits[i] > b.digits[i];
        }
        return true;
    }

    // 除法和取模
    std::pair<BigInt, BigInt> divMod(const BigInt& divisor) const {
        if (divisor.isZero()) throw std::runtime_error("Division by zero");
        
        BigInt dividend = *this;
        dividend.negative = false;
        BigInt div = divisor;
        div.negative = false;
        
        if (dividend < div) {
            return {BigInt(0), *this};
        }
        
        BigInt quotient(0);
        BigInt remainder(0);
        
        for (size_t i = dividend.digits.size(); i-- > 0;) {
            remainder = remainder * BigInt(10) + BigInt(dividend.digits[i]);
            
            int count = 0;
            while (remainder >= div) {
                remainder = remainder - div;
                count++;
            }
            
            quotient.digits.insert(quotient.digits.begin(), count);
        }
        
        quotient.negative = (negative != divisor.negative);
        remainder.negative = negative;
        
        return {quotient.trim(), remainder.trim()};
    }

    BigInt& trim() {
        while (digits.size() > 1 && digits.back() == 0) digits.pop_back();
        if (digits.size() == 1 && digits[0] == 0) negative = false;
        return *this;
    }
};
