#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <vector>
#include "polygon.hpp"

namespace fedorov
{
  namespace max
  {
    void processArea(const std::vector< Polygon > &polygons, std::ostream &out);
    void processVertexes(const std::vector< Polygon > &polygons, std::ostream &out);
  }

  namespace min
  {
    void processArea(const std::vector< Polygon > &polygons, std::ostream &out);
    void processVertexes(const std::vector< Polygon > &polygons, std::ostream &out);
  }

  namespace count
  {
    void processEven(const std::vector< Polygon > &polygons, std::ostream &out);
    void processOdd(const std::vector< Polygon > &polygons, std::ostream &out);
    void processVertexCount(const std::vector< Polygon > &polygons, size_t num, std::ostream &out);
  }

  namespace area
  {
    void processEven(const std::vector< Polygon > &polygons, std::ostream &out);
    void processOdd(const std::vector< Polygon > &polygons, std::ostream &out);
    void processMean(const std::vector< Polygon > &polygons, std::ostream &out);
    void processVertexCount(const std::vector< Polygon > &polygons, size_t num, std::ostream &out);
  }

  void areaCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void maxCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys);
  void minCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys);
  void countCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void lessAreaCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void echoCommand(std::istream &in, std::ostream &out, std::vector< Polygon > &polygons);
}

#endif
