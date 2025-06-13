#include "functors.hpp"

filonova::TriangleAreaAccumulator::TriangleAreaAccumulator(const std::vector< Point > &pts): point(pts[0]), points(pts) {}

double filonova::TriangleAreaAccumulator::operator()(double sum, size_t i) const
{
  return sum + filonova::triangleArea(point, points[i], points[i + 1]);
}

filonova::HasVertexCount::HasVertexCount(size_t count): count_(count) {}

bool filonova::HasVertexCount::operator()(const Polygon &p) const
{
  return p.points.size() == count_;
}

bool filonova::CompareByArea::operator()(const Polygon &a, const Polygon &b) const
{
  return getArea(a) < getArea(b);
}

bool filonova::CompareByVertexes::operator()(const Polygon &a, const Polygon &b) const
{
  return a.points.size() < b.points.size();
}

filonova::IntersectsWith::IntersectsWith(const Polygon &polygon): polygon_(polygon) {}

bool filonova::IntersectsWith::operator()(const Polygon &other) const
{
  auto left = std::minmax_element(polygon_.points.begin(), polygon_.points.end());
  auto right = std::minmax_element(other.points.begin(), other.points.end());
  return !((*left.second < *right.first) || (*right.second < *left.first));
}

double filonova::IsRectangle::dot(const Point &a, const Point &b, const Point &c)
{
  double abx = b.x - a.x;
  double aby = b.y - a.y;
  double bcx = c.x - b.x;
  double bcy = c.y - b.y;
  return abx * bcx + aby * bcy;
}

double filonova::IsRectangle::getDistanceSquared(const Point &a, const Point &b)
{
  double dx = b.x - a.x;
  double dy = b.y - a.y;
  return dx * dx + dy * dy;
}

bool filonova::IsRectangle::operator()(const Polygon &polygon) const
{
  const auto &points = polygon.points;

  if (points.size() != RECTANGLE_SIDES)
  {
    return false;
  }

  double diag1 = getDistanceSquared(points[0], points[2]);
  double diag2 = getDistanceSquared(points[1], points[3]);

  if (std::abs(diag1 - diag2) > EPS)
  {
    return false;
  }

  std::vector< filonova::AngleCheckPoints > angles_to_check;
  angles_to_check.reserve(RECTANGLE_SIDES);

  angles_to_check.push_back(filonova::AngleCheckPoints{points[3], points[0], points[1]});
  angles_to_check.push_back(filonova::AngleCheckPoints{points[0], points[1], points[2]});
  angles_to_check.push_back(filonova::AngleCheckPoints{points[1], points[2], points[3]});
  angles_to_check.push_back(filonova::AngleCheckPoints{points[2], points[3], points[0]});

  return std::all_of(angles_to_check.begin(), angles_to_check.end(), filonova::IsRightAngle{});
}
