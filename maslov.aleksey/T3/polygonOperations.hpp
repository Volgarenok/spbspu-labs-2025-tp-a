#ifndef POLYGON_OPERATIONS_HPP
#define POLYGON_OPERATIONS_HPP

#include "ioGeometry.hpp"

namespace maslov
{
  struct IsEven
  {
    bool operator()(const Polygon & polygon) const;
  };

  struct IsOdd
  {
    bool operator()(const Polygon & polygon) const;
  };

  struct AreaCalculator
  {
    double operator()(const Polygon & polygon) const;
  };

  struct SameVertexes
  {
    size_t num;
    bool operator()(const Polygon & polygon) const;
  };

  struct VertexesComparator
  {
    bool operator()(const Polygon & poly1, const Polygon & poly2) const;
  };

  struct IsRectangle
  {
    bool operator()(const Polygon & polygon) const;
  };

  struct SamePolygon
  {
    const Polygon & inPolygon;
    bool operator()(const Polygon & polygon) const;
  };

  struct EchoInserter
  {
    std::vector< Polygon > & result;
    const Polygon & inPolygon;
    int operator()(const Polygon & polygon);
  };
}

#endif
