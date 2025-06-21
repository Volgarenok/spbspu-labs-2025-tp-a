#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "ioGeometry.hpp"

namespace bocharov
{
  void getArea(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void getMax(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void getMin(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void getCount(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void getMaxSeqCommand(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void getRightsCnt(std::ostream & out, const std::vector< Polygon > & polygons);
}

#endif
