#ifndef RM_ECHO_COMMAND_HPP
#define RM_ECHO_COMMAND_HPP

#include <cstddef>
#include <vector>
#include "polygon.hpp"

namespace mazitov
{
  std::size_t getRmEcho(std::vector< Polygon > &polygons, const Polygon &ref);
}

#endif
