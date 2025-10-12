#ifndef USER_COMMANDS_HPP
#define USER_COMMANDS_HPP
#include <iostream>
#include <iomanip>
#include <functional>
#include <numeric>
#include <map>
#include <stream-guard.hpp>
#include "shapes.hpp"
#include "sub-utils.hpp"

namespace alymova
{
  using namespace std::placeholders;

  void area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void inFrame(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void rightShapes(std::ostream& out, const std::vector< Polygon >& polygons);
}
#endif
