#ifndef PRINTCMD_HPP
#define PRINTCMD_HPP

#include <iostream>
#include <vector>
#include "polygon.hpp"

namespace dribas
{
  void printArea(const std::vector< Polygon >&, std::istream&, std::ostream&);
  void printMax(const std::vector< Polygon >&, std::istream&, std::ostream&);
  void printMin(const std::vector< Polygon >&, std::istream&, std::ostream&);
  void printCount(const std::vector< Polygon >&, std::istream&, std::ostream&);
  void printLessArea(const std::vector< Polygon >&, std::istream&, std::ostream&);
  void printRightShapes(const std::vector< Polygon >&, std::ostream&);
}

#endif
