#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "polygon.hpp"
#include "functional.hpp"

namespace puzikov
{
  void checkVerticesParameter(const std::string &);
  void areaCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void minmaxCommand(std::istream &, std::ostream &, const std::vector< Polygon > &, AreaAlgo, VertAlgo);
  void maxCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void minCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void countCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void rmEchoCommand(std::istream &, std::ostream &, std::vector< Polygon > &);
  void sameCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
}

#endif
