#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iomanip>
#include <map>
#include <string>
#include <functional>
#include "polygon.hpp"

namespace shak
{
  void createCommandHandler(std::map< std::string, std::function< void() > > &cmds, std::vector< Polygon > &polygons);
  void cmdArea(const std::vector< Polygon > &polygons, std::istream &in, std::ostream &out);
  void cmdMax(const std::vector< Polygon > &polygons, std::istream &in, std::ostream &out);
  void cmdMin(const std::vector< Polygon > &polygons, std::istream &in, std::ostream &out);
  void cmdCount(const std::vector< Polygon > &polygons, std::istream &in, std::ostream &out);
  void cmdMaxSeq(const std::vector< Polygon > &polygons, std::istream &in, std::ostream &out);
  void cmdRects(const std::vector< Polygon > &polygons, std::ostream &out);
  void cmdMassCenter(const std::vector< Polygon > &polygons, std::istream &in, std::ostream &out);
}
#endif
