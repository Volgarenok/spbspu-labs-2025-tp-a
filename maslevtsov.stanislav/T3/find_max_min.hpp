#ifndef FIND_MAX_MIN_HPP
#define FIND_MAX_MIN_HPP

#include "shapes.hpp"

namespace maslevtsov {
  void find_max(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void find_min(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
}

#endif
