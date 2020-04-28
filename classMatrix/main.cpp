#include <iostream>
#include <cstdlib>
#include <exception>
#include <cmath>
#include <stdexcept>

class MatrixAllocationError : public std::bad_alloc {
};

class MatrixWrongSizeError : public std::domain_error {
public:
    MatrixWrongSizeError() : std::domain_error("Incorrect matrix size") {
    }
};

class MatrixIndexError : public std::out_of_range {
public:
    MatrixIndexError() : std::out_of_range("Incorrect matrix index") {
    }
};

// non-specified functions to get "zero" of type T

template <typename T> T getZero() {
    return T(0);
}

//=============== Matrix class ===============//

template <typename T>
class Matrix {
private:
    int rowsCnt = 0;
    int colsCnt = 0;
    T **array = nullptr;

    void MemoryAllocation() {
        array = new T*[rowsCnt];
        for (int i = 0; i < rowsCnt; ++i) {
            array[i] = new T[colsCnt];
        }
    }

    void Copy(const Matrix& otherMatrix) {
        rowsCnt = otherMatrix.rowsCnt;
        colsCnt = otherMatrix.colsCnt;
        MemoryAllocation();
        for (int i = 0; i < rowsCnt; ++i) {
            for (int j = 0; j < colsCnt; ++j) {
                array[i][j] = otherMatrix.array[i][j];
            }
        }
    }

    void Clear() {
        if (array != nullptr) {
            for (int i = 0; i < rowsCnt; ++i) {
                delete[] array[i];
            }
            delete[] array;
            array = nullptr;
        }
        rowsCnt = 0;
        colsCnt = 0;
    }

public:
    Matrix() = delete;

    Matrix( int rows, int columns) : rowsCnt(rows), colsCnt(columns) {
        MemoryAllocation();
        SetZeroMatrix();
    }

    Matrix(const Matrix& copyMatrix) {
        Copy(copyMatrix);
    }

    Matrix& operator=(const Matrix& otherMatrix) {
        if (&otherMatrix == this) {
            return *this;
        }
        Clear();
        Copy(otherMatrix);
        return *this;
    }

    ~Matrix() {
        Clear();
    };

    int getRowsNumber() const {
        return rowsCnt;
    }

    int getColumnsNumber() const {
        return colsCnt;
    }

    void SetZeroMatrix() {
        for (int i = 0; i < rowsCnt; ++i) {
            for (int j = 0; j < colsCnt; ++j) {
                array[i][j] = 0;
            }
        }
    }


    Matrix& operator+=(const Matrix& otherMatrix) {
        if (rowsCnt != otherMatrix.rowsCnt || colsCnt != otherMatrix.colsCnt) {
            throw MatrixWrongSizeError();
        }
        for (int i = 0; i < rowsCnt; ++i) {
            for (int j = 0; j < colsCnt; ++j) {
                array[i][j] += otherMatrix.array[i][j];
            }
        }
        return *this;
    }

    Matrix& operator-=(const Matrix& otherMatrix) {
        if (rowsCnt != otherMatrix.rowsCnt || colsCnt != otherMatrix.colsCnt) {
            throw MatrixWrongSizeError();
        }
        for (int i = 0; i < rowsCnt; ++i) {
            for (int j = 0; j < colsCnt; ++j) {
                array[i][j] -= otherMatrix.array[i][j];
            }
        }
        return *this;
    }

    Matrix& operator*=(const T& value) {
        for (int i = 0; i < rowsCnt; ++i) {
            for (int j = 0; j < colsCnt; ++j) {
                array[i][j] *= value;
            }
        }
        return *this;
    }

    Matrix& operator*=(const Matrix& otherMatrix) {
        *this = *this * otherMatrix;
        return *this;
    }

    T operator()(const int i, const int j) const {
        if (i < 0 || i >= rowsCnt || j < 0 || j >= colsCnt) {
            throw MatrixIndexError();
        }
        return array[i][j];
    }

    T& operator()(const int i, const int j) {
        if (i < 0 || i >= rowsCnt || j < 0 || j >= colsCnt) {
            throw MatrixIndexError();
        }
        return array[i][j];
    }

