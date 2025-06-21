#ifndef ECHO_HPP
#define ECHO_HPP

#include <vector>
#include "polygon.hpp"

namespace pilugina
{
  using vecIt = std::vector< pilugina::Polygon >::iterator;
  std::size_t echoInner(std::vector< Polygon > &polys, const Polygon &poly, vecIt it, std::size_t count);
  std::size_t echo(std::vector< Polygon > &, const Polygon &);
}

#endif
