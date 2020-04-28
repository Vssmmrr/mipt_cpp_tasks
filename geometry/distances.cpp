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

    double Distance(const Point &lhs, const Point &rhs);
    double Distance(const Point &point, const Segment &segment);

    class Line : public AbstractShape {
    private:
        Point origin_;
        Vector direction_;
    public:
        Line(const Point &origin = Point(0, 0), const Vector &direction = Vector(1, 0));
        Line(const Point &first, const Point &second);

        const Point& GetOrigin() const;
        const Vector& GetDirection() const;

        bool ContainsPoint(const Point &point) const override;
        bool CrossSegment(const Segment &segment) const override;
        Line *Clone() const override;
        Line *Move(const Vector &direction) override;
        void Print() const override;
    };

    bool AreParallel(const Line &lhs, const Line &rhs);
    double Distance(const Point& point, const Line& line);

    class Ray : public AbstractShape {
        Point origin_;
        Vector direction_;
    public:
        Ray(const Point &origin = Point(0, 0), const Vector &direction = Vector(1, 0));
        Ray(const Point &first, const Point &second);

        const Point &GetOrigin() const;
        const Vector &GetDirection() const;

        bool ContainsPoint(const Point &point) const override;
        bool CrossSegment(const Segment &segment) const override;
        Ray *Clone() const override;
        Ray *Move(const Vector &direction) override;
        void Print() const override;
    };

    double Distance(const Point& point, const Ray& ray);
}

int main() {
    long long x1 , y1 , x2, y2, x3, y3;
    std::cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;

    geometry::Point point(x1, y1);
    geometry::Line line(geometry::Point(x2, y2), geometry::Point(x3, y3));
    geometry::Ray ray(geometry::Point(x2, y2), geometry::Point(x3, y3));
    geometry::Segment segment(geometry::Point(x2, y2), geometry::Point(x3, y3));

    std::cout.precision(9);
    std::cout << std::fixed;

    std::cout << geometry::Distance(point, line) << '\n';
    std::cout << geometry::Distance(point, ray) << '\n';
    std::cout << geometry::Distance(point, segment) << '\n';

    return 0;
}


namespace geometry {

    Vector::Vector(long long x, long long y) : x_(x), y_(y) {
    }

    Vector::Vector(const Point& begin, const Point& end) :
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

    Vector& Vector::operator*=(long long multiplier) {
        x_ *= multiplier;
        return *this;
    }

    Vector& Vector::operator+=(const Vector& vec) {
        x_ += vec.x_;
        y_ += vec.y_;
        return *this;
    }

    Vector& Vector::operator-=(const Vector& vec) {
        x_ -= vec.x_;
        y_ -= vec.y_;
        return *this;
    }

    Vector operator+(const Vector& lhs, const Vector& rhs) {
        Vector result = lhs;
        result += rhs;
        return result;
    }

    Vector operator*(const Vector& vec, long long multiplier) {
        Vector result = vec;
        result *= multiplier;
        return result;
    }

