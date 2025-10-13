#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <functional>
#include <iostream>
#include <vector>
#include "polygon.hpp"


namespace shchadilov
{
  void printArea(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void printMax(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void printMin(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void printCount(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void printRights(std::ostream& out, const std::vector< Polygon >& polygons);
  void printRmEcho(std::istream& in, std::ostream& out, std::vector< Polygon >& polygons);
}

#endif