    Matrix getTransposed() const {
        Matrix result(colsCnt, rowsCnt);
        for (int i = 0; i < colsCnt; ++i) {
            for(int j = 0; j < rowsCnt; ++j) {
                result.array[i][j] = array[j][i];
            }
        }
        return result;
    }

    Matrix& transpose() {
        *this = getTransposed();
        return *this;
    }

};

template <class T>
std::istream& operator>>(std::istream& is, Matrix<T>& inputMatrix) {
    for (int i = 0; i < inputMatrix.getRowsNumber(); ++i) {
        for (int j = 0; j < inputMatrix.getColumnsNumber(); ++j) {
            is >> inputMatrix(i, j);
        }
    }
    return is;
}

template <class T>
std::ostream& operator<<(std::ostream& os,const Matrix<T>& outputMatrix) {
    for (int i = 0; i < outputMatrix.getRowsNumber(); ++i) {
        for (int j = 0; j < outputMatrix.getColumnsNumber(); ++j) {
            os << outputMatrix(i, j) << ' ';
        }
        os << '\n';
    }
    return os;
}

template <class T>
Matrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    Matrix<T> result = lhs;
    result += rhs;
    return result;
}

template <class T>
Matrix<T> operator-(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    Matrix<T> result = lhs;
    result -= rhs;
    return result;
}

template <class T>
Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    if (lhs.getColumnsNumber() != rhs.getRowsNumber()) {
        throw MatrixWrongSizeError();
    }

    Matrix<T> result(lhs.getRowsNumber(), rhs.getColumnsNumber());
    for (int i = 0; i < lhs.getRowsNumber(); ++i) {
        for (int j = 0; j < rhs.getColumnsNumber(); ++j) {
            for (int k = 0; k < lhs.getColumnsNumber(); ++k) {
                result(i, j) += lhs(i, k) * rhs(k, j);
            }
        }
    }
    return result;
}

template <class T>
Matrix<T> operator*(const Matrix<T>& otherMatrix, const T& value) {
    Matrix<T> result = otherMatrix;
    result *= value;
    return result;
}

template <class T>
Matrix<T> operator*(const T& value, const Matrix<T>& otherMatrix) {
    Matrix<T> result = otherMatrix;
    result *= value;
    return result;
}

//================ class Rational ===============//

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

//=================== main() ===============//

using namespace std;

int main() {
    int m, n, p, q;
    cin >> m >> n >> p >> q;

    Matrix<int> A(m, n), B(p, q);
    cin >> A >> B;

    A = A;

    try {
        cout << A + B * 2 - m * A << endl;
        cout << (A -= B += A *= 2) << endl;
        cout << (((A -= B) += A) *= 2) << endl;
        //std::cout << A * B << '\n';
    } catch (const MatrixWrongSizeError&) {
        cout << "A and B are of different size." << endl;
    }
    B = A;

    {
        Matrix<int> AA(A);
        Matrix<int> AAA(1, 1);
        AAA = A;
        cout << AA << endl;
        cout << (AAA += Matrix<int>(m, n)) + B << endl;
    }

    Rational r;
    cin >> r;
    Matrix<Rational> C(m, n), D(p, q);
    cin >> C >> D;
    try {
        cout << C * D << endl;
        cout << (C *= D) << endl;
        cout << C << endl;
    } catch (const MatrixWrongSizeError&) {
        cout << "C and D have not appropriate sizes for multiplication." << endl;
    }
    cout << C.getTransposed() * (r * C) << endl;
    cout << C.transpose() << endl;
    try {
        (C(0, 0) *= 6) /= 3;
        cout << C(0, 0) << endl;
        cout << C(m, m) << endl;
    } catch (const MatrixIndexError&) {
        cout << "Index out of range." << endl;
    }

    {
        const Matrix<Rational>& rC = C;
        cout << rC << endl;
        cout << rC.getRowsNumber() << ' ' << rC.getColumnsNumber() << ' ' << rC(0, 0) << endl;
        cout << (C = C) * (Rational(1, 2) * rC).getTransposed() << endl;
    }
    return 0;
}
