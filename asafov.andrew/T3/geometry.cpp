#include "geometry.h"
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <cmath>

namespace
{
  double roundArea(double area)
  {
    double intPart;
    double frac = modf(area, &intPart);
    if (frac >= 0.5)
    {
      return intPart + 1.0;
    }
    if (frac <= -0.5)
    {
      return intPart - 1.0;
    }
    return intPart;
  }
}

namespace asafov
{
  bool Point::operator==(const Point& other) const
  {
    return x == other.x && y == other.y;
  }

  bool Point::operator!=(const Point& other) const
  {
    return !(*this == other);
  }

  double computeArea(const Polygon& poly)
  {
    if (poly.points.size() < 3) return 0.0;

    double sum = 0.0;
    const auto& pts = poly.points;

    for (size_t i = 0; i < pts.size(); ++i)
    {
      const Point& p1 = pts[i];
      const Point& p2 = pts[(i + 1) % pts.size()];
      sum += (p1.x * p2.y) - (p2.x * p1.y);
    }

    return roundArea(std::abs(sum) / 2.0);
  }

  bool edgesIntersect(const Point& a1, const Point& a2, const Point& b1, const Point& b2)
  {
    auto cross = [](const Point& p, const Point& q)
    {
      return p.x * q.y - p.y * q.x;
    };

    Point r = {a2.x - a1.x, a2.y - a1.y};
    Point s = {b2.x - b1.x, b2.y - b1.y};
    Point qp = {b1.x - a1.x, b1.y - a1.y};

    double rxs = cross(r, s);
    if (std::abs(rxs) < 1e-10) return false;

    double t = cross(qp, s) / rxs;
    double u = cross(qp, r) / rxs;

    return (t >= 0 && t <= 1) && (u >= 0 && u <= 1);
  }

  bool doPolygonsIntersect(const Polygon& a, const Polygon& b)
  {
    auto check_edge = [](const Point& p1, const Point& p2, const Polygon& poly)
    {
      for (size_t i = 0; i < poly.points.size(); ++i)
      {
        const Point& p3 = poly.points[i];
        const Point& p4 = poly.points[(i + 1) % poly.points.size()];
        if (edgesIntersect(p1, p2, p3, p4))
        {
          return true;
        }
      }
      return false;
    };

    for (size_t i = 0; i < a.points.size(); ++i)
    {
      const Point& p1 = a.points[i];
      const Point& p2 = a.points[(i + 1) % a.points.size()];
      if (check_edge(p1, p2, b))
      {
        return true;
      }
    }

    return false;
  }

  bool arePolygonsSame(const Polygon& a, const Polygon& b)
  {
    if (a.points.size() != b.points.size()) return false;

    auto rotated_equal = [](const std::vector< Point >& v1, const std::vector< Point >& v2)
    {
      for (size_t offset = 0; offset < v1.size(); ++offset)
      {
        bool match = true;
        for (size_t i = 0; i < v1.size(); ++i)
        {
          if (v1[(i + offset) % v1.size()] != v2[i])
          {
            match = false;
            break;
          }
        }
        if (match) return true;
      }
      return false;
    };

    return rotated_equal(a.points, b.points) || rotated_equal(b.points, a.points);
  }
}
