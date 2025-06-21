#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <string>
#include <vector>
#include "Polygon.hpp"

namespace gavrilova {
  void processArea(const std::vector< Polygon >& polygons, const std::vector< std::string >& commands, std::ostream& out);
  void processMinMax(const std::vector< Polygon >& polygons, const std::vector< std::string >& commands, std::ostream& out);
  void processCount(const std::vector< Polygon >& polygons, const std::vector< std::string >& commands, std::ostream& out);
  void processPerms(const std::vector< Polygon >& polygons, const std::vector< std::string >& commands, std::ostream& out);
  void processLessArea(const std::vector< Polygon >& polygons, const std::vector< std::string >& commands, std::ostream& out);
}

#endif
