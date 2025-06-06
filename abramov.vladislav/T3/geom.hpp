#ifndef GEOM_HPP
#define GEOM_HPP
#include <vector>
#include <iostream>

namespace abramov
{
  struct Point
  {
    int x;
    int y;
  };
  std::istream &operator>>(std::istream &in, Point &p);

  struct Polygon
  {
    std::vector< Point > points;
  };
  std::istream &operator>>(std::istream &in, Polygon &polygon);

  double getArea(const Polygon &polygon);
  double areaEven(double s, const Polygon &polygon);
  double areaOdd(double s, const Polygon &polygon);
  double areaMean(const std::vector< Polygon > &polygons);
  double areaVertexes(double s, const Polygon &polygon, size_t vert);
  bool maxArea(const Polygon &p1, const Polygon& p2);
  bool maxVertexes(const Polygon &p1, const Polygon &p2);
  size_t countEven(const std::vector< Polygon > &polygons);
  size_t countOdd(const std::vector< Polygon > &polygons);
  size_t countVertexes(const std::vector< Polygon > &polygons, size_t vert);
  bool isPointsEqual(const Point &p1, const Point &p2);
  bool isPolygonsEqual(const Polygon &p1, const Polygon &p2);
  bool isPolygonsEqualToExample(const Polygon &p1, const Polygon &p2, const Polygon &ex);
};


#endif
