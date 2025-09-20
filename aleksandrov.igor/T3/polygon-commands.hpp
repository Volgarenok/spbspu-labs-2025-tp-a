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

  void getPolygons(std::istream&, Polygons&);
  void processCommands(std::istream&, std::ostream&, Polygons&);

  using namespace std::placeholders;

  void execArea(const Polygons&, std::istream&, std::ostream&);
  template< class F >
  void execAreaIf(const Polygons&, F, std::ostream&);
  void execAreaMean(const Polygons&, std::ostream&);

  void execIthSmallest(const Polygons&, size_t, std::istream&, std::ostream&);
  template< class F >
  void execIthSmallestImpl(const Polygons&, F, size_t, std::ostream&);

  void execCount(const Polygons&, std::istream&, std::ostream&);
  template< class Pred >
  void execCountIf(const Polygons&, Pred, std::ostream&);

  void execInFrame(const Polygons&, std::istream&, std::ostream&);

  void execRightShapes(const Polygons&, std::ostream&);
}

#endif

