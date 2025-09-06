#include "geometry.h"
#include <cmath>
#include <algorithm>
#include <numeric>
#include <functional>

namespace
{
  double crossProduct(const asafov::Point& a, const asafov::Point& b, const asafov::Point& c)
  {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
  }

  bool isPointOnSegment(const asafov::Point& p, const asafov::Point& a, const asafov::Point& b)
  {
    return crossProduct(a, b, p) == 0 &&
      std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x) &&
      std::min(a.y, b.y) <= p.y && p.y <= std::max(a.y, b.y);
  }
}

bool asafov::Point::operator==(const Point& other) const
{
  return x == other.x && y == other.y;
}

bool asafov::Point::operator!=(const Point& other) const
{
  return !(*this == other);
}

bool asafov::Polygon::operator==(const Polygon& other) const
{
  if (points.size() != other.points.size())
  {
    return false;
  }

  for (size_t i = 0; i < points.size(); ++i)
  {
    if (points[i] != other.points[i])
    {
      return false;
    }
  }

  return true;
}

double asafov::computeArea(const Polygon& poly)
{
  if (poly.points.size() < 3)
  {
    return 0.0;
  }

  double area = 0.0;
  const size_t n = poly.points.size();

  for (size_t i = 0; i < n; ++i)
  {
    const Point& p1 = poly.points[i];
    const Point& p2 = poly.points[(i + 1) % n];
    area += (p1.x * p2.y) - (p2.x * p1.y);
  }

  return std::abs(area) / 2.0;
}

bool asafov::arePolygonsSame(const Polygon& a, const Polygon& b)
{
  if (a.points.size() != b.points.size())
  {
    return false;
  }
  const size_t n = a.points.size();
  if (n < 3)
  {
    return false;
  }

  auto getEdgeLengths = [&](const Polygon& poly)
  {
    std::vector<double> lengths;
    for (size_t i = 0; i < n; ++i)
    {
      size_t j = (i + 1) % n;
      double dx = poly.points[j].x - poly.points[i].x;
      double dy = poly.points[j].y - poly.points[i].y;
      lengths.push_back(std::sqrt(dx*dx + dy*dy));
    }
    return lengths;
  };

  auto A = getEdgeLengths(a);
  auto B = getEdgeLengths(b);

  double scale = A[0] / B[0];
  for (double &l : B)
  {
    l *= scale;
  }

  for (size_t shift = 0; shift < n; ++shift)
  {
    bool ok = true;
    for (size_t i = 0; i < n; ++i)
    {
      if (std::abs(A[i] - B[(i+shift)%n]) > 1e-6)
      {
        ok = false;
        break;
      }
    }
    if (ok)
    {
      return true;
    }

    ok = true;
    for (size_t i = 0; i < n; ++i)
    {
      if (std::abs(A[i] - B[(shift+n-i)%n]) > 1e-6)
      {
        ok = false;
        break;
      }
    }
    if (ok)
    {
      return true;
    }
  }

  return false;
}

bool asafov::edgesIntersect(const Point& a1, const Point& a2, const Point& b1, const Point& b2)
{
  auto orientation = [](const Point& p, const Point& q, const Point& r) -> int
  {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0)
    {
      return 0;
    }
    return (val > 0) ? 1 : 2;
  };

  auto onSegment = [](const Point& p, const Point& q, const Point& r) -> bool
  {
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
    {
      return true;
    }
    return false;
  };

  int o1 = orientation(a1, a2, b1);
  int o2 = orientation(a1, a2, b2);
  int o3 = orientation(b1, b2, a1);
  int o4 = orientation(b1, b2, a2);

  if (o1 != o2 && o3 != o4)
  {
    return true;
  }

  if (o1 == 0 && onSegment(a1, b1, a2))
  {
    return true;
  }
  if (o2 == 0 && onSegment(a1, b2, a2))
  {
    return true;
  }
  if (o3 == 0 && onSegment(b1, a1, b2))
  {
    return true;
  }
  if (o4 == 0 && onSegment(b1, a2, b2))
  {
    return true;
  }

  return false;
}

