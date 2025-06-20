#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <vector>
namespace asafov
{
  struct Point
  {
    int x, y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  double computeArea(const Polygon& poly);
  bool operator==(const Point& a, const Point& b);
  bool arePolygonsSame(const Polygon& a, const Polygon& b);
  bool doPolygonsIntersect(const Polygon& a, const Polygon& b);
  bool edgesIntersect(const Point& a1, const Point& a2, const Point& b1, const Point& b2);
}
#endif
