#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <vector>
#include <istream>
#include <iterator>
#include <algorithm>
#include <delimiter.hpp>

namespace lanovenko
{
  struct Point
  {
    int x{};
    int y{};

    bool operator==(const Point& rhs) const noexcept;
    Point operator-(const Point& rhs) const noexcept;
  };

  struct Polygon
  {
    std::vector< Point > points;

    bool operator==(const Polygon& rhs) const noexcept;
  };

  std::istream& operator>>(std::istream& in, Point& dest);
  std::istream& operator>>(std::istream& in, Polygon& dest);
}

#endif
