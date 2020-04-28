#include <iostream>
#include <cstdlib>
#include <math.h>
#include <cstring>
#include <vector>

template <class T>
bool SameSign(const T& lhs, const T& rhs) {
    return (lhs >= 0 && rhs >= 0) || (lhs <= 0 && rhs <= 0);
}

template <class T>
bool DifferentSign(const T& lhs, const T& rhs) {
    return (lhs >= 0 && rhs <= 0) || (lhs <= 0 && rhs >= 0);
}

namespace geometry {

    class Vector;

    class AbstractShape;

    class Point;

    class Segment;

    class Ray;

    class Line;

    class Polygon;

    class Circle;

    class Vector {
    private:
        long long x_;
        long long y_;
    public:
        Vector(long long x = 0, long long y = 0);

        Vector(const Point &begin, const Point &end);

        Vector &operator=(const Vector &vector) = default;

        ~Vector() = default;

        long long GetX() const;

        long long GetY() const;

        double Length() const;

        Vector &operator*=(long long multiplier);

        Vector &operator+=(const Vector &vec);

        Vector &operator-=(const Vector &vec);
    };

    Vector operator*(const Vector &vec, long long multiplier);

    Vector operator*(long long multiplier, const Vector &vec);

    Vector operator+(const Vector &lhs, const Vector &rhs);

    Vector operator-(const Vector &lhs, const Vector &rhs);

    long long Scalar(const Vector &lhs, const Vector &rhs);

    long long VectorMultiplication(const Vector &lhs, const Vector &rhs);

    bool AreParallel(const Vector &lhs, const Vector &rhs);


    class AbstractShape {
    public:
        virtual ~AbstractShape() = default;

        virtual bool ContainsPoint(const Point &point) const = 0;

        virtual bool CrossSegment(const Segment &segment) const = 0;

        virtual AbstractShape *Clone() const = 0;

        virtual AbstractShape *Move(const Vector &dir) = 0;

        virtual void Print() const = 0;
    };

    class Point : public AbstractShape {
    private:
        long long x_;
        long long y_;
    public:
        Point(long long x = 0, long long y = 0);

        long long GetX() const;

        long long GetY() const;

        bool ContainsPoint(const Point &point) const override;

        bool CrossSegment(const Segment &segment) const override;

        Point *Clone() const override;

        Point *Move(const Vector &direction) override;

        void Print() const override;
    };

    class Segment : public AbstractShape {
    private:
        Point begin_;
        Point end_;
    public:
        Segment(const Point &begin, const Point &end);

        const Point &GetBegin() const;

        const Point &GetEnd() const;

        bool ContainsPoint(const Point &point) const override;

        bool CrossSegment(const Segment &other) const override;

        Segment *Clone() const override;

        Segment *Move(const Vector &direction) override;

        void Print() const override;
    };

    class Polygon : public AbstractShape {
        std::vector<Point> points_;
        long long num_points_;
    public:
        explicit Polygon(long long num_points = 0);

        long long NumPoints() const;

        Point& operator[](long long idx);
        const Point& operator[](long long idx) const;

        bool IsConvex() const;

        bool ContainsPoint(const Point& point) const override;
        bool CrossSegment(const Segment& segment) const override;
        Polygon* Clone() const override;
        Polygon* Move(const Vector& dir) override;
        void Print() const override;
    };
}

int main() {
    long long number;
    std::cin >> number;
    geometry::Polygon polygon(number);
    for (long long i = 0; i < number; ++i) {
        long long x, y;
        std::cin >> x >> y;
        polygon[i] = geometry::Point(x, y);
    }

    if (polygon.IsConvex()) {
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }

    return 0;
}

namespace geometry {

    Vector::Vector(long long x, long long y) : x_(x), y_(y) {
    }

    Vector::Vector(const Point &begin, const Point &end) :
            x_(end.GetX() - begin.GetX()),
            y_(end.GetY() - begin.GetY()) {
    }

    long long Vector::GetX() const {
        return x_;
    }

    long long Vector::GetY() const {
        return y_;
    }

    double Vector::Length() const {
        return sqrt(pow(GetX(), 2) + pow(GetY(), 2));
    }

    Vector &Vector::operator*=(long long multiplier) {
        x_ *= multiplier;
        return *this;
    }

    Vector &Vector::operator+=(const Vector &vec) {
        x_ += vec.x_;
        y_ += vec.y_;
        return *this;
    }

    Vector &Vector::operator-=(const Vector &vec) {
        x_ -= vec.x_;
        y_ -= vec.y_;
        return *this;
    }

    Vector operator+(const Vector &lhs, const Vector &rhs) {
        Vector result = lhs;
        result += rhs;
        return result;
    }

    Vector operator*(const Vector &vec, long long multiplier) {
        Vector result = vec;
        result *= multiplier;
        return result;
    }

    Vector operator*(long long multiplier, const Vector &vec) {
        Vector result = vec;
        result *= multiplier;
        return result;
    }

