#include "polygon.hpp"
#include <cmath>

namespace kazak {
  double Polygon::area() const
  {
    double result = 0.0;
    size_t n = points.size();
    for (size_t i = 0; i < n; ++i) {
      size_t j = (i + 1) % n;
      result += (points[i].x * points[j].y) - (points[j].x * points[i].y);
    }
    return std::abs(result) / 2.0;
  }

  size_t Polygon::vertexCount() const
  {
    return points.size();
  }

  bool Polygon::operator==(const Polygon & other) const
  {
    return points == other.points;
  }

  std::ostream & operator<<(std::ostream & os, const Polygon & poly)
  {
    os << poly.vertexCount();
    for (const auto & p : poly.points) {
      os << " " << p;
    }
    return os;
  }
}

