#ifndef POINT_HPP
#define POINT_HPP

#include <iosfwd>

namespace gavrilova {
  struct Point {
    int x;
    int y;
  };

  bool operator==(const Point& a, const Point& b);
  bool operator<(const Point& a, const Point& b);

  std::istream& operator>>(std::istream& is, Point& point);
  std::ostream& operator<<(std::ostream& os, const Point& point);
}

#endif
