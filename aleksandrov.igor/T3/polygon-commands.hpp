#ifndef POLYGON_COMMANDS_HPP
#define POLYGON_COMMANDS_HPP

#include <ios>
#include <vector>
#include "geometry.hpp"

namespace aleksandrov
{
  using FrameRect = std::pair< Point, Point >;
  using Vector = std::pair< int, int >;

  void getPolygons(std::istream&, std::vector< Polygon >&);

  double areaEven(const std::vector< Polygon >&);
  double areaOdd(const std::vector< Polygon >&);
  double areaMean(const std::vector< Polygon >&);
  double areaN(const std::vector< Polygon >&, size_t);
  void area(const std::vector< Polygon >&, std::istream&, std::ostream&);

  void maxArea(const std::vector< Polygon >&, std::ostream&);
  void maxVertices(const std::vector< Polygon >&, std::ostream&);
  void max(const std::vector< Polygon >&, std::istream&, std::ostream&);

  void minArea(const std::vector< Polygon >&, std::ostream&);
  void minVertices(const std::vector< Polygon >&, std::ostream&);
  void min(const std::vector< Polygon >&, std::istream&, std::ostream&);

  size_t countEven(const std::vector< Polygon >&);
  size_t countOdd(const std::vector< Polygon >&);
  size_t countN(const std::vector< Polygon >&, size_t);
  void count(const std::vector< Polygon >&, std::istream&, std::ostream&);

  FrameRect findFrameRect(const Polygon&);
  FrameRect findGlobalFrameRect(const std::vector< Polygon >&);
  void inFrame(const std::vector< Polygon >&, std::istream&, std::ostream&);

  void rightShapes(const std::vector< Polygon >&, std::ostream&);
}

#endif

