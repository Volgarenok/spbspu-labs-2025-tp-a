#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "ioGeometry.hpp"

namespace maslov
{
  void getArea(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void getMax(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void getMin(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void getCount(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void getEcho(std::istream & in, std::ostream & out, std::vector< Polygon > & polygons);
  void getRects(std::ostream & out, const std::vector< Polygon > & polygons);
}

#endif
