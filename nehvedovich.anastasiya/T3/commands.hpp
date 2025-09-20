#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "polygon.hpp"
#include <functional>
#include <map>
#include <vector>

namespace nehvedovich
{

  void areaCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void maxCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void minCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void countCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void lessAreaCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void inFrameCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void sameCommand(std::istream &in, std::ostream &out, std::vector< Polygon > &polygons);

}

#endif
