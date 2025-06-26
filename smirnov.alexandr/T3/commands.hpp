#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <functional>
#include <string>
#include "polygon.hpp"

namespace smirnov
{
  void areaCommand(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out);
  void maxCommand(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out);
  void minCommand(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out);
  void countCommand(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out);
  void inframeCommand(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out);
  void maxseqCommand(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out);
}
#endif
