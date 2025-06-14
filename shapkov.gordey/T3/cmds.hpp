#ifndef CMDS_HPP
#define CMDS_HPP
#include <vector>
#include <iostream>
#include "GeometricalTypes.hpp"

namespace shapkov
{
  using VecOfPolygons = std::vector< Polygon >;
  void area(std::istream& in, std::ostream& out, const VecOfPolygons& src);
  void areaEven(std::ostream& out, const VecOfPolygons& src);
  void areaOdd(std::ostream& out, const VecOfPolygons& src);
  void areaMean(std::ostream& out, const VecOfPolygons& src);
  void areaVertexes(std::ostream& out, const VecOfPolygons& src, size_t vertexes);
  void max(std::istream& in, std::ostream& out, const VecOfPolygons& src);
  void maxArea(std::ostream& out, const VecOfPolygons& src);
  void maxVertexes(std::ostream& out, const VecOfPolygons& src);
  void min(std::istream& in, std::ostream& out, const VecOfPolygons& src);
  void minArea(std::ostream& out, const VecOfPolygons& src);
  void minVertexes(std::ostream& out, const VecOfPolygons& src);
  void count(std::istream& in, std::ostream& out, const VecOfPolygons& src);
  void countEven(std::ostream& out, const VecOfPolygons& src);
  void countOdd(std::ostream& out, const VecOfPolygons& src);
  void countVertexes(std::ostream& out, const VecOfPolygons& src, size_t vertexes);
  void rects(std::ostream& out, const VecOfPolygons& src);
  void same(std::istream& in, std::ostream& out, const VecOfPolygons& src);
}

#endif
