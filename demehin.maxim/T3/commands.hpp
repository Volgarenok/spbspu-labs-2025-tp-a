#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include "geometry.hpp"

namespace demehin
{
  void printAreaSum(std::istream& in, const std::vector< Polygon >& plgs, std::ostream& out);
  void printMaxValueOf(std::istream& in, const std::vector< Polygon >& plgs, std::ostream& out);
  void printMinValueOf(std::istream& in, const std::vector< Polygon >& plgs, std::ostream& out);
  void printCountOf(std::istream& in, const std::vector< Polygon >& plgs, std::ostream& out);
  void printPermsCnt(std::istream& in, const std::vector< Polygon >& plgs, std::ostream& out);
  void printRightsCnt(const std::vector< Polygon >& plgs, std::ostream& out);
}

#endif
