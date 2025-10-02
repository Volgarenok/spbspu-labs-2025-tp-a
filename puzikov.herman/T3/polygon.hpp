#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iostream>
#include <vector>

namespace puzikov
{
  struct Point
  {
    int x, y;
  };
  bool operator==(const Point &, const Point &);
  std::istream &operator>>(std::istream &, Point &);
  std::ostream &operator<<(std::ostream &, const Point &);

  struct Polygon
  {
    std::vector< Point > points;
  };
  bool operator==(const Polygon &, const Polygon &);
  bool operator!=(const Polygon &, const Polygon &);
  std::istream &operator>>(std::istream &, Polygon &);
  std::ostream &operator<<(std::ostream &, const Polygon &);

  void readPolygons(std::istream &, std::vector< Polygon > &);

  struct PolygonAreaAccumulator
  {
    PolygonAreaAccumulator(const Polygon &);
    double operator()(double acc, const Point &p1) const;

    const Polygon &poly;
    size_t n;
  };

  double calcPolygonArea(const Polygon &);
}

#endif
