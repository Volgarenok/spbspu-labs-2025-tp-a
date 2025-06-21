#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "polygon.hpp"

namespace mazitov
{
  void areaCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void maxCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void minCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void countCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void intersectsCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void rmEchoCommand(std::istream &, std::ostream &, std::vector< Polygon > &);
}

#endif
