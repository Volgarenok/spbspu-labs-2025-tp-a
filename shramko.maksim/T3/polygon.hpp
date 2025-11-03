#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include <istream>

namespace shramko
{
  struct Point
  {
    int x;
    int y;
  };

  bool operator==(const Point& a, const Point& b);
  bool operator<(const Point& a, const Point& b);
  Point operator-(const Point& a, const Point& b);
  long long dot(const Point& a, const Point& b);

  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream& operator>>(std::istream& in, Point& point);
  std::istream& operator>>(std::istream& in, Polygon& polygon);
  double getPolygonArea(const Polygon& polygon);
}

#endif
