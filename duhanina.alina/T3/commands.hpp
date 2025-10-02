#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include <functional>
#include <iostream>
#include "polygon.hpp"

namespace duhanina
{
  void printAreaSum(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out);
  void printMaxValue(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out);
  void printMinValue(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out);
  void printCount(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out);
  void printLessArea(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out);
  void eraseRmecho(std::istream& in, std::vector< Polygon >& polygons, std::ostream& out);
}

#endif
