#ifndef FUNCTORS_HPP
#define FUNCTORS_HPP

#include "polygon.hpp"

namespace shiryaeva
{
  struct IsEven
  {
    bool operator()(const Polygon& polygon) const;
  };

  struct IsOdd
  {
    bool operator()(const Polygon& polygon) const;
  };

  struct HasVertexCount
  {
    size_t count;
    bool operator()(const Polygon& polygon) const;
  };

  struct GetPolygonArea
  {
    double operator()(const Polygon& polygon) const;
  };

  struct GetPolygonVertexCount
  {
    size_t operator()(const Polygon& polygon) const;
  };
}

#endif
