#ifndef INFRAME_HPP
#define INFRAME_HPP

#include <string>
#include <vector>
#include "polygon.hpp"

namespace guseynov
{
  namespace commands
  {
    void handleInFrameCommand(const std::vector< Polygon >& polygons, const std::string& param);
  }
}

#endif