    Vector operator*(long long multiplier, const Vector& vec) {
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

    bool AreParallel(const Vector& lhs, const Vector& rhs) {
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

    bool Point::ContainsPoint(const Point& point) const {
        return x_ == point.x_ && y_ == point.y_;
    }
    bool Point::CrossSegment(const Segment& segment) const {
        return segment.ContainsPoint(*this);
    }

    Point* Point::Clone() const {
        return new Point(*this);
    }

    Point* Point::Move(const Vector& direction) {
        x_ += direction.GetX();
        y_ += direction.GetY();
        return this;
    }

    void Point::Print() const {
        std::cout << x_ << ' ' << y_ << ' ';
    }

    Segment::Segment(const Point& begin, const Point& end) :
            begin_(begin),
            end_(end) {
    }

    const Point& Segment::GetBegin() const {
        return begin_;
    }

    const Point& Segment::GetEnd() const {
        return end_;
    }

    bool Segment::ContainsPoint(const Point& point) const {
        Vector begin_connector(begin_, point);
        Vector end_connector(point, end_);
        return AreParallel(begin_connector, end_connector) && Scalar(begin_connector, end_connector) >= 0;
    }

    bool Segment::CrossSegment(const Segment& other) const {
        // this(A, B)
        // other(C, D)
        Vector AB(begin_, end_), CD(other.begin_, other.end_),
                AC(begin_, other.begin_), AD(begin_, other.end_),
                BC(end_, other.begin_), BD(end_, other.end_);
        if (VectorMultiplication(CD, AB) != 0 && DifferentSign(VectorMultiplication(AB, AC),
                                                               VectorMultiplication(AB, AD)) && DifferentSign(VectorMultiplication(CD, AC),
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

    Segment* Segment::Clone() const {
        return new Segment(*this);
    }

    Segment* Segment::Move(const Vector& direction) {
        begin_.Move(direction);
        end_.Move(direction);
        return this;
    }

    void Segment::Print() const {
        begin_.Print();
        end_.Print();
    }

    Line::Line(const Point& origin, const Vector& direction) :
            origin_(origin),
            direction_(direction) {
    }

    Line::Line(const Point& first, const Point& second) :
            origin_(first), direction_(first, second) {
    }

    const Point& Line::GetOrigin() const {
        return origin_;
    }

    const Vector& Line::GetDirection() const {
        return direction_;
    }

    bool Line::ContainsPoint(const Point& point) const {
        return AreParallel(Vector(origin_, point), direction_);
    }

    bool Line::CrossSegment(const Segment& segment) const {
        if (ContainsPoint(segment.GetBegin()) || ContainsPoint(segment.GetEnd())) {
            return true;
        }
        Vector first_connector(origin_, segment.GetBegin()),
                second_connector(origin_, segment.GetEnd());

        return !SameSign(VectorMultiplication(first_connector, direction_),
                         VectorMultiplication(second_connector, direction_));
    }

    Line* Line::Clone() const {
        return new Line(*this);
    }

    Line* Line::Move(const Vector& direction) {
        origin_.Move(direction);
        return this;
    }

    void Line::Print() const {
        origin_.Print();
        Point second_point = origin_;
        second_point.Move(direction_);
        second_point.Print();
    }

    bool AreParallel(const Line& lhs, const Line& rhs) {
        return AreParallel(lhs.GetDirection(), rhs.GetDirection());
    }

    Ray::Ray(const Point& origin, const Vector& direction) : origin_(origin), direction_(direction) {
    }

    Ray::Ray(const Point& first, const Point& second) : origin_(first), direction_(Vector(first, second)) {
    }

    const Point& Ray::GetOrigin() const {
        return origin_;
    }

    const Vector& Ray::GetDirection() const {
        return direction_;
    }

    bool Ray::ContainsPoint(const Point& point) const {
        Vector connector(origin_, point);
        return AreParallel(connector, direction_) && Scalar(connector, direction_) >= 0;
    }

    bool Ray::CrossSegment(const Segment& segment) const {
        if (AreParallel(direction_, Vector(segment.GetBegin(), segment.GetEnd()))) {
            return ContainsPoint(segment.GetBegin()) || ContainsPoint(segment.GetEnd());
        }

        Vector a(segment.GetBegin(), segment.GetEnd()), b(segment.GetBegin(), origin_);

        long long det = VectorMultiplication(direction_, a),
                first_det = VectorMultiplication(direction_, b),
                second_det = VectorMultiplication(a, b);

        if (det < 0) {
            det = -det;
            first_det = -first_det;
            second_det = -second_det;
        }

        return first_det >= 0 && first_det <= det && second_det >= 0;
    }

    Ray* Ray::Clone() const {
        return new Ray(*this);
    }

    Ray* Ray::Move(const Vector& direction) {
        origin_.Move(direction);
        return this;
    }

    void Ray::Print() const {
        origin_.Print();
        Point end = origin_;
        end.Move(direction_);
        end.Print();
    }

    double Distance(const Point& lhs, const Point& rhs) {
        return Vector(lhs, rhs).Length();
    }

    double Distance(const Point& point, const Line& line) {
        Vector connector(point, line.GetOrigin());
        return fabs(VectorMultiplication(connector, line.GetDirection())) / line.GetDirection().Length();
    }

    double Distance(const Point& point, const Ray& ray) {
        Vector connector(ray.GetOrigin(), point);
        const Vector& direction = ray.GetDirection();
        const Point& origin = ray.GetOrigin();

        if (Scalar(connector, direction) < 0) {
            return Distance(origin, point);
        }

        return fabs(VectorMultiplication(connector, direction)) / direction.Length();
    }

    double Distance(const Point& point, const Segment& segment) {
        Point s_begin = segment.GetBegin();
        Point s_end = segment.GetEnd();
        Vector begin_connector(s_begin, point);
        Vector end_connector(s_end, point);
        Vector segment_dir(s_begin, s_end);

        if (Scalar(begin_connector, segment_dir) < 0) {
            return Distance(point, s_begin);
        }
        if (Scalar(end_connector, segment_dir) > 0) {
            return Distance(point, s_end);
        }

        return fabs(VectorMultiplication(begin_connector, segment_dir)) / segment_dir.Length();
    }
}
