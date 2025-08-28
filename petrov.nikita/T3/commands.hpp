#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include <iostream>
#include "geometric_shapes.hpp"

namespace petrov
{
  double calculateTrapezeArea(const Point & point_1, const Point & point_2);
  double calculateArea(const Polygon & polygon);


  void addIfEven(const std::vector< Polygon > & polygons, std::ostream & out);
  void addIfOdd(const std::vector< Polygon > & polygons, std::ostream & out);
  void mean(const std::vector< Polygon > & polygons, std::ostream & out);
  void sum(size_t & num_of_vertexes, const std::vector< Polygon > & polygons, std::ostream & out);

  void area(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out);


  size_t getVertexesNum(const Polygon & polygon);

  void max_area(const std::vector< Polygon > & polygons, std::ostream & out);
  void max_vertexes(const std::vector< Polygon > & polygons, std::ostream & out);

  void max(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out);


  void min_area(const std::vector< Polygon > & polygons, std::ostream & out);
  void min_vertexes(const std::vector< Polygon > & polygons, std::ostream & out);

  void min(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out);

  void countIfEven(const std::vector< Polygon > & polygons, std::ostream & out);
  void countIfOdd(const std::vector< Polygon > & polygons, std::ostream & out);
  void countIfThisNumber(size_t & num_of_vertexes, const std::vector< Polygon > & polygons, std::ostream & out);

  void count(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out);


  void rmecho(std::vector< Polygon > & polygons, std::istream & in, std::ostream & out);


  void maxseq(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out);
}

#endif
