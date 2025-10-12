#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iosfwd>
#include <vector>

#include "geometry.hpp"

namespace kostyukov
{
  void area(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void max(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void min(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void count(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void permsCount(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void rightShapesCount(std::ostream&, const std::vector< Polygon >&);
}
#endif
