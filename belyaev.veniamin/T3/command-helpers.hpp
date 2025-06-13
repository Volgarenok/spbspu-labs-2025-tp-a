#ifndef COMMAND_HELPERS_HPP
#define COMMAND_HELPERS_HPP
#include "shapes.hpp"
#include <algorithm>
#include <cmath>
#include <functional>
#include <numeric>
#include <limits>

namespace belyaev
{
  double accumulateTerm(double sum, int i, const std::vector<Point>& pnts, int n);
  double calcArea(const Polygon& src);
  bool compareAreas(const Polygon& lhs, const Polygon& rhs);
  bool compareVertices(const Polygon& lhs, const Polygon& rhs);
  bool isPolygonEven(const Polygon& src);
  bool isPolygonOdd(const Polygon& src);
  bool isPolygonOfSize(const Polygon& src, const size_t& userSize);
  bool rmEchoHelper(const Polygon& rmPolygon, const Polygon& lhs, const Polygon& rhs);
  bool isStringNumeric(const std::string& str);
  size_t getVertices(const Polygon& src);

  struct Borders
  {
    int minX = std::numeric_limits<int>::max();
    int minY = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    int maxY = std::numeric_limits<int>::min();
  };
  Borders getPointBorders(Borders box, const Point& pnt);
  Borders getPolygonBorders(Borders box, const Polygon& src);
  bool isPointInBorders(const Point& p, const Borders& box);
}

#endif
