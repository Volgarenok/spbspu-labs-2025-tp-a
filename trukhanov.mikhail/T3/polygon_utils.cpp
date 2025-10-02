#include "polygon_utils.hpp"
#include <algorithm>
#include <cmath>
#include <numeric>
#include "polygon.hpp"

trukhanov::isSize::isSize(size_t s) : size(s) {}

bool trukhanov::isSize::operator()(const trukhanov::Polygon& p) const
{
  return p.points.size() == size;
}

bool trukhanov::CompareByArea(const Polygon& lhs, const Polygon& rhs)
{
  double lhsArea = getArea(lhs);
  double rhsArea = getArea(rhs);
  return lhsArea < rhsArea;
}

bool trukhanov::LessArea::operator()(const Polygon& p) const
{
  return getArea(p) < area;
}

bool trukhanov::compareByVertexes(const Polygon& lhs, const Polygon& rhs)
{
  return lhs.points.size() < rhs.points.size();
}

bool trukhanov::HasDuplicates(const Polygon& p)
{
  if (p.points.size() < 2)
  {
    return false;
  }
  std::vector< Point > sortedPoints = p.points;
  std::sort(sortedPoints.begin(), sortedPoints.end());
  return std::adjacent_find(sortedPoints.begin(), sortedPoints.end()) != sortedPoints.end();
}

bool trukhanov::PolygonHasMinSize(const trukhanov::Polygon& p)
{
  return p.points.size() >= 3;
}

double trukhanov::VectorProduct(const Point& a, const Point& b, const Point& c)
{
  return 0.5 * std::abs(static_cast<double>((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y)));
}

trukhanov::Angle trukhanov::MakeAngles::operator()(std::size_t i) const
{
  std::size_t n = pts.size();
  return { pts[i], pts[(i + 1) % n], pts[(i + 2) % n] };
}

bool trukhanov::HasRightAngle::operator()(const Angle& ang) const
{
  int x1 = ang.a.x - ang.b.x;
  int y1 = ang.a.y - ang.b.y;
  int x2 = ang.c.x - ang.b.x;
  int y2 = ang.c.y - ang.b.y;
  return x1 * x2 + y1 * y2 == 0;
}

bool trukhanov::CommonVertex::operator()(size_t j) const
{
  std::vector< trukhanov::Point > combined = polygons[i].points;
  combined.insert(combined.end(), polygons[j].points.begin(), polygons[j].points.end());

  std::sort(combined.begin(), combined.end());
  return std::adjacent_find(combined.begin(), combined.end()) != combined.end();
}

bool trukhanov::CheckAllPairs::operator()(size_t n) const
{
  if (n >= polygons.size() - 1)
  {
    return false;
  }

  std::vector<size_t> nextIndex(polygons.size() - n - 1);
  std::iota(nextIndex.begin(), nextIndex.end(), n + 1);

  bool found = std::any_of(nextIndex.begin(), nextIndex.end(), CommonVertex(polygons, n));
  if (found)
  {
    return true;
  }

  return (*this)(n + 1);
}

bool trukhanov::hasCommonPoint(const std::vector< trukhanov::Polygon >& polygons)
{
  if (polygons.size() < 2)
  {
    return false;
  }
  return trukhanov::CheckAllPairs(polygons)(0);
}

std::pair< trukhanov::Point, trukhanov::Point > trukhanov::normalizeSide(const std::pair< trukhanov::Point, trukhanov::Point >& side)
{
  if (side.first < side.second)
  {
    return side;
  }
  else
  {
    return { side.second, side.first };
  }
}

std::vector< std::pair< trukhanov::Point, trukhanov::Point > > trukhanov::CollectSides::operator()() const
{
  std::vector< std::pair< Point, Point > > Sides(pts.size());
  std::transform(pts.begin(), pts.end() - 1, pts.begin() + 1, Sides.begin(), makeSidePair);
  Sides.back() = { pts.back(), pts.front() };
  std::transform(Sides.begin(), Sides.end(), Sides.begin(), normalizeSide);
  std::sort(Sides.begin(), Sides.end());
  return Sides;
}

std::pair< trukhanov::Point, trukhanov::Point> trukhanov::makeSidePair(const trukhanov::Point& a, const trukhanov::Point& b)
{
  return { a, b };
}

bool trukhanov::CommonSide::operator()(size_t j) const
{
  std::vector< std::pair< Point, Point > > Sides1 = CollectSides{ polygons[i].points }();
  std::vector< std::pair< Point, Point > > Sides2 = CollectSides{ polygons[j].points }();

  std::vector< std::pair< Point, Point > > combined(Sides1.size() + Sides2.size());
  std::copy(Sides1.begin(), Sides1.end(), combined.begin());
  std::copy(Sides2.begin(), Sides2.end(), combined.begin() + Sides1.size());

  std::sort(combined.begin(), combined.end());
  return std::adjacent_find(combined.begin(), combined.end()) != combined.end();
}

bool trukhanov::CheckAllSides::operator()(size_t n) const
{
  if (n >= polygons.size() - 1)
  {
    return false;
  }

  std::vector< size_t > nextIndex(polygons.size() - n - 1);
  std::iota(nextIndex.begin(), nextIndex.end(), n + 1);

  bool found = std::any_of(nextIndex.begin(), nextIndex.end(), CommonSide(polygons, n));
  if (found)
  {
    return true;
  }
  return (*this)(n + 1);
}
