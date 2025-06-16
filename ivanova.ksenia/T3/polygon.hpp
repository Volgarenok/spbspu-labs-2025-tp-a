#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <istream>
#include <ostream>
#include <vector>

namespace ivanova
{
  struct Point
  {
    int x, y;
    bool operator==(const Point& other) const;
  };

  struct Polygon
  {
    std::vector<Point> points;
    bool operator==(const Polygon& other) const;
  };

  std::istream& operator>>(std::istream& in, Point& point);
  std::ostream& operator<<(std::ostream& out, const Point& point);
  std::istream& operator>>(std::istream& in, Polygon& poly);
  std::ostream& operator<<(std::ostream& out, const Polygon& poly);
  double calculateArea(const Polygon& poly);
}

#endif
