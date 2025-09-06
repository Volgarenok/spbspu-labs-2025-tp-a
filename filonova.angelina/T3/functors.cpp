#include "functors.hpp"

filonova::HasVertexCount::HasVertexCount(size_t count):
  count_(count)
{}

bool filonova::HasVertexCount::operator()(const Polygon &p) const
{
  return p.points.size() == count_;
}

bool filonova::CompareByArea::operator()(const Polygon &a, const Polygon &b) const
{
  return getPolygonArea(a) < getPolygonArea(b);
}

bool filonova::CompareByVertexes::operator()(const Polygon &a, const Polygon &b) const
{
  return a.points.size() < b.points.size();
}

bool filonova::ComparePointByX::operator()(const Point &a, const Point &b) const
{
  return a.x < b.x || (a.x == b.x && a.y < b.y);
}

bool filonova::ComparePointByY::operator()(const Point &a, const Point &b) const
{
  return a.y < b.y || (a.y == b.y && a.x < b.x);
}

filonova::IntersectsWith::IntersectsWith(const Polygon &polygon):
  polygon_(polygon)
{}

bool filonova::IntersectsWith::operator()(const Polygon &other) const
{
  int p1_minX = std::min_element(polygon_.points.cbegin(), polygon_.points.cend(), ComparePointByX{})->x;
  int p1_maxX = std::max_element(polygon_.points.cbegin(), polygon_.points.cend(), ComparePointByX{})->x;
  int p1_minY = std::min_element(polygon_.points.cbegin(), polygon_.points.cend(), ComparePointByY{})->y;
  int p1_maxY = std::max_element(polygon_.points.cbegin(), polygon_.points.cend(), ComparePointByY{})->y;

  int p2_minX = std::min_element(other.points.cbegin(), other.points.cend(), ComparePointByX{})->x;
  int p2_maxX = std::max_element(other.points.cbegin(), other.points.cend(), ComparePointByX{})->x;
  int p2_minY = std::min_element(other.points.cbegin(), other.points.cend(), ComparePointByY{})->y;
  int p2_maxY = std::max_element(other.points.cbegin(), other.points.cend(), ComparePointByY{})->y;

  bool x_overlap = (p1_minX <= p2_maxX) && (p2_minX <= p1_maxX);
  bool y_overlap = (p1_minY <= p2_maxY) && (p2_minY <= p1_maxY);

  return x_overlap && y_overlap;
}

bool filonova::IsRightAngle::operator()(const AngleCheckPoints &angle) const
{
  return IsRectangle::dot(angle.prev, angle.current, angle.next) <= EPS;
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

  std::vector< filonova::AngleCheckPoints > angles;
  angles.reserve(RECTANGLE_SIDES);

  angles.push_back({points[3], points[0], points[1]});
  angles.push_back({points[0], points[1], points[2]});
  angles.push_back({points[1], points[2], points[3]});
  angles.push_back({points[2], points[3], points[0]});

  return std::all_of(angles.begin(), angles.end(), IsRightAngle{});
}
