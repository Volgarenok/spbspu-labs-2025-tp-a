#ifndef SHAPES_CMDS_H
#define SHAPES_CMDS_H
#include <iostream>
#include "shapes.h"

namespace savintsev
{
  void area(std::istream & in, std::ostream & out, const std::vector< Polygon > & data);
  void count(std::istream & in, std::ostream & out, const std::vector< Polygon > & data);
  void max(std::istream & in, std::ostream & out, const std::vector< Polygon > & data);
  void min(std::istream & in, std::ostream & out, const std::vector< Polygon > & data);
  void intersections(std::istream & in, std::ostream & out, const std::vector< Polygon > & data);
  void same(std::istream & in, std::ostream & out, const std::vector< Polygon > & data);
  void print(std::ostream & out, const std::vector< Polygon > & data);
}

#endif
