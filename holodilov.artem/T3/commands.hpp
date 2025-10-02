#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "polygon.hpp"

namespace holodilov::commands
{
  void area(std::istream& is, std::ostream& os, const VecPolygons& vecPolygons);

  void min(std::istream& is, std::ostream& os, const VecPolygons& vecPolygons);

  void max(std::istream& is, std::ostream& os, const VecPolygons& vecPolygons);

  void count(std::istream& is, std::ostream& os, const VecPolygons& vecPolygons);

  void echo(std::istream& is, std::ostream& os, VecPolygons& vecPolygons);

  void lessArea(std::istream& is, std::ostream& os, const VecPolygons& vecPolygons);
}

#endif
