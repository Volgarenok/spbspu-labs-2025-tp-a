#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include <iostream>
#include "polygon.hpp"

namespace trukhanov
{
  void area(std::istream& in, std::ostream& out, const std::vector < Polygon >& src);
  void max(std::istream& in, std::ostream& out, const std::vector < Polygon >& src);
  void min(std::istream& in, std::ostream& out, const std::vector < Polygon >& src);
  void count(std::istream& in, std::ostream& out, const std::vector < Polygon >& src);
  void lessArea(std::istream& in, std::ostream& out, const std::vector < Polygon >& src);
  void right(std::istream& in, std::ostream& out, const std::vector < Polygon >& src);
  void commonSide(std::istream& in, std::ostream& out, const std::vector < Polygon >& src);
}

#endif
