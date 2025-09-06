#ifndef MAX_COMMANDS_HPP
#define MAX_COMMANDS_HPP

#include "polygon.hpp"

namespace pilugina
{
  void printMaxArea(const std::vector< Polygon > &, std::ostream &);
  void printMaxVertices(const std::vector< Polygon > &, std::ostream &);
  void printMax(const std::vector< Polygon > &, std::istream &, std::ostream &);
}

#endif
