#ifndef FUNCTORS_HPP
#define FUNCTORS_HPP

#include "polygon.hpp"

constexpr size_t RECTANGLE_SIDES = 4;
static constexpr double EPS = 1e-9;

namespace filonova
{
  struct HasVertexCount
  {
    size_t count_;
    HasVertexCount(size_t count);
    bool operator()(const Polygon &p) const;
  };

  struct CompareByArea
  {
    bool operator()(const Polygon &a, const Polygon &b) const;
  };

  struct CompareByVertexes
  {
    bool operator()(const Polygon &a, const Polygon &b) const;
  };

  struct ComparePointByX
  {
    bool operator()(const Point &a, const Point &b) const;
  };

  struct ComparePointByY
  {
    bool operator()(const Point &a, const Point &b) const;
  };

  struct IntersectsWith
  {
    Polygon polygon_;
    IntersectsWith(const Polygon &polygon);
    bool operator()(const Polygon &other) const;
  };

  struct IsRectangle
  {
    static double dot(const Point &a, const Point &b, const Point &c);
    static double getDistanceSquared(const Point &a, const Point &b);
    bool operator()(const Polygon &polygon) const;
  };

  struct AngleCheckPoints
  {
    const Point &prev;
    const Point &current;
    const Point &next;
  };

  struct IsRightAngle
  {
    bool operator()(const AngleCheckPoints &angle) const;
  };
}

#endif
