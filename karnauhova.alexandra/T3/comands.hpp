#ifndef COMANDS_HPP
#define COMANDS_HPP
#include <iostream>
#include <vector>
#include "shapes.hpp"

namespace karnauhova
{
  void areaComands(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  double meanArea(const std::vector< Polygon >& polygons);
  void maxComands(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void maxArea(const std::vector< Polygon >& polygons, std::ostream& out);
  void maxVert(const std::vector< Polygon >& polygons, std::ostream& out);
  void minComands(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void minArea(const std::vector< Polygon >& polygons, std::ostream& out);
  void minVert(const std::vector< Polygon >& polygons, std::ostream& out);
  void countComands(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void lessareaComand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void echoComand(std::istream& in, std::ostream& out, std::vector< Polygon >& polygons);
}
#endif
