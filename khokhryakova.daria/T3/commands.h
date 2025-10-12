#ifndef COMMANDS_H
#define COMMANDS_H

#include <iosfwd>
#include <vector>
#include <string>
#include "poligon.h"

namespace khokhryakova
{
  void area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void echo(std::istream& in, std::ostream& out, std::vector< Polygon >& polygons, const std::string& filename);
  void maxSeq(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
}

#endif