bool asafov::doPolygonsIntersect(const Polygon& a, const Polygon& b)
{
  const size_t n = a.points.size();
  const size_t m = b.points.size();

  for (size_t i = 0; i < n; ++i)
  {
    const Point& a1 = a.points[i];
    const Point& a2 = a.points[(i + 1) % n];

    for (size_t j = 0; j < m; ++j)
    {
      const Point& b1 = b.points[j];
      const Point& b2 = b.points[(j + 1) % m];

      if (edgesIntersect(a1, a2, b1, b2))
      {
        return true;
      }
    }
  }

  if (isPointInPolygon(a.points[0], b) || isPointInPolygon(b.points[0], a))
  {
    return true;
  }

  return false;
}

bool asafov::isPointInPolygon(const Point& point, const Polygon& poly)
{
  if (poly.points.size() < 3) return false;

  bool inside = false;
  const size_t n = poly.points.size();

  for (size_t i = 0, j = n - 1; i < n; j = i++)
  {
    const Point& p1 = poly.points[i];
    const Point& p2 = poly.points[j];

    if (isPointOnSegment(point, p1, p2))
    {
      return true;
    }

    if (((p1.y > point.y) != (p2.y > point.y)) &&
        (point.x < (p2.x - p1.x) * (point.y - p1.y) / static_cast<double>(p2.y - p1.y) + p1.x))
    {
      inside = !inside;
    }
  }

  return inside;
}

bool asafov::isPolygonConvex(const Polygon& poly)
{
  if (poly.points.size() < 3)
  {
    return false;
  }

  bool sign = false;
  const size_t n = poly.points.size();

  for (size_t i = 0; i < n; ++i)
  {
    const Point& a = poly.points[i];
    const Point& b = poly.points[(i + 1) % n];
    const Point& c = poly.points[(i + 2) % n];

    double cross = crossProduct(a, b, c);

    if (i == 0)
    {
      sign = cross > 0;
    }
    else if ((cross > 0) != sign)
    {
      return false;
    }
  }

  return true;
}

bool asafov::isPolygonRectangle(const Polygon& poly)
{
  if (poly.points.size() != 4 || !isPolygonConvex(poly))
  {
    return false;
  }

  const std::vector<Point>& p = poly.points;

  for (size_t i = 0; i < 4; ++i)
  {
    const Point& a = p[i];
    const Point& b = p[(i + 1) % 4];
    const Point& c = p[(i + 2) % 4];

    int dx1 = b.x - a.x;
    int dy1 = b.y - a.y;
    int dx2 = c.x - b.x;
    int dy2 = c.y - b.y;

    if (dx1 * dx2 + dy1 * dy2 != 0)
    {
      return false;
    }
  }

  return true;
}

bool asafov::hasRightAngle(const Polygon& poly)
{
  const size_t n = poly.points.size();

  for (size_t i = 0; i < n; ++i)
  {
    const Point& a = poly.points[i];
    const Point& b = poly.points[(i + 1) % n];
    const Point& c = poly.points[(i + 2) % n];

    int dx1 = b.x - a.x;
    int dy1 = b.y - a.y;
    int dx2 = c.x - b.x;
    int dy2 = c.y - b.y;

    if (dx1 * dx2 + dy1 * dy2 == 0)
    {
      return true;
    }
  }

  return false;
}

asafov::Polygon asafov::getBoundingBox(const std::vector<Polygon>& polygons)
{
  if (polygons.empty())
  {
    return Polygon{};
  }

  int minX = polygons[0].points[0].x;
  int maxX = polygons[0].points[0].x;
  int minY = polygons[0].points[0].y;
  int maxY = polygons[0].points[0].y;

  for (const auto& poly : polygons)
  {
    for (const auto& point : poly.points)
    {
      minX = std::min(minX, point.x);
      maxX = std::max(maxX, point.x);
      minY = std::min(minY, point.y);
      maxY = std::max(maxY, point.y);
    }
  }

  return Polygon{ { Point{ minX, minY }, Point{ maxX, minY }, Point{ maxX, maxY }, Point{ minX, maxY } } };
}
