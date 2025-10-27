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

  std::size_t Polygon::size() const
  {
    return points.size();
  }

  double Polygon::area() const
  {
    if (points.size() < 3)
    {
      return 0.0;
    }
    double sum = std::inner_product(
      points.begin(), points.end() - 1,
      points.begin() + 1,
      0.0,
      std::plus<>(),
      crossMultiply
    );
    sum += crossMultiply(points.back(), points.front());
    return std::abs(sum) * 0.5;
  }

  bool Polygon::operator==(const Polygon& other) const
  {
    return points == other.points;
  }

  double Polygon::crossMultiply(const Point& a, const Point& b)
  {
    return a.x * b.y - a.y * b.x;
  }
}