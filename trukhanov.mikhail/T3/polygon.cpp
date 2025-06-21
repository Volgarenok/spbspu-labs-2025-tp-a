#include "polygon.hpp"
#include <algorithm>
#include <numeric>
#include <vector>
#include "functors.hpp"

double trukhanov::getArea(const Polygon& polygon)
{
  std::size_t n = polygon.points.size();
  if (n < 3)
    return 0.0;

  std::vector< std::size_t > indices(n - 2);
  std::iota(indices.begin(), indices.end(), 1);

  PolygonAreaSum summer(polygon);

  double area = std::accumulate(indices.begin(), indices.end(), 0.0, summer);
  return area;
}

bool trukhanov::isEven(const Polygon& polygon)
{
  return polygon.points.size() != 0 && polygon.points.size() % 2 == 0;
}

bool trukhanov::isOdd(const Polygon& polygon)
{
  return polygon.points.size() != 0 && polygon.points.size() % 2 != 0;
}

bool trukhanov::compareByVertexes(const Polygon& lhs, const Polygon& rhs)
{
  return lhs.points.size() < rhs.points.size();
}

static int dot(const trukhanov::Point& a, const trukhanov::Point& b)
{
  return a.x * b.x + a.y * b.y;
}

static trukhanov::Point sub(const trukhanov::Point& a, const trukhanov::Point& b)
{
  return { a.x - b.x, a.y - b.y };
}

static bool isRightAngle(const trukhanov::Point& a, const trukhanov::Point& b, const trukhanov::Point& c)
{
  trukhanov::Point ab = sub(b, a);
  trukhanov::Point bc = sub(c, b);
  return dot(ab, bc) == 0;
}

struct HasRightAngle
{
  const std::vector< trukhanov::Point >& points;
  size_t size;

  HasRightAngle(const std::vector< trukhanov::Point >& pts) : points(pts), size(pts.size()) {}

  bool operator()(size_t i) const
  {
    using namespace trukhanov;
    const Point& a = points[i];
    const Point& b = points[(i + 1) % size];
    const Point& c = points[(i + 2) % size];
    return isRightAngle(a, b, c);
  }
};

bool trukhanov::isRight(const Polygon& polygon)
{
  size_t n = polygon.points.size();
  if (n < 3)
  {
    return false;
  }

  std::vector< size_t > indices(n);
  std::iota(indices.begin(), indices.end(), 0);

  return std::any_of(indices.begin(), indices.end(), HasRightAngle(polygon.points));

  return false;
}

bool trukhanov::operator<(const Point& lhs, const Point& rhs)
{
  return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
}

bool trukhanov::operator==(const Point& lhs, const Point& rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}
