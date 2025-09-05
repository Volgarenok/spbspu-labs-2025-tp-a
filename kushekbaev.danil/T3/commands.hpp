#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include "structs.hpp"

namespace kushekbaev
{
  void area(std::istream& in, std::ostream out, const std::vector< Polygon >&);
  void max(std::istream& in, std::ostream out, const std::vector< Polygon >&);
  void min(std::istream& in, std::ostream out, const std::vector< Polygon >&);
  void count(std::istream& in, std::ostream out, const std::vector< Polygon >&);
  void rightshapes(std::ostream out, const std::vector< Polygon >&);
  void same(std::istream& in, std::ostream out, const std::vector< Polygon >&);
}

#endif
