#ifndef MIN_COMMANDS_HPP
#define MIN_COMMANDS_HPP

#include "polygon.hpp"

namespace pilugina
{
  double updateMinArea(double, const Polygon &);
  double updateMinVertices(std::size_t, const Polygon &);

  void printMinArea(const std::vector< Polygon > &, std::ostream &);
  void printMinVertices(const std::vector< Polygon > &, std::ostream &);
  void printMin(const std::vector< Polygon > &, std::istream &, std::ostream &);
}

#endif
