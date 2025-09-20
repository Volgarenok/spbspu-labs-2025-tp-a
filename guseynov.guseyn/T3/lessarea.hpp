#ifndef LESSAREA_HPP
#define LESSAREA_HPP

#include <string>
#include <vector>
#include "polygon.hpp"

namespace guseynov
{
  namespace commands
  {
    void handleLessAreaCommand(const std::vector< Polygon >& polygons, const std::string& param);
  }
}

#endif
