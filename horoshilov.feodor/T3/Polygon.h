#ifndef POLYGON_H
#define POLYGON_H

#include <iostream>
#include <ostream>
#include <vector>
#include "Point.h"

namespace horoshilov
{
  struct Polygon
  {
    std::vector< Point > points;
    bool operator==(const Polygon& other) const;
  };
  std::istream& operator>>(std::istream& in, Polygon& poly);
}

#endif
