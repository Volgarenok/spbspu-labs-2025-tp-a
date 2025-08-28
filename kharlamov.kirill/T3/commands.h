#ifndef COMMANDS_H
#define COMMANDS_H

#include <iosfwd>
#include <vector>
#include "polygon.h"

namespace kharlamov
{
  using PolygonVec = std::vector< Polygon >;
  void doAreaCommand(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void doMaxCommand(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void doMinCommand(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void doCountCommand(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void doMaxSeqCommand(std::istream& in, std::ostream& out, const PolygonVec& polygons);
  void doSameCommand(std::istream& in, std::ostream& out, const PolygonVec& polygons);
}

#endif
