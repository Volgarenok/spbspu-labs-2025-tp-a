#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include "polygon.hpp"

namespace smirnov
{
  void doAreaCommand(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void doMaxCommand(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void doMinCommand(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void doCountCommand(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void doRectsCommand(const std::vector< Polygon >& polygons, std::ostream& out);
  void doIntersections(const std::vector< Polygon >& data, std::istream& in, std::ostream& out);
}

#endif
