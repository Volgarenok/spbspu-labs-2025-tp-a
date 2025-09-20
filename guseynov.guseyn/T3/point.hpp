#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>

namespace guseynov
{
  struct Point
  {
    int x;
    int y;
    bool operator==(const Point& other) const;
  };
  
  std::istream& operator>>(std::istream& in, Point& point);
}

#endif
