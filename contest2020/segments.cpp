#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <math.h>

struct Point {
    long long x_;
    long long y_;

    Point(long long x = 0, long long y = 0) : x_(x), y_(y){
    }
};

struct Segment {
    Point begin_;
    Point end_;

    Segment(Point begin = (0, 0), Point end = (0, 0)) : begin_(begin), end_(end) {
    }
};

class Vector {
private:
    long long x_;
    long long y_;
public:
    Vector(long long x = 0, long long y = 0) : x_(x), y_(y) {
    }

    Vector(const Point& begin = (0, 0), const Point& end = (0, 0)) :
            x_(end.x_ - begin.x_),
            y_(end.y_ - begin.y_) {
    }

    ~Vector() = default;

    long long GetX() const {
        return x_;
    }

    long long GetY() const {
        return y_;
    }

};

long long VectorMultiplication(const Vector &lhs, const Vector &rhs) {
    return lhs.GetX() * rhs.GetY() - lhs.GetY() * rhs.GetX();
}


double Square(const Point& main_point, const Segment& segm) {
    double result = 0;
    Vector first(main_point, segm.begin_);
    Vector second(main_point, segm.end_);
    result = 0.5 * VectorMultiplication(first, second);
    return fabsl(result);

};


int main() {
    long long x1, y1;
    std::cin >> x1 >> y1;
    Point main_point(x1, y1);

    long long num;
    std::cin >> num;

    std::vector<Segment> segments;

    for (long long i = 0; i < num; ++i) {
        long long x1, y1, y2, x2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        Segment segm({x1, y1}, {x2, y2});
        segments.push_back(segm);
    }

    std::sort(segments.begin(), segments.end(), [main_point](const Segment& first,
                                                             const Segment& second)
    { return Square(main_point, first) < Square(main_point, second);}) ;

    for (long long i = 0; i < num; ++i) {
        std::cout << segments[i].begin_.x_ << ' ' << segments[i].begin_.y_ << ' ' <<
                  segments[i].end_.x_ << ' ' << segments[i].end_.y_ << '\n';
    }

    return 0;
}
