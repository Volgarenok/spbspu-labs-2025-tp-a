#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include "polygon.hpp"

namespace sveshnikov
{
  void area(std::istream &in, std::ostream &out, const std::vector< Polygon > &shapes);
  void max(std::istream &in, std::ostream &out, const std::vector< Polygon > &shapes);
  void min(std::istream &in, std::ostream &out, const std::vector< Polygon > &shapes);
  void count(std::istream &in, std::ostream &out, const std::vector< Polygon > &shapes);
  void maxseq(std::istream &in, std::ostream &out, const std::vector< Polygon > &shapes);
  void rmecho(std::istream &in, std::ostream &out, std::vector< Polygon > &shapes);
}

#endif
