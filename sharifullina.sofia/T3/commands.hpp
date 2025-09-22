#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <vector>
#include "shapes.hpp"

namespace sharifullina
{
  void printArea(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void printMax(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void printMin(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void printCount(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void printPerms(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void printLessArea(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
}

#endif