    long long Scalar(const Vector &lhs, const Vector &rhs) {
        return lhs.GetX() * rhs.GetX() + lhs.GetY() * rhs.GetY();
    }

    long long VectorMultiplication(const Vector &lhs, const Vector &rhs) {
        return lhs.GetX() * rhs.GetY() - lhs.GetY() * rhs.GetX();
    }

    bool AreParallel(const Vector &lhs, const Vector &rhs) {
        return VectorMultiplication(lhs, rhs) == 0;
    }

    Point::Point(long long x, long long y) : x_(x), y_(y) {
    }

    long long Point::GetX() const {
        return x_;
    }

    long long Point::GetY() const {
        return y_;
    }

    bool Point::ContainsPoint(const Point &point) const {
        return x_ == point.x_ && y_ == point.y_;
    }

    bool Point::CrossSegment(const Segment &segment) const {
        return segment.ContainsPoint(*this);
    }

    Point *Point::Clone() const {
        return new Point(*this);
    }

    Point *Point::Move(const Vector &direction) {
        x_ += direction.GetX();
        y_ += direction.GetY();
        return this;
    }

    void Point::Print() const {
        std::cout << x_ << ' ' << y_ << ' ';
    }

    Segment::Segment(const Point &begin, const Point &end) :
            begin_(begin),
            end_(end) {
    }

    const Point &Segment::GetBegin() const {
        return begin_;
    }

    const Point &Segment::GetEnd() const {
        return end_;
    }

    bool Segment::ContainsPoint(const Point &point) const {
        Vector begin_connector(begin_, point);
        Vector end_connector(point, end_);
        return AreParallel(begin_connector, end_connector) && Scalar(begin_connector, end_connector) >= 0;
    }

    bool Segment::CrossSegment(const Segment &other) const {
        // this(A, B)
        // other(C, D)
        Vector AB(begin_, end_), CD(other.begin_, other.end_),
                AC(begin_, other.begin_), AD(begin_, other.end_),
                BC(end_, other.begin_), BD(end_, other.end_);
        if (VectorMultiplication(CD, AB) != 0 && DifferentSign(VectorMultiplication(AB, AC),
                                                               VectorMultiplication(AB, AD)) &&
            DifferentSign(VectorMultiplication(CD, AC),
                          VectorMultiplication(CD, BC))) {
            return true;
        } else if (VectorMultiplication(CD, AB) == 0) {
            // parallel lines
            if (VectorMultiplication(AB, AC) == 0 && VectorMultiplication(CD, AC) == 0 &&
                (Scalar(AC, AD) <= 0 || Scalar(BC, AC) <= 0 ||
                 Scalar(BC, BD) <= 0 || Scalar(AD, BD) <= 0)) {
                // one line and correct order
                return true;
            }
            return false;
        }
        return false;
    }

    Segment *Segment::Clone() const {
        return new Segment(*this);
    }

    Segment *Segment::Move(const Vector &direction) {
        begin_.Move(direction);
        end_.Move(direction);
        return this;
    }

    void Segment::Print() const {
        begin_.Print();
        end_.Print();
    }

    Polygon::Polygon(long long num_points) : num_points_(num_points) {
        points_.reserve(num_points);
    }

    long long Polygon::NumPoints() const {
        return num_points_;
    }

    Point& Polygon::operator[](long long idx) {
        return points_[idx];
    }

    const Point& Polygon::operator[](long long idx) const {
        return points_[idx];
    }

    bool Polygon::IsConvex() const {
        long long last_rotation = 0;

        for (int i = 0; i < num_points_; ++i) {
            Vector first_edge(points_[i], points_[(i + 1) % num_points_]),
                    second_edge(points_[(i + 1) % num_points_], points_[(i + 2) % num_points_]);
            long long rotation = VectorMultiplication(first_edge, second_edge);

            if (rotation == 0 && Scalar(first_edge, second_edge) < 0) {
                return false;
            }
            if ((rotation < 0 && last_rotation > 0) || (rotation > 0 && last_rotation < 0)) {
                return false;
            }
            if (rotation != 0) {
                last_rotation = rotation;
            }
        }
        return true;
    }

    bool Polygon::ContainsPoint(const Point& point) const {
      if (points_[0].GetX() == point.GetX())  {
          return true;
      }
      return false;
    }

    bool Polygon::CrossSegment(const Segment& segment) const {
        for (int i = 0; i < num_points_; ++i) {
            Segment edge(points_[i], points_[(i + 1) % num_points_]);
            if (edge.CrossSegment(segment)) {
                return true;
            }
        }
        return false;
    }

    Polygon* Polygon::Clone() const {
        return new Polygon(*this);
    }

    Polygon* Polygon::Move(const Vector& dir) {
        for (long long i = 0; i < num_points_; ++i) {
            points_[i].Move(dir);
        }
        return this;
    }

    void Polygon::Print() const {
    }
}
