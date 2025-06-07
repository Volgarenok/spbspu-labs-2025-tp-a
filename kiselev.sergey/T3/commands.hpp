#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include "polygon.hpp"
#include <iosfwd>
#include <vector>
namespace kiselev
{
  void area(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void max(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void min(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void count(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void lessArea(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void rects(std::ostream&, const std::vector< Polygon >&);
}
#endif
