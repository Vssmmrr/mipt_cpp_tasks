#include <iostream>
#include <cstdlib>
#include <exception>
#include <cstring>
#include <cmath>
using namespace std;

template <class T>
T GCD(const T& first_number, const T& second_number) {
    return second_number == 0 ? first_number :
    GCD(second_number, first_number % second_number);
}

class RationalDivisionByZero :  public std::exception {
};


class Rational {
private:
    int p;
    int q; // is maintained to be positive

    void reduce() {
        int g = GCD(abs(p), q);
        p /= g;
        q /= g;
        if (q < 0) {
            p *= -1;
            q *= -1;
        }
    }

public:

//    void Reduce() {
//        reduce();
//    }

    Rational(int p_ = 0, int q_ = 1) : p(p_), q(q_) {
        reduce();
    }

    ~Rational() = default;

    int getNumerator() const {
        return p;
    }

    int getDenominator() const {
        return q;
    }

//    void setNumerator(int p_) {
//        p = p_;
//    }
//
//    void setDenominator(int q_) {
//        q = q_;
//    }

    Rational& operator++() {
        p += q;
        reduce();
        return *this;
    }

    Rational operator++(int) {
        Rational number = *this;
        ++(*this);
        return number;
    }

    Rational& operator--() {
        p -= q;
        reduce();
        return *this;
    }

    Rational operator--(int) {
        Rational number = *this;
        --(*this);
        return number;
    }

    Rational& operator+=(const Rational& other) {
        p = p * other.q + other.p * q;
        q *= other.q;
        reduce();
        return *this;
    }

    Rational& operator-=(const Rational& other) {
        p = p * other.q - other.p * q;
        q *= other.q;
        reduce();
        return *this;
    }

    Rational& operator*=(const Rational& other) {
        p = p * other.p;
        q = q * other.q;
        reduce();
        return *this;
    }

    Rational& operator/=(const Rational& other) {
        if (other.p == 0) {
            throw RationalDivisionByZero{};
        }
        p = p * other.q;
        q = q * other.p;
        reduce();
        return *this;
    }

    friend std::istream& operator>>(std::istream& is, Rational& number);
};

std::istream& operator>>(std::istream& is, Rational& number) {
    const int kMaxLength = 32; // 2^15
    char str[kMaxLength];
    is >> str;

    if (sscanf(str, "%d/%d", &number.p,
            &number.q) < 2 || number.q == 0) {
        number.q = 1;
    }
    number.reduce();
    return is;
}

std::ostream& operator<<(std::ostream& os, const Rational& number) {
    if (number.getDenominator() != 1) {
        os << number.getNumerator() << '/' << number.getDenominator();
    } else {
        os << number.getNumerator();
    }
    return os;
}

Rational operator-(const Rational& number) {
    return {-number.getNumerator(), number.getDenominator()};
}

Rational operator+(const Rational& number) {
    return number;
}

Rational operator+(const Rational& lhs, const Rational& rhs) {
    Rational sum = lhs;
    return sum += rhs;
}

Rational operator-(const Rational& lhs, const Rational& rhs) {
    Rational residual = lhs;
    return residual-= rhs;
}

Rational operator*(const Rational& lhs, const Rational& rhs) {
    Rational multiplier = lhs;
    return multiplier *= rhs;
}

Rational operator/(const Rational& lhs, const Rational& rhs) {
    Rational quotient = lhs;
    return quotient/= rhs;
}

bool operator>(const Rational& lhs, const Rational& rhs) {
    return lhs.getNumerator() * rhs.getDenominator() >
    rhs.getNumerator() * lhs.getDenominator();
}

bool operator<(const Rational &lhs, const Rational &rhs) {
    return rhs > lhs;
}

bool operator==(const Rational &lhs, const Rational &rhs) {
    return lhs.getNumerator() * rhs.getDenominator() ==
           rhs.getNumerator() * lhs.getDenominator();
}

bool operator!=(const Rational &lhs, const Rational &rhs) {
    return !(lhs == rhs);
}

bool operator>=(const Rational &lhs, const Rational &rhs) {
    return !(lhs < rhs);
}

bool operator<=(const Rational& lhs, const Rational& rhs) {
    return !(lhs > rhs);
}


int main() {
    int a;
    cin >> a;

    int p, q;
    cin >> p >> q;
    const Rational rc(p, q); // q != 0 is guaranteed by author of tests
    cout << rc.getNumerator() << ' ' << rc.getDenominator() << endl;

    Rational r1, r2;
    cin >> r1 >> r2;
    cout << r1 << endl;
    cout << r2 << endl;

    try {
        cout << 1/r1 << endl;
    } catch (const RationalDivisionByZero& ex) {
        cout << "Cannot get reciprocal of r1." << endl;
    }

    try {
        cout << rc/r2 << endl;
    } catch (const RationalDivisionByZero& ex) {
        cout << "Cannot divide by r2." << endl;
    }

    cout << (r1 < r2) << endl;
    cout << (r1 <= r2) << endl;
    cout << (r1 > r2) << endl;
    cout << (r1 >= r2) << endl;
    cout << (r1 == r2) << endl;
    cout << (r1 != r2) << endl;

    cout << (r1 < a) << endl;
    cout << (r1 <= a) << endl;
    cout << (r1 > a) << endl;
    cout << (r1 >= a) << endl;
    cout << (r1 == a) << endl;
    cout << (r1 != a) << endl;

    cout << (a < r2) << endl;
    cout << (a <= r2) << endl;
    cout << (a > r2) << endl;
    cout << (a >= r2) << endl;
    cout << (a == r2) << endl;
    cout << (a != r2) << endl;

    cout << rc + a << endl
         << a + rc << endl
         << -rc * r1 << endl
         << (+r1 - r2 * rc) * a << endl;

    cout << ++r1 << endl;
    cout << r1 << endl;
    cout << r1++ << endl;
    cout << r1 << endl;
    cout << --r1 << endl;
    cout << r1 << endl;
    cout << r1-- << endl;
    cout << r1 << endl;
    cout << ++++r1 << endl;
    cout << r1 << endl;

    cout << ((((r1 += r2) /= Rational(-5,3)) -= rc) *= a) << endl;
    cout << (r1 += r2 /= 3) << endl;
    cout << r1 << endl;
    cout << r2 << endl;
    return 0;
}
