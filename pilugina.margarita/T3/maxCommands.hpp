#ifndef MAX_COMMANDS_HPP
#define MAX_COMMANDS_HPP

#include "polygon.hpp"

namespace pilugina
{
  double updateMaxArea(double, const Polygon &);
  double updateMaxVertices(std::size_t, const Polygon &);

  void printMaxArea(const std::vector< Polygon > &, std::ostream &);
  void printMaxVertices(const std::vector< Polygon > &, std::ostream &);
  void printMax(const std::vector< Polygon > &, std::istream &, std::ostream &);
}

#endif
