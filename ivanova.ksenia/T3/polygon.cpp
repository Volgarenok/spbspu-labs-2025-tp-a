#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iterator>
#include <numeric>
#include <utility>

#include "polygon.hpp"
#include "streamGuard.hpp"

namespace ivanova
{
  bool Point::operator==(const Point& other) const
  {
    return x == other.x && y == other.y;
  }

  Point Point::operator-(const Point& other) const
  {
    return { x - other.x, y - other.y };
  }

  Point Point::operator-() const
  {
    return { -x, -y };
  }
}