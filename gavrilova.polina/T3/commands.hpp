#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <string>
#include <vector>
#include "Polygon.hpp"

namespace gavrilova {
  void execArea(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void execMax(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void execMin(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void execCount(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void execPerms(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void execLessArea(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
}

#endif
