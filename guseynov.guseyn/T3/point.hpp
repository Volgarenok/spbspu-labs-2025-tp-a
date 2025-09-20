#ifndef POINT_HPP
#define POINT_HPP

namespace guseynov
{
  struct Point
  {
    int x;
    int y;
    bool operator==(const Point& other) const;
  };
}

#endif
