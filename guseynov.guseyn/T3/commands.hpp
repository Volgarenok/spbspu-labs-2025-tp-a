#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <vector>
#include "polygon.hpp"

namespace guseynov
{
  namespace commands
  {
    void handleAreaCommand(const std::vector< Polygon >& polygons, const std::string& param);
    void handleMaxCommand(const std::vector< Polygon >& polygons, const std::string& param);
    void handleMinCommand(const std::vector< Polygon >& polygons, const std::string& param);
    void handleCountCommand(const std::vector< Polygon >& polygons, const std::string& param);
  }
}

#endif
