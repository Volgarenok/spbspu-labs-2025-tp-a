#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <vector>
#include <iostream>
#include <map>
#include <functional>
#include "shapes.hpp"

namespace finaev
{
  void count(std::istream& in, std::ostream& out, const std::vector< Polygon >&);
  void area(std::istream& in, std::ostream& out, const std::vector< Polygon >&);
  void max(std::istream& in, std::ostream& out, const std::vector< Polygon >&);
  void min(std::istream& in, std::ostream& out, const std::vector< Polygon >&);
  void echo(std::istream& in, std::ostream& out, std::vector< Polygon >&);
  void same(std::istream& in, std::ostream& out, const std::vector< Polygon >&);
  std::map< std::string, std::function< void() > > createCommandsHandler(std::istream& in, std::ostream& out, std::vector< Polygon >&);
}

#endif
