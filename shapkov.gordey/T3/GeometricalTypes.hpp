#ifndef GEOMETRICAL_TYPES
#define GEOMETRICAL_TYPES
#include <iostream>
#include <vector>

namespace shapkov
{
  struct Point
  {
    double x, y;
  };
  struct Polygon
  {
    std::vector< Point > points;
  };
  std::istream& operator>>(std::istream& in, Point& point);
  std::ostream& operator<<(std::ostream& out, const Point& point);
  std::istream& operator>>(std::istream& in, Polygon& polygon);
  std::ostream& operator<<(std::ostream& out, const Polygon& polygon);
  double getDistance(const Point& p1, const Point& p2);
  double getAreaOfTriangle(const Polygon& p);
  struct TriangleGenerator
  {
    size_t& index;
    const std::vector< Point >& points;
    shapkov::Polygon operator()();
  };
  Polygon makeTriangle(size_t i, const std::vector< Point >& pts);
  std::vector< Polygon > polygonToTriangles(const Polygon& p);
  double getArea(const Polygon& p);
  bool isEven(const Polygon& p);
  bool isOdd(const Polygon& p);
  bool compareByVertexes(const Polygon& p1, const Polygon& p2);
  bool isRectangle(const Polygon& p);
}

#endif
