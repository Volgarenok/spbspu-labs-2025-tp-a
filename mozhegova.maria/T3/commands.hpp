#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <vector>
#include "shapes.hpp"

namespace mozhegova
{
  void printArea(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void printMax(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void printMin(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void printCount(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void printEcho(std::istream & in, std::ostream & out, std::vector< Polygon > & polygons);
  void printSame(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
}

#endif
