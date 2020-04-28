#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <iomanip>

class Point {
private:
    double x_;
    double y_;
public:

    Point(double x = 0, double y = 0) : x_(x), y_(y) {
    }

    Point(const Point& a) : x_(a.x_), y_(a.y_) {
    }

    double GetX() const {
        return x_;
    }

    double GetY() const {
        return y_;
    }
};

class Line {
private:
    double A_;
    double B_;
    double C_;
public:

    Line(double A = 0, double B = 0, double C = 0) : A_(A), B_(B), C_(C) {
    }

    ~Line(){
    }

    Point DirectionVector() const {
        return Point(-B_, A_);
    }

    bool IsNotCrossed(const Line& line) const {
        return (A_ * line.B_ - line.A_ * B_ == 0);
    }

    Point  IntersectionPoint(const Line& line) {
        return Point(-(C_ * line.B_ - line.C_ * B_) / (line.B_ * A_ - B_ * line.A_),
                     - (A_ * line.C_ - line.A_ * C_) / (line.B_ * A_ - B_ * line.A_));
    }

    friend  double Distance(const Line& lhs, const Line& rhs) {
        return fabs(rhs.C_ * lhs.A_ - lhs.C_ * rhs.A_) / sqrt(pow(lhs.A_ * rhs.A_, 2) + pow(lhs.B_ * rhs.A_, 2));
    }

};

int main() {
    long double A_1, B_1, C_1, A_2, B_2, C_2;
    std::cin >> A_1 >> B_1 >> C_1 >> A_2 >> B_2 >> C_2;

    Line first(A_1, B_1, C_1);
    Line second(A_2, B_2, C_2);

    Point p1 = first.DirectionVector();
    Point p2 = second.DirectionVector();

    std::cout.precision(9);
    std::cout << std::fixed;

    std::cout  << p1.GetX() << ' ' << p1.GetY() << '\n';
    std::cout << p2.GetX() << ' ' << p2.GetY() << '\n';
    if (first.IsNotCrossed(second)) {
        std::cout << Distance(first, second) << '\n';
    }
    else {
        Point p3 = first.IntersectionPoint(second);
        std::cout << p3.GetX() << ' ' << p3.GetY() << '\n';
    }

    return 0;
}

