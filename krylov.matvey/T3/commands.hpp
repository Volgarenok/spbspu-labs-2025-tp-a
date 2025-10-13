#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <map>
#include <vector>
#include <string>
#include <functional>
#include "geometry.hpp"

namespace krylov
{
  void doAreaComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in);
  void doMaxComm(const std::vector< Polygon>& polygons, std::ostream& out, std::istream& in);
  void doMinComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in);
  void doCountComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in);
  void doMaxseqComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in);
  void doIntersectComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in);
  void doIsCuttedComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in);
}

#endif
