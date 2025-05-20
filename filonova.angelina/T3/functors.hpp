#ifndef FUNCTORS_HPP
#define FUNCTORS_HPP

#include "polygon.hpp"

#include <algorithm>
#include <cmath>

namespace filonova
{
  struct IsEven
  {
    bool operator()(const Polygon &polygon) const;
  };

  struct IsOdd
  {
    bool operator()(const Polygon &polygon) const;
  };

  struct GetPolygonArea
  {
    double operator()(const Polygon &polygon) const;
  };

  struct GetPolygonVertexCount
  {
    size_t operator()(const Polygon &polygon) const;
  };

  struct HasVertexCount
  {
    size_t count;
    bool operator()(const Polygon &polygon) const;
  };

  struct IntersectsWith
  {
    Polygon polygon;
    bool operator()(const Polygon &other) const;
  };

  struct IsRectangle
  {
    bool operator()(const Polygon &polygon) const;

  private:
    static double dot(const Point &a, const Point &b, const Point &c);
    static double getDistanceSquared(const Point &a, const Point &b);
  };

}

#endif
