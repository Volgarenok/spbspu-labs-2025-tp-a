#ifndef SHAPES
#define SHAPES

#include <iostream>
#include <vector>

namespace puzikov
{
  struct Point
  {
    int x, y;
  };
  bool operator==(const Point &p1, const Point &p2);
  std::istream &operator>>(std::istream &in, Point &dest);
  std::ostream &operator<<(std::ostream &out, const Point &src);

  struct Polygon
  {
    std::vector< Point > points;
  };
  bool operator==(const Polygon &p1, const Polygon &p2);
  bool operator!=(const Polygon &p1, const Polygon &p2);
  std::istream &operator>>(std::istream &in, Polygon &dest);
  std::ostream &operator<<(std::ostream &out, const Polygon &src);

  void readPolygons(std::istream &, std::vector< Polygon > &);
  void writePolygons(std::ostream &, std::vector< Polygon > &);

  struct PolygonAreaAccumulator;
  double calcPolygonArea(const puzikov::Polygon &poly);
}

#endif
