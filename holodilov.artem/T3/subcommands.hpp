#ifndef SUBCOMMANDS_HPP
#define SUBCOMMANDS_HPP

#include "polygon.hpp"

namespace holodilov::subcommands
{
  void countEven(std::ostream& os, const VecPolygons& vecPolygons);
  void countOdd(std::ostream& os, const VecPolygons& vecPolygons);
  void countAmount(std::ostream& os, const VecPolygons& vecPolygons, size_t amountVertexes);

  void maxArea(std::ostream& os, const VecPolygons& vecPolygons);
  void maxVertexes(std::ostream& os, const VecPolygons& vecPolygons);

  void minArea(std::ostream& os, const VecPolygons& vecPolygons);
  void minVertexes(std::ostream& os, const VecPolygons& vecPolygons);

  void areaEven(std::ostream& os, const VecPolygons& vecPolygons);
  void areaOdd(std::ostream& os, const VecPolygons& vecPolygons);
  void areaMean(std::ostream& os, const VecPolygons& vecPolygons);
  void areaAmount(std::ostream& os, const VecPolygons& vecPolygons, size_t amountVertexes);
}

#endif
