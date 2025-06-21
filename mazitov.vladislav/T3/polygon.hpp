#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <istream>
#include <vector>

namespace mazitov
{
  struct Point
  {
    int x, y;
  };
  bool operator<(const Point &lhs, const Point &rhs);
  bool operator==(const Point &lhs, const Point &rhs);
  std::istream &operator>>(std::istream &in, Point &dest);
  std::ostream& operator<<(std::ostream& out, const Point& src);

  struct Polygon
  {
    std::vector< Point > points;
  };
  bool operator==(const Polygon &lhs, const Polygon &rhs);
  std::istream &operator>>(std::istream &in, Polygon &dest);
  std::ostream& operator<<(std::ostream& out, const Polygon& src);

  struct PolygonAreaAccumulator
  {
    PolygonAreaAccumulator(const Polygon &p):
      poly(p),
      n(p.points.size())
    {}

    double operator()(double acc, const Point &p1) const
    {
      size_t i = &p1 - &poly.points[0];
      const Point &p2 = poly.points[(i + 1) % n];
      return acc + (p1.x * p2.y - p2.x * p1.y);
    }

    const Polygon &poly;
    size_t n;
  };

  double getPolygonArea(const Polygon &);
  bool isEvenVertexNum(const Polygon &);
  bool isOddVertexNum(const Polygon &);
  bool isEqualVertexNum(std::size_t, const Polygon &);

  double areaSumOperator(double, const Polygon &);
}

#endif
