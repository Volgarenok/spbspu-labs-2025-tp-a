#ifndef COMMANDS_H
#define COMMANDS_H
#include <vector>
#include <istream>
#include <ostream>
#include "geom.hpp"
namespace nikonov
{
  void getArea(const std::vector< Polygon >&, std::istream&, std::ostream&);
  void getMax(const std::vector< Polygon >&, std::istream&, std::ostream&);
  void getMin(const std::vector< Polygon >&, std::istream&, std::ostream&);
  void getCount(const std::vector< Polygon >&, std::istream&, std::ostream&);
  void getPerms(const std::vector< Polygon >&, std::istream&, std::ostream&);
  void getMaxSeq(const std::vector< Polygon >&, std::istream&, std::ostream&);
}
#endif
