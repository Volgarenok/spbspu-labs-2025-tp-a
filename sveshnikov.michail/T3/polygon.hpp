#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <iostream>
#include <vector>

namespace sveshnikov
{
  struct Point
  {
    int x, y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  int get_x(const sveshnikov::Point &p);
  int get_y(const sveshnikov::Point &p);

  double getPolygonArea(const Polygon &poly);

  bool operator==(const sveshnikov::Point &pos1, const sveshnikov::Point &pos2);
  bool operator==(const sveshnikov::Polygon &poly1, const sveshnikov::Polygon &poly2);

  std::istream &operator>>(std::istream &in, Point &pos);
  std::istream &operator>>(std::istream &in, Polygon &shape);

  void loadPolygons(std::istream &in, std::vector< Polygon > &shapes);
}

#endif
