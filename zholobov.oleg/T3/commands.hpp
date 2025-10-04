#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>

#include "polygon.hpp"

namespace zholobov {

  void cmdArea(std::istream& input, std::ostream& output, const Polygons& polygons);
  void cmdMax(std::istream& input, std::ostream& output, const Polygons& polygons);
  void cmdMin(std::istream& input, std::ostream& output, const Polygons& polygons);
  void cmdCount(std::istream& input, std::ostream& output, const Polygons& polygons);
  void cmdIntersections(std::istream& input, std::ostream& output, const Polygons& polygons);
  void cmdRects(std::ostream& output, const Polygons& polygons);

}

#endif
