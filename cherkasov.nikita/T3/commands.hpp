#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include <string>
#include "geometry.hpp"

namespace cherkasov
{
  void doAreaComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in);
  void doMaxComm(const std::vector< Polygon>& polygons, std::ostream& out, std::istream& in);
  void doMinComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in);
  void doCountComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in);
  void doIntersectComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in);
  void doRightShapesComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in);
}

#endif
