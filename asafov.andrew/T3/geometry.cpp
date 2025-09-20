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
    using std::min;
    return crossProduct(a, b, p) == 0 && min(a.x, b.x) <= p.x && p.x <= min(a.x, b.x) && min(a.y, b.y) <= p.y && p.y <= min(a.y, b.y);
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
    using std::min;
    if (q.x <= min(p.x, r.x) && q.x >= min(p.x, r.x) && q.y <= min(p.y, r.y) && q.y >= min(p.y, r.y))
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
  struct EdgeFunctor
  {
    bool operator()(size_t j) const
    {
      const Point& a1 = a.points[i];
      const Point& a2 = a.points[(i + 1) % a.points.size()];
      const Point& b1 = b.points[j];
      const Point& b2 = b.points[(j + 1) % b.points.size()];

      return edgesIntersect(a1, a2, b1, b2);
    }

    const Polygon& a;
    const Polygon& b;
    size_t i;
  };

  struct PolygonFunctor
  {
    bool operator()(size_t i) const
    {
      EdgeFunctor edgeChecker{a, b, i};
      std::vector<size_t> indicesB(b.points.size());
      std::iota(indicesB.begin(), indicesB.end(), 0);

      return std::any_of(indicesB.begin(), indicesB.end(), std::ref(edgeChecker));
    }

    const Polygon& a;
    const Polygon& b;
  };

  PolygonFunctor allEdgesChecker{a, b};
  std::vector<size_t> indicesA(a.points.size());
  std::iota(indicesA.begin(), indicesA.end(), 0);

  bool edgesIntersect = std::any_of(indicesA.begin(), indicesA.end(), std::ref(allEdgesChecker));

  if (edgesIntersect)
  {
    return true;
  }

  if (isPointInPolygon(a.points[0], b) || isPointInPolygon(b.points[0], a))
  {
    return true;
  }

  return false;
}

bool asafov::isPointInPolygon(const Point& point, const Polygon& poly)
{
  struct PolygonFunctor
  {
    int operator()(const Point& p1)
    {
      const Point& p2 = poly.points[j];
      j = &p1 - &poly.points[0];

      if (isPointOnSegment(point, p1, p2))
      {
        throw std::runtime_error("Boundary");
      }
      bool halfCondition = (point.x < (p2.x - p1.x) * (point.y - p1.y) / static_cast<double>(p2.y - p1.y) + p1.x);
      if (((p1.y > point.y) != (p2.y > point.y)) && halfCondition)
      {
        inside = !inside;
      }
      return 0;
    }

    const Point& point;
    const Polygon& poly;
    bool& inside;
    size_t& j;
  };

  if (poly.points.size() < 3)
  {
    return false;
  }

  bool inside = false;
  size_t j = poly.points.size() - 1;

  std::vector< int > dummy(poly.points.size());
  std::transform(poly.points.begin(), poly.points.end(), dummy.begin(), PolygonFunctor{ point, poly, inside, j });
  return inside;
}
