#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include "polygon.hpp"
#include <iosfwd>
#include <vector>
namespace kiselev
{
  void doAreaCommand(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void doMaxCommand(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void doMinCommand(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void doCountCommand(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void doLessAreaCommand(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void doRectsCommand(std::ostream&, const std::vector< Polygon >&);
}
#endif
