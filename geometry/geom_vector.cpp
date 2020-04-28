#include <iostream>
#include <iomanip>
#include <cmath>

class Point {
    int x_;
    int y_;
public:
    Point(int x = 0, int y = 0) : x_(x), y_(y) {
    }

    Point(const Point& otherPoint) : x_(otherPoint.x_), y_(otherPoint.y_) {
    }

    int GetX() const {
        return x_;
    }

    int GetY() const {
        return y_;
    }

};

class Vector {
    int x_;
    int y_;
public:

    Vector(int x = 0, int y = 0) : x_(x), y_(y) {
    }

    Vector(const Point& begin, const Point& end) {

    }

    int GetX() const {
        return x_;
    }

    int GetY() const {
        return  y_;
    }

    double Length() const {
        return sqrt(pow(GetX(), 2) + pow(GetY(), 2));
    }

    Vector& operator*=(int multiplier) {
        x_ *= multiplier;
        return *this;
    }

    Vector& operator+=(const Vector& vec) {
        x_ += vec.x_;
        y_ += vec.y_;
        return *this;
    }

    Vector& operator-=(const Vector& vec) {
        x_ -= vec.x_;
        y_ -= vec.y_;
        return *this;
    }
};

Vector operator+(const Vector& lhs, const Vector& rhs) {
    Vector result = lhs;
    result += rhs;
    return result;
}

int Scalar(const Vector& lhs, const Vector& rhs) {
    return lhs.GetX() * rhs.GetX() + lhs.GetY() * rhs.GetY();
}

int VectorMultiplication(const Vector& lhs, const Vector& rhs) {
    return lhs.GetX() * rhs.GetY() - lhs.GetY() * rhs.GetX();
}

int main() {
    int x1, x2, y1, y2, x0, x3, y0, y3;
    std::cin >> x0 >> y0 >> x1 >> y1;
    std::cin >> x2 >> y2 >> x3 >> y3;

    Vector first(x1 - x0, y1 - y0);
    Vector second(x3 - x2, y3 - y2);

    std::cout << std::fixed << std::setprecision(9) << first.Length()
     << ' ' << second.Length() << '\n';

    Vector sum(first + second);
    std::cout << std::fixed << std::setprecision(9) <<
        (long double)sum.GetX() << ' ' << (long double)sum.GetY() << '\n';

    double scalar = Scalar(first, second);
    double vector = VectorMultiplication(first, second);
    std::cout << std::fixed << std::setprecision(9) <<
        scalar << ' ' << vector << '\n';

    double triangle_square = fabs(vector) / 2;
    std::cout << std::fixed << std::setprecision(9) << triangle_square << '\n';

    return 0;
}
