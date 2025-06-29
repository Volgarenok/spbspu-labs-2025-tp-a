#ifndef FUNCTORS_HPP
#define FUNCTORS_HPP

#include <cstddef>
#include "polygon.hpp"

namespace trukhanov
{
  struct isSize
  {
    size_t size;
    bool operator()(const Polygon& p) const;
  };

  struct CompareByArea
  {
    bool operator()(const Polygon& lhs, const Polygon& rhs) const;
  };

  struct LessArea
  {
    double area;
    bool operator()(const Polygon& p) const;
  };

  struct AccumulateAll
  {
    double operator()(double sum, const Polygon& p) const;
  };

  struct CompareByVertexes
  {
    bool operator()(const Polygon& lhs, const Polygon& rhs) const;
  };

  struct HasDuplicates
  {
    bool operator()(const Polygon& p) const;
  };

  struct PolygonHasMinSize
  {
    bool operator()(const Polygon& p) const;
  };

  struct TriangleArea
  {
    double operator()(const Point& a, const Point& b, const Point& c) const;
  };

  struct PolygonAreaSum
  {
    PolygonAreaSum(const Polygon& polygon);

    double operator()(double sum, size_t i) const;

  private:
    const Polygon& polygon_;
  };
}

#endif
