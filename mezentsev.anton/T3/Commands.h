#ifndef COMMANDS_H
#define COMMANDS_H
#include "Polygon.h"
#include <iosfwd>
#include <vector>

namespace mezentsev
{
  void area(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void max(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void min(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void count(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void lessArea(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void maxSeq(std::istream&, std::ostream&, const std::vector< Polygon >&);
}
#endif
