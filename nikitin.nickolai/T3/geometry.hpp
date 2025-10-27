#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <vector>
#include <iostream>
#include <cstddef>
#include <iterator>
#include <algorithm>

namespace nikitin
{
  struct Point
  {
    int x = 0;
    int y = 0;
  };
  bool operator<(const Point &, const Point &);
  bool operator==(const Point &, const Point &);
  std::istream& operator>>(std::istream &, Point &);
  std::ostream& operator<<(std::ostream &, const Point &);

  struct Polygon
  {
    std::vector< Point > points;
  };
  bool operator==(const Polygon &, const Polygon &);
  std::istream& operator>>(std::istream &, Polygon &);
  std::ostream& operator<<(std::ostream &, const Polygon &);
}

#endif
