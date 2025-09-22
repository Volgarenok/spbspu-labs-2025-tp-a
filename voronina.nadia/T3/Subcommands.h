#ifndef SUBCOMMANDS_H
#define SUBCOMMANDS_H

#include <iosfwd>
#include <vector>

#include "Shapes.h"

namespace voronina
{
  int maxSeqFolder(int left, int right);
  bool isThereRightAngleInPolygon(const voronina::Polygon& shape);

  void getEven(const std::vector< Polygon >& shapes, std::ostream& out);
  void getOdd(const std::vector< Polygon >& shapes, std::ostream& out);
  void getMean(const std::vector< Polygon >& shapes, std::ostream& out);
  void getVertexes(const std::vector< Polygon >& shapes, std::ostream& out, int vertexes);

  void getAreaMax(const std::vector< Polygon >& shapes, std::ostream& out);
  void getVertexesMax(const std::vector< Polygon >& shapes, std::ostream& out);

  void getAreaMin(const std::vector< Polygon >& shapes, std::ostream& out);
  void getVertexesMin(const std::vector< Polygon >& shapes, std::ostream& out);

  void getEvenCount(const std::vector< Polygon >& shapes, std::ostream& out);
  void getOddCount(const std::vector< Polygon >& shapes, std::ostream& out);
  void getVertexesCount(const std::vector< Polygon >& shapes, std::ostream& out, int vertexes);

  void getRegularEvenCount(const std::vector< Polygon >& shapes, std::ostream& out);
  void getRegularOddCount(const std::vector< Polygon >& shapes, std::ostream& out);
  void getRegularVertexesCount(const std::vector< Polygon >& shapes, std::ostream& out, int vertexes);
}
#endif
