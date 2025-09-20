#ifndef VERTEX_PROCESSING_HPP
#define VERTEX_PROCESSING_HPP
#include <list>
#include "geometric_shape.hpp"

namespace zakirov
{
  void count(const std::list< Polygon > & points, std::istream & in, std::ostream & out);
  void echo(std::list< Polygon > & points, std::istream & in, std::ostream & out);
}

#endif
