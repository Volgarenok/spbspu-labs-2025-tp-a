#ifndef INTERSECTIONS_COMMAND_HPP
#define INTERSECTIONS_COMMAND_HPP

#include "polygon.hpp"

namespace mazitov
{
  struct IntersectsWith
  {
    IntersectsWith(const Polygon &polygon);
    bool operator()(const Polygon &other) const;

  private:
    Polygon polygon_;
  };
}

#endif
