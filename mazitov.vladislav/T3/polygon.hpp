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

  double triangleArea(const Point&, const Point&, const Point&);
  double getPolygonArea(const Polygon &);
  bool isEvenVertexNum(const Polygon &);
  bool isOddVertexNum(const Polygon &);
  bool isEqualVertexNum(std::size_t, const Polygon &);

  double areaSumOperator(double, const Polygon &);
}

#endif
