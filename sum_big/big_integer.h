#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <stdexcept>
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
    const static size_t maxLength = 3000; // number of digits
    const static uint8_t baseLog = 1; // use 1 or 2 for debug

    typedef uint32_t digit_type;
    typedef uint64_t double_digit_type; // for multiplication

    const static uint32_t base = static_cast<digit_type>(pow(10, baseLog));
    //const static uint32_t base;  // base of numeral system
    const static size_t maxReadSize = 20010; // for reading (buffer size)

    digit_type digits[maxLength];

    size_t amount;
    bool is_negative_ = false;

    static BigInteger SumPositives(const BigInteger& lhs, const BigInteger& rhs);
    static BigInteger MinusPositives(const BigInteger& lhs, const BigInteger& rhs);

    static void DividePositives(const BigInteger& lhs, const BigInteger& rhs,
                                BigInteger& result, BigInteger& remainder);

    static bool AbsLess(const BigInteger& lhs, const BigInteger& rhs);

    template <class IntegerType>
    void Set(IntegerType number);

    void Set(const char* number_str);

    //void Move(size_t offset);

    void Reverse();

    static digit_type Divide(const BigInteger& lhs, const BigInteger& rhs);

public:

    digit_type operator[](size_t idx) const;
    digit_type& operator[](size_t idx);


    template <class IntegerType = int>
    BigInteger(IntegerType number = 0);
    BigInteger(const char* number_str);

    bool IsNegative() const;
    size_t Size() const;

    BigInteger& operator+=(const BigInteger& value);
    BigInteger& operator-=(const BigInteger& value);
    BigInteger& operator/=(const BigInteger& value);
    BigInteger& operator*=(const BigInteger& value);
    BigInteger& operator%=(const BigInteger& value);

    BigInteger& operator++();
    BigInteger operator++(int);

    BigInteger& operator--();
    BigInteger operator--(int);

    friend BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs);
    friend BigInteger operator-(const BigInteger& lhs, const BigInteger& rhs);
    friend BigInteger operator*(const BigInteger& lhs, const BigInteger& rhs);
    friend BigInteger operator/(const BigInteger& lhs, const BigInteger& rhs);
    friend BigInteger operator%(const BigInteger& lhs, const BigInteger& rhs);

    friend BigInteger abs(const BigInteger& value);
    friend int sign(const BigInteger& value);

    friend bool operator<(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator>(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator>=(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator<=(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator==(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator!=(const BigInteger& lhs, const BigInteger& rhs);

    friend std::ostream& operator<<(std::ostream& os, const BigInteger& value);
    friend std::istream& operator>>(std::istream& is, BigInteger& value);

};

#endif //BIG_INTEGER_H
