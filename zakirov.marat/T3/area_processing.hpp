#ifndef AREA_PROCESSING_HPP
#define AREA_PROCESSING_HPP
#include <list>
#include "geometric_shape.hpp"

namespace zakirov
{
  void process_area(const std::list< Polygon > & points, std::istream & in, std::ostream & out);
  void process_less_area(const std::list< Polygon > & points, std::istream & in, std::ostream & out);
}

#endif
