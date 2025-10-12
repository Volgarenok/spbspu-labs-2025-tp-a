#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <iomanip>
#include <functional>
#include <numeric>
#include <map>
#include <stream_guard.hpp>
#include "shapes.hpp"

namespace cherepkov
{
  using namespace std::placeholders;

  void area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void inFrame(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void same(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
}
#endif
