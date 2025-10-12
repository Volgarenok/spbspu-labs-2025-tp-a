#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <map>
#include <vector>
#include <string>
#include <functional>
#include "geom.hpp"

namespace abramov
{
  void getCommands(std::map< std::string, std::function< void() > > &commands, std::vector< Polygon > &polygons);
  void doAreaComm(const std::vector< Polygon > &polygons, std::ostream &out, std::istream &in);
  void doMaxComm(const std::vector< Polygon> &polygons, std::ostream &out, std::istream &in);
  void doMinComm(const std::vector< Polygon > &polygons, std::ostream &out, std::istream &in);
  void doCountComm(const std::vector< Polygon > &polygons, std::ostream &out, std::istream &in);
  void doRmechoComm(std::vector< Polygon > &polygons, std::ostream &out, std::istream &in);
  void doPermsComm(const std::vector< Polygon > &polygons, std::ostream &out, std::istream &in);
}

#endif
