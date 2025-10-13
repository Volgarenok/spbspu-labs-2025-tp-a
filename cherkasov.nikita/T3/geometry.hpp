#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <vector>
#include <iostream>

namespace cherkasov
{
  struct Point
  {
    int x;
    int y;
    bool operator==(const Point& other) const;
    bool operator<(const Point& other) const;
  };
  std::istream& operator>>(std::istream& in, Point& p);

  struct Polygon
  {
    std::vector< Point > points;
    bool operator==(const Polygon& other) const;
  };
  std::istream& operator>>(std::istream& in, Polygon& polygon);

  struct VertexesCmp
  {
    size_t k;

    explicit VertexesCmp(size_t vert):
      k(vert)
    {}
    bool operator()(const Polygon &polygon) const;
  };

  struct IntersectCmp
  {
    const Polygon& polygon;
    explicit IntersectCmp(const Polygon& p): polygon(p) {}
    bool operator()(const Polygon& other) const;
  };

  struct RightAngleCmp
  {
    bool operator()(const Polygon& poly) const;
  };

  double getArea(const Polygon& polygon);
  double areaMean(const std::vector< Polygon >& polygons);
  bool maxArea(const Polygon& p1, const Polygon& p2);
  bool maxVertexes(const Polygon& p1, const Polygon& p2);
  size_t countEven(const std::vector< Polygon >& polygons);
  size_t countOdd(const std::vector< Polygon >& polygons);
  size_t countVertexes(const std::vector< Polygon >& polygons, size_t vert);
  bool isPointsEqual(const Point& p1, const Point& p2);
  bool isPolygonsEqual(const Polygon& p1, const Polygon& p2);
  bool isEven(const Polygon& p);
  bool isOdd(const Polygon& p);
  int crossProduct(const Point& p1, const Point& p2);
}

#endif
