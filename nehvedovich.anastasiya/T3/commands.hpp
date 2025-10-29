#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include <string>
#include "polygon.hpp"

namespace nehvedovich
{
  void areaCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void maxCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void minCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void countCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void inFrameCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void lessAreaCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
}

#endif
