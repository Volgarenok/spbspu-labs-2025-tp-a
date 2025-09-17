#ifndef PRINTCMD_HPP
#define PRINTCMD_HPP

#include <iostream>
#include <vector>
#include "polygon.hpp"

namespace shramko
{
  void printArea(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void printMax(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void printMin(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void printCount(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void printLessArea(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void printRightShapes(const std::vector< Polygon >& polygons, std::ostream& out);
}

#endif
