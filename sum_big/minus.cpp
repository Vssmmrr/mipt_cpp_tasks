#include <cstring>
#include <math.h>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <utility>

class BigIntegerOverflow {
};

class BigInteger {
private:
    const static size_t maxLength = 300000; // number of digits
    const static uint8_t baseLog = 1; // use 1 or 2 for debug

    typedef int32_t digit_type;
    typedef uint64_t double_digit_type; // for multiplication

    const static uint32_t base = static_cast<digit_type>(pow(10, baseLog));
    const static size_t maxReadSize = 20010; // for reading (buffer size)

    digit_type digits[maxLength];

    size_t amount;
    int sign;

    void Reverse() {
        for (size_t i = 0; i < amount / 2; ++i) {
            std::swap(digits[i], digits[amount - i - 1]);
        }
    }

    void Fill(size_t begin, size_t end, const int value) {
        for (size_t i = begin; i < end; ++i) {
            digits[i] = value;
        }
    }

public:
    BigInteger() {
        amount = 0;
        sign = 1;
        Fill(0, maxLength, 0);
    }

    BigInteger(const long long input) {
        long long number = fabs(input);
        if (input < 0) {
            sign = -1;
        } else {
            sign = 1;
        }

        size_t i;
        for (i = 0; number != 0; ++i) {
            digits[i] = number % baseLog;
            number /= baseLog;
        }

        amount = i;
        Fill(amount, maxLength, 0);
    }

    BigInteger(const char* input) {
        ssize_t finish = 0;
        if (input[0] == '-') {
            sign = -1;
            finish = 1;
        } else {
            sign = 1;
        }

        amount = strlen(input);
        size_t j = 0;
        for (ssize_t i = amount - 1; i >= finish; --i) {
            digits[j] = input[i] - '0';
            ++j;
        }

        amount = j;
        Fill(amount, maxLength, 0);
    }


    ~BigInteger(){}

    bool operator>(const BigInteger& input) const {
        if (sign != input.sign) {
            return sign > input.sign;
        }

        if (amount != input.amount) {
            return amount > input.amount;
        }
        else {
            for (size_t i = 0; i < amount; ++i) {
                if (digits[i] != input.digits[i]) {
                    return digits[i] > input.digits[i];
                }
            }
            return false;
        }
    }

    bool operator==(const BigInteger& input) const {
        if (sign != input.sign) {
            return false;
        }

        if (amount != input.amount) {
            return false;
        }
        else {
            for (size_t i = 0; i < amount; ++i) {
                if (digits[i] != input.digits[i]) {
                    return false;
                }
            }
            return true;
        }
    }

    size_t Size() const {
        return amount;
    }

    int operator[](size_t idx) const {
        return digits[idx];
    }

    friend std::istream& operator>>(std::istream& is, BigInteger& value) {
        char str[maxReadSize] = {};
        is >> str;
        BigInteger tmp(str);
        value = tmp;
        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, const BigInteger& num) {
        if (num.amount == 0) {
            os << 0;
        } else {
            if (num.sign == -1) {
                os << '-';
            }
            os << static_cast<BigInteger::double_digit_type>(num[num.amount - 1]);
            for (size_t i = num.amount - 1; i >= 1; --i) {
                os.width(baseLog);
                os.fill('0');
                os << static_cast<BigInteger::double_digit_type>(num[i - 1]);
            }
        }
        return os;
    }

    BigInteger operator-(const BigInteger& input) {
        BigInteger result(*this);
        result.sign = 1;
        for (size_t i = 0; i < result.amount; ++i) {
            result.digits[i] -= input.digits[i];
            if (result.digits[i] < 0) {
                result.digits[i] += base;
                --result.digits[i + 1];
            }
        }

        size_t pos = result.amount;
        while (pos && !result.digits[pos]) {
            --pos;
        }
        result.amount = pos + 1;
        return result;
    }
};

int main() {
    BigInteger first;
    BigInteger second;
    std::cin >> first >> second;

    BigInteger minus(first - second);
    std::cout << minus << '\n';

    return 0;
}

