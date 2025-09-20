#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <functional>
#include <iostream>
#include <vector>
#include "polygon.hpp"


namespace shchadilov
{
  void printArea(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void printMax(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void printMin(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void printCount(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void printRights(const std::vector< Polygon >& polygons, std::ostream& out);
  void printRmEcho(std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
}

#endif
