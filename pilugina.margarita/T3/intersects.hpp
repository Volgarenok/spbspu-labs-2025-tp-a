#ifndef INTERSECTS_HPP
#define INTERSECTS_HPP

#include "polygon.hpp"

namespace pilugina
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
