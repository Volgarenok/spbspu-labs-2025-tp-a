#ifndef POLYGON_HPP
#define POLYGON_HPP

#include "input_struct.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

constexpr size_t MIN_VERTEX_COUNT = 3;

namespace filonova
{
  struct Point
  {
    int x, y;
  };
  std::istream &operator>>(std::istream &in, Point &point);
  bool operator<(const Point &lhs, const Point &rhs);
  bool operator==(const Point &lhs, const Point &rhs);

  struct Polygon
  {
    std::vector< Point > points;
  };
  std::istream &operator>>(std::istream &in, Polygon &polygon);

  bool isOdd(const Polygon &p);
  bool isEven(const Polygon &p);

  double getDistance(const Point &p1, const Point &p2);
  Polygon buildTriangle(size_t i, const std::vector< Point > &pts);

  struct TriangleGenerator
  {
    size_t &index;
    const std::vector< Point > &points;
    Polygon operator()();
  };

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
