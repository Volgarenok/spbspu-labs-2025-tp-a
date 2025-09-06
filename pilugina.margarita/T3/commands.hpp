#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "polygon.hpp"

namespace pilugina
{
  void areaCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void maxCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void minCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void countCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void echoCommand(std::istream &, std::ostream &, std::vector< Polygon > &);
  void intersectsCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void permsCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void perimeterCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
}

#endif
