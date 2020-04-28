#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <utility>

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


class MatrixIsDegenerateError : public std::range_error {
public:
    MatrixIsDegenerateError() : std::range_error("The matrix is degenerate"){
    }
};

// non-specified functions to get "zero" and "one" of type T

template <typename T> T getZero() {
    return T(0);
}

template <typename T> T getOne() {
    return T(1);
}

//=============== Matrix class ===============//

template <typename T>
class Matrix {
protected:
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

    virtual ~Matrix() {
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

    virtual Matrix& operator+=(const Matrix& otherMatrix) {
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

    virtual Matrix& operator-=(const Matrix& otherMatrix) {
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

    virtual Matrix& operator*=(const T& value) {
        for (int i = 0; i < rowsCnt; ++i) {
            for (int j = 0; j < colsCnt; ++j) {
                array[i][j] *= value;
            }
        }
        return *this;
    }

    virtual Matrix& operator*=(const Matrix& otherMatrix) {
        *this = *this * otherMatrix;
        return *this;
    }

    T operator()(int i,int j) const {
        if (i < 0 || i >= rowsCnt || j < 0 || j >= colsCnt) {
            throw MatrixIndexError();
        }
        return array[i][j];
    }

    T& operator()(int i, int j) {
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

    virtual Matrix& transpose() {
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
std::ostream& operator<<(std::ostream& os, const Matrix<T>& outputMatrix) {
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
//=============== SquareMatrix class ===============//

template <typename T>
class SquareMatrix : public Matrix<T> {
private:

public:
    SquareMatrix(int size) : Matrix<T>(size, size) {
    }

    SquareMatrix(const Matrix<T>& otherMatrix) : Matrix<T>(otherMatrix) {
        if (otherMatrix.getRowsNumber() == otherMatrix.getColumnsNumber()) {
            this->Copy(otherMatrix);
        } else {
            this->Clear();
            throw MatrixWrongSizeError();
        }
    }

    int getSize() const {
        return Matrix<T>::getRowsNumber();
    }

    T getTrace() const {
        T trace = getZero<T>();
        for (int i = 0; i < Matrix<T>::getRowsNumber(); ++i) {
            trace += Matrix<T>::array[i][i];
        }
        return trace;
    }

    SquareMatrix getTransposed() const {
        return Matrix<T>::getTransposed();
    }

    SquareMatrix& transpose() {
        Matrix<T>::transpose();
        return *this;
    }

    T getDeterminant() const {
        T det = 1;
        SquareMatrix<T> temp = *this;
        for(int col = 0; col < this->getSize(); ++col){
            int row = col;
            for(; row < this->getSize() && temp(row, col) == 0; ++row){}
            if(row == this->getSize()){
                return 0;
            }
            if(row != col){
                for (int i = 0; i < this->getSize(); ++i){
                    std::swap(temp(row, i), temp(col, i));
                }
                det *= -1;
            }
            det *= temp(col, col);
            for(int j = col + 1; j < this->getSize(); ++j){
                T k = temp(j, col) / temp(col, col);
                for(int i = col; i < this->getSize(); ++i){
                    temp(j, i) -= temp(col, i) * k;
                }
            }
        }
        return det;
    }

    SquareMatrix<T> Minor(int p, int q) const {
        SquareMatrix<T> result(getSize() - 1);
        for (int row = 0; row < getSize(); ++row) {
            for (int col = 0; col < getSize(); ++col) {
                if (row != p && col != q) {
                    int i = row;
                    int j = col;
                    if (row > p) {
                        i = row - 1;
                    }
                    if (col > q) {
                        j = col - 1;
                    }
                    result(i, j) = (*this)(row, col);
                }
            }
        }
        return result;
    }

    SquareMatrix<T> getInverse() const {
        T det = getDeterminant();
        if (det == T(0)) {
            throw MatrixIsDegenerateError();
        }
        SquareMatrix<T> result(getSize());
        for(int row = 0; row < getSize(); ++row) {
            for(int col = 0; col < getSize(); ++col) {
                result(row, col) = Minor(row, col).getDeterminant() / det;
                if((row + col) % 2) {
                    result(row, col) *= -1;
                }
            }
        }
        result.transpose();
        return result;
    }

    SquareMatrix<T>& invert() {
        *this = getInverse();
        return *this;
    }

};

template <class T, class U>
SquareMatrix<T> operator*(const U& value, const SquareMatrix<T>& other) {
    SquareMatrix<T> result = other;
    result *= value;
    return result;
}

template <class T, class U>
SquareMatrix<T> operator*(const SquareMatrix<T>& other, const U& value) {
    SquareMatrix<T> result = other;
    result *= value;
    return result;
}

template <class T>
SquareMatrix<T> operator+(const SquareMatrix<T>& lhs, const SquareMatrix<T>& rhs) {
    SquareMatrix<T> res = lhs;
    res += rhs;
    return res;
}

template <class T>
SquareMatrix<T> operator-(const SquareMatrix<T>& lhs, const SquareMatrix<T>& rhs) {
    SquareMatrix<T> res = lhs;
    res -= rhs;
    return res;
}

template <class T>
SquareMatrix<T> operator*(const SquareMatrix<T>& lhs, const SquareMatrix<T>& rhs) {
    const Matrix<T>& lhs_base = lhs;
    const Matrix<T>& rhs_base = rhs;
    SquareMatrix<T> result = lhs_base * rhs_base;
    return result;
}

template <class T>
Matrix<T> operator*(const Matrix<T>& lhs, const SquareMatrix<T>& rhs) {
    const Matrix<T>& rhs_base = rhs;
    Matrix<T> result = lhs * rhs_base;
    return result;
}

template <class T>
Matrix<T> operator*(const SquareMatrix<T>& lhs, const Matrix<T>& rhs) {
    const Matrix<T>& lhs_base = lhs;
    Matrix<T> result = lhs_base * rhs;
    return result;
}


//================ class Rational ===============//

template <class T>
T GCD(const T& first_number, const T& second_number) {
    if (second_number == 0) {
        return first_number;
    } else {
        return GCD(second_number, first_number % second_number);
    }
}

class RationalDivisionByZero : public std::exception {
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
    int m, n, p;
    Rational r;
    cin >> m >> n >> p >> r;

    Matrix<Rational> A(m, n);
    SquareMatrix<Rational> S(p);
    cin >> A >> S;

    try {
        cout << (A * S) * A.getTransposed() << endl;
    } catch (const MatrixWrongSizeError&) {
        cout << "A and S have not appropriate sizes for multiplication." << endl;
    }

    cout << (r * (S = S) * S).getSize() << endl;

    SquareMatrix<Rational> P(S);

    cout << (P * (S + S - 3 * P)).getDeterminant() << endl;

    const SquareMatrix<Rational>& rS = S;

    cout << rS.getSize() << ' ' << rS.getDeterminant() << ' ' << rS.getTrace() << endl;
    cout << (S = S) * (S + rS) << endl;
    cout << (S *= S) << endl;

    try {
        cout << rS.getInverse() << endl;
        cout << P.invert().getTransposed().getDeterminant() << endl;
        cout << P << endl;
    } catch (const MatrixIsDegenerateError&) {
        cout << "Cannot inverse matrix." << endl;
    }

    return 0;
}
