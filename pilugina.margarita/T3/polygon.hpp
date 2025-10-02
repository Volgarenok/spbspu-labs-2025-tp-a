#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <istream>
#include <vector>
#include <delimiterIO.hpp>

namespace pilugina
{
  struct Point
  {
    int x, y;
  };
  bool operator<(const Point &lhs, const Point &rhs);
  bool operator==(const Point &lhs, const Point &rhs);
  std::istream &operator>>(std::istream &in, Point &dest);

  struct Polygon
  {
    std::vector< Point > points;
  };
  bool operator==(const Polygon &lhs, const Polygon &rhs);
  std::istream &operator>>(std::istream &in, Polygon &dest);
  bool operator<(const Polygon &lhs, const Polygon &rhs);
}

#endif
