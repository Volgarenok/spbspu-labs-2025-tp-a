#ifndef CMDS_RUN_H
#define CMDS_RUN_H
#include <iosfwd>
#include <vector>
#include "polygon.h"

namespace ohantsev
{
  void fillPolygons(std::vector< Polygon >& polygons, std::ifstream& in);
  void cmdsHandle(std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
}
#endif
