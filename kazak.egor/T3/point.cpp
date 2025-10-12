#include "point.hpp"

namespace kazak {
  bool Point::operator==(const Point & other) const
  {
    return x == other.x && y == other.y;
  }

  std::ostream & operator<<(std::ostream & os, const Point & point)
  {
    return os << "(" << point.x << ";" << point.y << ")";
  }
}

