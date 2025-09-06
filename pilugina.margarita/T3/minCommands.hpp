#ifndef MIN_COMMANDS_HPP
#define MIN_COMMANDS_HPP

#include "polygon.hpp"

namespace pilugina
{
  void printMinArea(const std::vector< Polygon > &, std::ostream &);
  void printMinVertices(const std::vector< Polygon > &, std::ostream &);
  void printMin(const std::vector< Polygon > &, std::istream &, std::ostream &);
}

#endif
