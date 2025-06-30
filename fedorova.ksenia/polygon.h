#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <istream>

namespace fedorova
{
  struct Point
  {
    int x;
    int y;
  };

  struct Polygon
  {
    std::vector<Point> points;
  };

  std::istream& operator>>(std::istream&, Point&);
  std::istream& operator>>(std::istream&, Polygon&);
  bool operator==(const Point&, const Point&);
  bool operator==(const Polygon&, const Polygon&);
  double getArea(const Polygon&);
  bool hasRightAngle(const Polygon&);
  bool isPermutation(const Polygon&, const Polygon&);
}

#endif

