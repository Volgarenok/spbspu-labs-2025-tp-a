#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include "geom.h"

namespace sherkunov
{
  double subArea(const Point&, const Point&);
  double areaPolygon(const Polygon&);
  bool isEven(const Polygon&);
  bool isOdd(const Polygon&);
  bool isNum(const Polygon&, size_t);
  double evenAreaAccumulator(double, const Polygon&);
  double oddAreaAccumulator(double, const Polygon&);
  double meanAreaAccumulator(double, const Polygon&);
  double numAreaAccumulator(double, const Polygon&, size_t);

  bool areaComparator(const Polygon&, const Polygon&);
  bool vertexesComparator(const Polygon&, const Polygon&);
  void area(const std::vector< Polygon >&, std::istream&, std::ostream&);
  double areaEven(const std::vector< Polygon >&);
  double areaOdd(const std::vector< Polygon >&);
  double areaMean(const std::vector< Polygon >&);
  double areaNum(const std::vector< Polygon >&, size_t);

  void max(const std::vector< Polygon >&, std::istream&, std::ostream&);
  double maxArea(const std::vector< Polygon >&);
  size_t maxVertexes(const std::vector< Polygon >&);

  void min(const std::vector< Polygon >&, std::istream&, std::ostream&);
  double minArea(const std::vector< Polygon >&);
  size_t minVertexes(const std::vector< Polygon >&);

  void count(const std::vector< Polygon >&, std::istream&, std::ostream&);
  size_t countEven(const std::vector< Polygon >&);
  size_t countOdd(const std::vector< Polygon >&);
  size_t countNum(const std::vector< Polygon >&, size_t);
  void inframe(const std::vector< Polygon >&, std::istream&, std::ostream&);
  void rightshapes(const std::vector< Polygon >&, std::istream&, std::ostream&);
}

#endif
