#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iostream>
#include <vector>

namespace nehvedovich
{
  struct Point
  {
    int x = 0, y = 0;
  };

  bool operator==(const Point& p1, const Point& p2);
  std::istream& operator>>(std::istream& in, Point& point);

  struct Polygon
  {
    std::vector< Point > points;
  };

  bool operator==(const Polygon& p1, const Polygon& p2);
  bool operator!=(const Polygon& p1, const Polygon& p2);
  std::istream& operator>>(std::istream& in, Polygon& polygon);

  bool isEven(const Polygon& p);
  bool isOdd(const Polygon& p);

  bool hasVertexCount(const Polygon& p, size_t count);

  bool compareByVertices(const nehvedovich::Polygon& p1, const nehvedovich::Polygon& p2);
  bool compareByArea(const nehvedovich::Polygon& p1, const nehvedovich::Polygon& p2);
  bool comparePointByX(const nehvedovich::Point& a, const nehvedovich::Point& b);
  bool comparePointByY(const nehvedovich::Point& a, const nehvedovich::Point& b);

  double getDistance(const Point& point1, const Point& point2);
  double getTriangleArea(const Point& point1, const Point& point2, const Point& point3);
  double getArea(const Polygon& polygon);
}

#endif
