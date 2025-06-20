#include "geometry.h"
#include <numeric>
#include <algorithm>

bool asafov::operator==(const Point& a, const Point& b)
{
  return a.x == b.x && a.y == b.y;
}

bool asafov::edgesIntersect(const Point& a1, const Point& a2, const Point& b1, const Point& b2)
{
  auto cross = [](const Point& p, const Point& q)
  {
    return p.x * q.y - p.y * q.x;
  };
  Point r = {a2.x - a1.x, a2.y - a1.y}, s = {b2.x - b1.x, b2.y - b1.y}, qp = {b1.x - a1.x, b1.y - a1.y};
  double rxs = cross(r, s), qpxr = cross(qp, r);
  if (std::abs(rxs) < 1e-10) return false;
  double t = cross(qp, s) / rxs, u = qpxr / rxs;
  return (t >= 0 && t <= 1) && (u >= 0 && u <= 1);
}

bool asafov::doPolygonsIntersect(const Polygon& a, const Polygon& b)
{
  auto check_edges = [](const Polygon& p1, const Polygon& p2)
  {
    auto edge_intersect = [&p2](const Point& pt1)
    {
      const Point& pt2 = *(&pt1 + 1);
      return std::any_of(p2.points.begin(), p2.points.end() - 1,
                         [&pt1, &pt2](const Point& pt3)
                         {
                           const Point& pt4 = *(&pt3 + 1);
                           return edgesIntersect(pt1, pt2, pt3, pt4);
                         });
    };
    return std::any_of(p1.points.begin(), p1.points.end() - 1, edge_intersect);
  };
  return check_edges(a, b) || check_edges(b, a);
}

bool asafov::arePolygonsSame(const Polygon& a, const Polygon& b)
{
  if (a.points.size() != b.points.size()) return false;
  return std::search(a.points.begin(), a.points.end(), b.points.begin(), b.points.end()) != a.points.end() ||
    std::search(b.points.begin(), b.points.end(), a.points.begin(), a.points.end()) != b.points.end();
}

double asafov::computeArea(const Polygon& poly)
{
  auto area_op = [](double sum, const Point& p1, const Point& p2)
  {
    return sum + (p1.x * p2.y) - (p2.x * p1.y);
  };

  double sum = 0.0;
  const auto& pts = poly.points;
  if (pts.size() > 1)
  {
    sum = std::inner_product(pts.begin(), pts.end() - 1, pts.begin() + 1, 0.0, std::plus< >(), area_op);
    sum += area_op(0.0, pts.back(), pts.front());
  }
  return std::abs(sum) / 2.0;
}
