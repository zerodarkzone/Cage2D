#ifndef ALX_POINT_HPP
#define ALX_POINT_HPP


#include <alx/Vector2Base.hpp>


namespace alx {


/**
    A Point class.
    @param T type of coordinate.
 */
template <class T> class Point : public Vector2Base<Point<T>, T> {
public:
    /**
        constructor from x and y coordinates.
     */
    Point(T x = T(), T y = T()) : Vector2Base<Point<T>, T>(x, y) {
    }
};

template <> class Point<double> : public Vector2Base<Point<double>, double> {
public:
    /**
        constructor from x and y coordinates.
     */
    Point(double x = 0.0, double y = 0.0) : Vector2Base<Point<double>, double>(x, y) {
    }
    explicit Point(const Point<int> &p) : Vector2Base<Point<double>, double>(p.getX(), p.getY()) {
    }
};

/**
    Creates a point.
    @param x x coordinate.
    @param y y coordinate.
    @return a point with the given coordinates.
 */
template <class T> Point<T> makePoint(T x, T y) {
    return Point<T>(x, y);
}


} //namespace alx


#endif //ALX_POINT_HPP
