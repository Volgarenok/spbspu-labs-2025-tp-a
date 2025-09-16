#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <vector>
#include <ostream>
#include "point.hpp"

namespace kazak {
  struct Polygon {
    std::vector<Point> points;

    double area() const;
    size_t vertexCount() const;
    bool operator==(const Polygon & other) const;
  };

  std::ostream & operator<<(std::ostream & os, const Polygon & poly);
}

#endif
