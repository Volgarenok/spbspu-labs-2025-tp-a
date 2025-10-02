#ifndef COMMAND_HANDLERS_HPP
#define COMMAND_HANDLERS_HPP
#include <iostream>
#include <map>
#include "command-helpers.hpp"
#include "shapes.hpp"

namespace belyaev
{
  template< typename Pred >
  void areaOddEven(const std::vector< Polygon >& data, std::ostream& out, Pred predicate);
  void areaMean(const std::vector< Polygon >& data, std::ostream& out);
  void areaVertices(const std::vector< Polygon >& data, std::ostream& out, size_t vertices);
  void area(const std::vector< Polygon >& data, std::istream& in, std::ostream& out);

  void minMaxArea(const std::vector< Polygon >& data, std::ostream& out, const std::string& command);
  void minMaxVertices(const std::vector< Polygon >& data, std::ostream& out, const std::string& command);
  void minMax(const std::vector< Polygon >& data, std::istream& in, std::ostream& out, const std::string& command);

  void countEven(const std::vector< Polygon >& data, std::ostream& out);
  void countOdd(const std::vector< Polygon >& data, std::ostream& out);
  void countVertices(const std::vector< Polygon >& data, std::ostream& out, size_t givenSize);
  void count(const std::vector< Polygon >& data, std::istream& in, std::ostream& out);

  void rmecho(std::vector< Polygon >& data, std::istream& in, std::ostream& out);
  void inframe(const std::vector< Polygon >& data, std::istream& in, std::ostream& out);

  using commandMap = std::map< std::string, std::function< void(std::istream&, std::ostream&) > >;
  commandMap mapCommandHandlers(std::vector< Polygon >& data);
}

#endif
