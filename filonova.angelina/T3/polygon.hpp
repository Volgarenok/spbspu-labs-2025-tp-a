#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <input_struct.hpp>

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

constexpr size_t MIN_VERTEX_COUNT = 3;

namespace filonova
{
  struct Point
  {
    int x, y;
  };
  std::istream &operator>>(std::istream &in, Point &point);
  bool operator<(const Point &lhs, const Point &rhs);
  bool operator==(const Point &lhs, const Point &rhs);

  struct Polygon
  {
    std::vector< Point > points;
  };
  std::istream &operator>>(std::istream &in, Polygon &polygon);

  bool isOdd(const Polygon &p);
  bool isEven(const Polygon &p);

  struct Triangle
  {
    Point a, b, c;
  };

  struct TriangleGenerator
  {
    const std::vector< Point > &points_;
    size_t current_;

    explicit TriangleGenerator(const std::vector< Point > &points, size_t start = 1);
    bool hasNext() const;
    Triangle next();
  };

  struct TriangleAreaFunctor
  {
    double operator()(const Triangle &tri) const;
  };

  double triangleArea(const Point &a, const Point &b, const Point &c);
  double getArea(const Polygon &polygon);
  double computeTotalArea(const std::vector< Polygon > &polygons);
}

#endif
