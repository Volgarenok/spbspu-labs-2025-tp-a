#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include "polygon.hpp"

namespace smirnov
{
  void printArea(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void printMax(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void printMin(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void printCount(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void printInFrame(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void printMaxSeq(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
}
#endif
