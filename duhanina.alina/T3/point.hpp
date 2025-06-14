#ifndef POINT_HPP
#define POINT_HPP

#include <istream>

namespace duhanina
{
  struct Point
  {
    int x;
    int y;
  };

  bool operator==(const Point& lhs, const Point& rhs);
  bool operator!=(const Point& lhs, const Point& rhs);
  std::istream& operator>>(std::istream& in, Point& point);
}

#endif
