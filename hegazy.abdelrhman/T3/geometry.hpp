#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <iostream>
#include <vector>

namespace bob
{
  struct Point
  {
    int x, y;
  };

  std::istream &operator>>(std::istream &in, Point &p);

  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream &operator>>(std::istream &in, Polygon &poly);

  bool isOdd(const Polygon &p);
  bool isEven(const Polygon &p);

  double getDistance(const Point &a, const Point &b);

  Polygon buildTriangle(size_t i, const std::vector< Point > &pts);

  struct TriangleGenerator
  {
    size_t &index;
    const std::vector< Point > &points;
    Polygon operator()();
  };

  std::vector< Polygon > polygonToTriangles(const Polygon &poly);

  double getTriangleArea(const Polygon &triangle);
  double getPolygonArea(const Polygon &poly);
  double getTotalArea(const std::vector< Polygon > &polygons);

  bool compareByArea(const Polygon &a, const Polygon &b);
  bool compareByVertexes(const Polygon &a, const Polygon &b);
  bool comparePointByX(const Point &a, const Point &b);
  bool comparePointByY(const Point &a, const Point &b);
}
#endif
