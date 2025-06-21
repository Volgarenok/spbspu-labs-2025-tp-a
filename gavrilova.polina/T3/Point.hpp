#ifndef POINT_HPP
#define POINT_HPP

#include <fstream>

namespace gavrilova {

  struct Point {
    int x;
    int y;

    bool operator==(const Point& other) const;
    bool operator<(const Point& other) const;
  };

  std::ostream& operator<<(std::ostream& os, const Point& point);
  std::istream& operator>>(std::istream& is, Point& point);

}

#endif
