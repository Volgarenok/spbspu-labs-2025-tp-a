#ifndef FUNCTIONAL_HPP
#define FUNCTIONAL_HPP

#include <vector>
#include "polygon.hpp"

namespace fedorov
{
  double calcPolygonArea(const Polygon &poly);
  bool validatePolygon(const Polygon &poly);
  void readPolygons(std::istream &in, std::vector< Polygon > &polygons);
}

#endif
