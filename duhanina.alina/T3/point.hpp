#ifndef POINT_HPP
#define POINT_HPP

#include <istream>

namespace duhanina
{
  struct Point
  {
    int x = 0;
    int y = 0;
  };

  bool operator==(const Point& lhs, const Point& rhs);
  bool operator!=(const Point& lhs, const Point& rhs);
  std::istream& operator>>(std::istream& in, Point& point);
}

#endif
