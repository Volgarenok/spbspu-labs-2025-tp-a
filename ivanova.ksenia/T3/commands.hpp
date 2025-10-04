#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include <iostream>
#include "polygon.hpp"

namespace ivanova
{
  void area(std::istream& in, std::ostream& out, const std::vector < Polygon >& src);
  void max(std::istream& in, std::ostream& out, const std::vector < Polygon >& src);
  void min(std::istream& in, std::ostream& out, const std::vector < Polygon >& src);
  void count(std::istream& in, std::ostream& out, const std::vector < Polygon >& src);
  void echo(std::istream& in, std::ostream& out, std::vector < Polygon >& src);
  void same(std::istream& in, std::ostream& out, const std::vector < Polygon >& src);
}

#endif
