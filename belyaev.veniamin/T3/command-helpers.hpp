#ifndef COMMAND_HELPERS_HPP
#define COMMAND_HELPERS_HPP
#include <algorithm>
#include <functional>
#include <numeric>
#include <limits>
#include "shapes.hpp"

namespace belyaev
{
  size_t getVertices(const Polygon& src);
  bool isPolygonEven(const Polygon& src);
  bool isPolygonOdd(const Polygon& src);
  bool isPolygonOfSize(const Polygon& src, const size_t& userSize);
  bool isStringNumeric(const std::string& str);

  double areaTermCalculate(const std::vector< Point >& points, size_t index);
  double calcArea(const Polygon& src);
  void areaOut(double result, std::ostream& out);

  using comparatorFunction = std::function< bool(const Polygon&, const Polygon&) >;
  bool compareAreas(const Polygon& lhs, const Polygon& rhs);
  bool compareVertices(const Polygon& lhs, const Polygon& rhs);
  Polygon minElement(const std::vector< Polygon >& data, comparatorFunction comparator);
  Polygon maxElement(const std::vector< Polygon >& data, comparatorFunction comparator);

  bool rmEchoHelper(const Polygon& rmPolygon, const Polygon& lhs, const Polygon& rhs);
  struct Borders
  {
    int minX_, minY_, maxX_, maxY_;
    Borders(int minX, int minY, int maxX, int maxY):
      minX_(minX),
      minY_(minY),
      maxX_(maxX),
      maxY_(maxY)
    {}
  };
  bool compareX(const Point& lhs, const Point& rhs);
  bool compareY(const Point& lhs, const Point& rhs);
  Borders getPolygonBox(const Polygon& src);
  bool compareMinX(const Polygon& lhs, const Polygon& rhs);
  bool compareMaxX(const Polygon& lhs, const Polygon& rhs);
  bool compareMinY(const Polygon& lhs, const Polygon& rhs);
  bool compareMaxY(const Polygon& lhs, const Polygon& rhs);
  Borders getMaxPolygonBox(const std::vector< Polygon >& polys);
  bool isPointInBorders(const Point& p, const Borders& box);
}

#endif
