#ifndef USER_COMMANDS_HPP
#define USER_COMMANDS_HPP
#include <map>
#include <functional>
#include <iostream>
#include "shapes.hpp"

namespace alymova
{
  using namespace std::placeholders;
  using CommandDataset = std::map< std::string, std::function< void(const std::vector< Polygon >&) > >;
  using AreaMaxMinSubcommands = std::map< std::string, std::function< double(double, const Polygon&) > >;
  using CountSubcommands = std::map< std::string, std::function< bool(const Polygon&) > >;
  template< class T >
  using Comparator = std::function< T(T, T) >;

  void area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void maxAndMin(Comparator< double > cmp_area, Comparator< size_t > cmp_vertexes, std::istream& in,
    std::ostream& out, const std::vector< Polygon >& polygons);
  void count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void inFrame(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void rightShapes(std::ostream& out, const std::vector< Polygon >& polygons);

  CommandDataset complectCommands(std::istream& in, std::ostream& out);
}
#endif
