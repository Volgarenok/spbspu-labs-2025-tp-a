#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <cstddef>
#include <iostream>
#include <vector>

namespace ivanova
{
  struct Point
  {
    int x = 0;
    int y = 0;

    bool operator==(const Point&) const;

    Point operator-(const Point&) const;

    Point operator-() const;
  };

  struct Polygon
  {
    std::vector< Point > points;

    std::size_t size() const;

    double area() const;

    bool operator==(const Polygon&) const;

  private:
    static double crossMultiply(const Point&, const Point&);
  };

  std::istream& operator>>(std::istream&, Point&);
  std::ostream& operator<<(std::ostream&, const Point&);
  std::istream& operator>>(std::istream&, Polygon&);
  std::ostream& operator<<(std::ostream&, const Polygon&);
}

#endif
