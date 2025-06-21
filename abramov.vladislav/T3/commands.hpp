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
  void area(const std::vector< Polygon > &polygons, std::ostream &out, std::istream &in);
  void max(const std::vector< Polygon> &polygons, std::ostream &out, std::istream &in);
  void min(const std::vector< Polygon > &polygons, std::ostream &out, std::istream &in);
  void count(const std::vector< Polygon > &polygons, std::ostream &out, std::istream &in);
  void rmecho(std::vector< Polygon > &polygons, std::ostream &out, std::istream &in);
  void perms(const std::vector< Polygon > &polygons, std::ostream &out, std::istream &in);
}

#endif
