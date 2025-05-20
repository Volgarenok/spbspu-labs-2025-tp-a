#include "functors.hpp"

bool filonova::IsEven::operator()(const Polygon &polygon) const
{
  return polygon.points.size() % 2 == 0;
}

bool filonova::IsOdd::operator()(const Polygon &polygon) const
{
  return polygon.points.size() % 2 != 0;
}

double filonova::GetPolygonArea::operator()(const Polygon &polygon) const
{
  return getArea(polygon);
}

size_t filonova::GetPolygonVertexCount::operator()(const Polygon &polygon) const
{
  return polygon.points.size();
}

bool filonova::HasVertexCount::operator()(const Polygon &polygon) const
{
  return polygon.points.size() == count;
}

bool filonova::IntersectsWith::operator()(const Polygon &other) const
{
  auto left = std::minmax_element(polygon.points.begin(), polygon.points.end());
  auto right = std::minmax_element(other.points.begin(), other.points.end());
  return !((*left.second < *right.first) || (*right.second < *left.first));
}

bool filonova::IsRectangle::operator()(const Polygon &polygon) const
{
  const auto &points = polygon.points;
  constexpr size_t RECTANGLE_SIDES = 4;

  if (points.size() != RECTANGLE_SIDES)
  {
    return false;
  }

  static constexpr double EPS = 1e-9;
  double diag1 = getDistanceSquared(points[0], points[2]);
  double diag2 = getDistanceSquared(points[1], points[3]);
  if (std::abs(diag1 - diag2) > EPS)
  {
    return false;
  }

  for (size_t i = 0; i < RECTANGLE_SIDES; ++i)
  {
    if (dot(points[i], points[(i + 1) % RECTANGLE_SIDES], points[(i + 2) % RECTANGLE_SIDES]) > EPS)
    {
      return false;
    }
  }

  return true;
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
