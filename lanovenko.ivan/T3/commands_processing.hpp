#ifndef COMMANDS_PROCESSING_HPP
#define COMMANDS_PROCESSING_HPP

#include <iostream>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <functional>
#include <map>
#include <string>
#include <scope_guard.hpp>
#include "shapes.hpp"

namespace lanovenko
{
  void printArea(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void printMax(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void printMin(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void printCount(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void printRmecho(std::istream& in, std::ostream& out, std::vector< Polygon >& polygons);
  void printRightShapes(std::ostream& out, const std::vector< Polygon >& polygons);
}

#endif
