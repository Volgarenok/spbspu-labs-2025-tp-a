#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

namespace ivanova
{
  struct Polygon;
}

namespace ivanova
{
  void areaCommand(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void maxCommand(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void minCommand(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void countCommand(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void echoCommand(std::istream&, std::ostream&, std::vector< Polygon >&);
  void sameCommand(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void printCommand(std::ostream&, const std::vector< Polygon>&);

  double area(const std::vector < Polygon >&, const std::string&);
  double area(const std::vector < Polygon >&, std::size_t);
  double maxArea(const std::vector < Polygon >&);
  double minArea(const std::vector < Polygon >&);
  std::size_t maxVertexes(const std::vector < Polygon >&);
  std::size_t minVertexes(const std::vector < Polygon >&);
  std::size_t count(const std::vector < Polygon >&, const std::string&);
  std::size_t count(const std::vector < Polygon >&, std::size_t);
  std::size_t echo(std::vector < Polygon >&, const Polygon&);
  std::size_t same(const std::vector < Polygon >&, const Polygon&);
}

#endif
