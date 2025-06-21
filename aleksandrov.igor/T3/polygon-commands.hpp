#ifndef POLYGON_COMMANDS_HPP
#define POLYGON_COMMANDS_HPP

#include <ios>
#include <vector>
#include "geometry.hpp"

namespace aleksandrov
{
  using FrameRect = std::pair< Point, Point >;
  using Vector = std::pair< int, int >;

  namespace detail
  {
    double multPoints(const Point&, const Point&);
    double calcArea(const Polygon&);
  }
  void getPolygons(std::istream&, std::vector< Polygon >&);

  double areaEven(const std::vector< Polygon >&);
  double areaOdd(const std::vector< Polygon >&);
  double areaMean(const std::vector< Polygon >&);
  double areaN(const std::vector< Polygon >&);
  void area(const std::vector< Polygon >&, std::istream&, std::ostream&);

  double maxArea(const std::vector< Polygon >&);
  size_t maxVertexes(const std::vector< Polygon >&);
  void max(const std::vector< Polygon >&, std::istream&, std::ostream&);

  double minArea(const std::vector< Polygon >&);
  size_t minVertexes(const std::vector< Polygon >&);
  void min(const std::vector< Polygon >&, std::istream&, std::ostream&);

  size_t countEven(const std::vector< Polygon >&);
  size_t countOdd(const std::vector< Polygon >&);
  size_t countN(const std::vector< Polygon >&, size_t);
  void count(const std::vector< Polygon >&, std::istream&, std::ostream&);

  void inframe(const std::vector< Polygon >&, std::istream&, std::ostream&);
  FrameRect findFrameRect(const Polygon&);
  FrameRect findGlobalFrameRect(const std::vector< Polygon >&);

  void rightshapes(const std::vector< Polygon >&, std::ostream&);
}

#endif

