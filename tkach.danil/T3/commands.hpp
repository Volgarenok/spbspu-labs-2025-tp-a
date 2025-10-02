#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include <iostream>
#include "shapes.hpp"

namespace tkach
{
  void printArea(std::istream& in, std::ostream& out, const std::vector< Polygon >& data);
  void printMax(std::istream& in, std::ostream& out, const std::vector< Polygon >& data);
  void printMin(std::istream& in, std::ostream& out, const std::vector< Polygon >& data);
  void printCount(std::istream& in, std::ostream& out, const std::vector< Polygon >& data);
  void printSame(std::istream& in, std::ostream& out, const std::vector< Polygon >& data);
  void printRects(std::ostream& out, const std::vector< Polygon >& data);
}

#endif
