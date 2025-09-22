#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <input_struct.hpp>
#include <vector>
#include <istream>
#include <cmath>

namespace shiryaeva
{
  constexpr size_t MIN_VERTEX_COUNT = 3;

  struct Point
  {
    int x;
    int y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  bool isOdd(const Polygon &p);
  bool isEven(const Polygon &p);

  std::istream &operator>>(std::istream &in, Point &point);
  std::istream &operator>>(std::istream &in, Polygon &polygon);

  bool operator==(const Point &lhs, const Point &rhs);
  bool operator<(const Point &lhs, const Point &rhs);

  struct TriangleGenerator
  {
    size_t &index;
    const std::vector< Point > &points;
    Polygon operator()();
  };

  double getDistance(const Point &p1, const Point &p2);
  Polygon buildTriangle(size_t i, const std::vector< Point > &pts);
  std::vector< Polygon > polygonToTriangles(const Polygon &p);
  double getTriangleArea(const Polygon &triangle);
  double getPolygonArea(const Polygon &polygon);
  double getTotalArea(const std::vector< Polygon > &polygons);
  bool compareByArea(const Polygon &a, const Polygon &b);
  bool compareByVertexes(const Polygon &a, const Polygon &b);
  bool comparePointByX(const Point &a, const Point &b);
  bool comparePointByY(const Point &a, const Point &b);
}

#endif
