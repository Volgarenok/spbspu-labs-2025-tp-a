#ifndef ECHO_RMECHO_HPP
#define ECHO_RMECHO_HPP

#include "shapes.hpp"

namespace maslevtsov {
  void echo(std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void remove_echo(std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
}

#endif
