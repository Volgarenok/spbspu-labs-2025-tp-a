#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <vector>
#include <iostream>

namespace shak
{
  struct Point
  {
    int x, y;
  };
  std::istream &operator>>(std::istream &in, Point &dest);
  bool operator==(const Point &left, const Point &right);
  double getDistance(const Point &point1, const Point &point2);
  double getTriangleArea(const Point &point1, const Point &point2, const Point &point3);

 struct Polygon
  {
    std::vector< Point > points;
  };
  std::istream &operator>>(std::istream &in, Polygon &dest);
  bool isEven(const Polygon &polygon);
  bool isOdd(const Polygon &polygon);
  bool areEqualVertexes(size_t numOfVertexes, const Polygon &polygon);
  bool checkRectangle(const Polygon &polygon);
  double getArea(const Polygon &polygon);
  size_t getVertexes(const Polygon &polygon);
  size_t equalCounter(const Polygon &polygon, const std::vector< Point > &targetPoints, size_t& counter);

  struct DPoint
  {
    double x, y;
  };
  bool operator==(const DPoint &left, const DPoint& right);
  double getX(const shak::Point &point);
  double getY(const shak::Point &point);
  DPoint getMassCenter(const Polygon &polygon);
}
#endif
