#ifndef COMMANDS_H
#define COMMANDS_H

#include "Shapes.h"

namespace voronina
{
  void area(const std::vector< Polygon >& shapes, std::istream& in, std::ostream& out);
  void max(const std::vector< Polygon >& shapes, std::istream& in, std::ostream& out);
  void min(const std::vector< Polygon >& shapes, std::istream& in, std::ostream& out);
  void count(const std::vector< Polygon >& shapes, std::istream& in, std::ostream& out);
  void maxseq(const std::vector< Polygon >& shapes, std::istream& in, std::ostream& out);
  void countRegular(const std::vector< Polygon >& shapes, std::istream& in, std::ostream& out);
  void rightshapes(const std::vector< Polygon >& shapes, std::ostream& out);
}
#endif
