#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <string>
#include "geometry.h"
namespace asafov
{
  void processCommand(const std::vector< Polygon >& polygons, const std::string& cmd);
}
#endif
