#ifndef COMMANDS_H
#define COMMANDS_H

#include <iosfwd>
#include <vector>
#include "Polygon.h"

namespace horoshilov
{
  void printArea(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void printMax(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void printMin(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void printCount(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void printIntersections(std::istream& in, std::ostream& out, const std::vector< horoshilov::Polygon >& polygons);
  void printPermsCount(std::istream& in, std::ostream& out, const std::vector< horoshilov::Polygon >& plgs);
}

#endif
