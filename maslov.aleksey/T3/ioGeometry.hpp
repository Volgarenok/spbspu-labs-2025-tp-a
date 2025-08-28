#ifndef IO_GEOMETRY_HPP
#define IO_GEOMETRY_HPP

#include <iosfwd>
#include <vector>

namespace maslov
{
  struct Point
  {
    int x, y;
    bool operator==(const Point & rhs) const;
  };

  struct Polygon
  {
    std::vector< Point > points;
    bool operator==(const Polygon & rhs) const;
  };

  bool isOdd(const Polygon & polygon);
  bool isEven(const Polygon & polygon);
  bool isRectangle(const Polygon & polygon);
  bool hasNVertexes(const Polygon & polygon, size_t num);
  double getPolygonArea(const Polygon & polygon);
  bool compareVertexes(const Polygon & poly1, const Polygon & poly2);

  std::istream & operator>>(std::istream & in, Point & dest);
  std::istream & operator>>(std::istream & in, Polygon & dest);
  std::ostream & operator<<(std::ostream & out, const Point & dest);
  std::ostream & operator<<(std::ostream & out, const Polygon & dest);
}

#endif
