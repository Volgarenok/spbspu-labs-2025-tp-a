#ifndef EXTREMUM_PROCESSING_HPP
#define EXTREMUM_PROCESSING_HPP
#include <list>
#include "geometric_shape.hpp"

namespace zakirov
{
  void find_max_extremum(const std::list< Polygon > & points, std::istream & in, std::ostream & out);
  void find_min_extremum(const std::list< Polygon > & points, std::istream & in, std::ostream & out);
}

#endif
