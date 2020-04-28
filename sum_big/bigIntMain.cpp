#include "big_integer.h"
#include <stdexcept>
#include <cstring>
#include <math.h>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <utility>

typedef uint32_t digit_type;
typedef uint64_t double_digit_type;

digit_type BigInteger::operator[](size_t idx) const {
    return digits[idx];
}

digit_type& BigInteger::operator[](size_t idx) {
    return digits[idx];
}

bool BigInteger::AbsLess(const BigInteger& lhs, const BigInteger& rhs) {
    if (lhs.amount != rhs.amount) {
        return lhs.amount < rhs.amount;
    }

    for (size_t i = lhs.amount; i >= 1; --i) {
        if (lhs[i - 1] != rhs[i - 1]) {
            return lhs[i - 1] < rhs[i - 1];
        }
    }
    return false;
}

BigInteger BigInteger::SumPositives(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger result;
    result.amount = std::max(lhs.amount, rhs.amount);

    digit_type residual = 0;
    for (size_t i = 0; i < result.amount; ++i) {
        digit_type value = lhs.digits[i] + rhs.digits[i] + residual;
        result[i] = value % base;
        residual = value / base;
    }
    if (residual != 0) {
        if (result.amount < maxLength) {
            ++result.amount;
            result[result.amount - 1] = residual;
        } else {
            throw BigIntegerOverflow();
        }
    }
    return result;
}

BigInteger BigInteger::MinusPositives(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger res;
//    if (AbsLess(lhs, rhs)) {
//        res.is_negative_ = true;
//    }

    res.amount = lhs.amount;
    bool transfered = false;
    for (size_t i = 0; i < lhs.amount; ++i) {
        digit_type lhs_digit = lhs[i];
        digit_type rhs_digit = rhs[i];

        if (transfered) {
            rhs_digit += 1;
            transfered = false;
        }

        if (lhs_digit < rhs_digit) {
            transfered = true;
            lhs_digit += base;
        }

        res[i] = lhs_digit - rhs_digit;
    }

    while (res.amount > 0 && res[res.amount - 1] == 0) {
        --res.amount;
    }
    return res;
}

bool BigInteger::IsNegative() const {
    return is_negative_;
}

size_t BigInteger::Size() const {
    return amount;
}

BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs) {
    if (!lhs.is_negative_ && !rhs.is_negative_) {
        return BigInteger::SumPositives(lhs, rhs);
    } else if (lhs.is_negative_ && rhs.is_negative_) {
        return -BigInteger::SumPositives(lhs, rhs);
    } else if (lhs.is_negative_) {
        return BigInteger::SubtractPositives(rhs, lhs);
    } else {
        return BigInteger::SubtractPositives(lhs, rhs);
    }
}