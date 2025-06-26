#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <vector>
#include <iterator>
#include <istream>
#include <algorithm>
#include <dataStruct.hpp>

namespace averenkov
{
  struct Point
  {
    int x;
    int y;
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
  };

  std::istream& operator>>(std::istream& in, Point& p);

  struct Polygon
  {
    std::vector< Point > points;
    bool operator==(const Polygon& other) const;
  };

  std::istream& operator>>(std::istream& in, Polygon& poly);

};

#endif
