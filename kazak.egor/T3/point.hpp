#ifndef POINT_HPP
#define POINT_HPP
#include <ostream>

namespace kazak {
  struct Point {
    int x, y;
    bool operator==(const Point & other) const;
  };

  std::ostream & operator<<(std::ostream & os, const Point & point);
}

#endif
