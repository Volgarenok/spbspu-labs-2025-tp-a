#ifndef POLYGON_COMMANDS_HPP
#define POLYGON_COMMANDS_HPP

#include <ios>
#include <vector>
#include <functional>
#include "geometry.hpp"

namespace aleksandrov
{
  using Polygons = std::vector< Polygon >;
  using FrameRect = std::pair< Point, Point >;
  using Vector = std::pair< int, int >;

  void getPolygons(std::istream&, std::vector< Polygon >&);

  template< class Pred >
  void printAreaSumIf(const Polygons&, Pred, std::ostream&);
  void printMeanArea(const Polygons&, std::ostream&);
  void area(const Polygons&, std::istream&, std::ostream&);

  template< class F >
  void printIthSmallest(const Polygons&, size_t, F, std::ostream&);
  void ithSmallest(const Polygons&, size_t, std::istream&, std::ostream&);

  template< class Pred >
  void printCountIf(const Polygons&, Pred, std::ostream&);
  void count(const Polygons&, std::istream&, std::ostream&);

  void inFrame(const Polygons&, std::istream&, std::ostream&);

  void rightShapes(const Polygons&, std::ostream&);
}

#endif

