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

  return std::equal(points.begin(), points.end(), other.points.begin());
}

double asafov::computeArea(const Polygon& poly)
{
  if (poly.points.size() < 3)
  {
    return 0.0;
  }

  struct PolygonAreaCalculator
  {
    double operator()(size_t i)
    {
      const Point& p1 = poly.points[i];
      const Point& p2 = poly.points[(i + 1) % poly.points.size()];
      area += crossProduct(Point{ 0, 0 }, p1, p2);
      return area;
    }

    const Polygon& poly;
    double area = 0.0;
  };

  PolygonAreaCalculator calculator{ poly };
  std::vector< size_t > indices(poly.points.size());
  std::iota(indices.begin(), indices.end(), 0);

  std::vector<double> dummy(poly.points.size());
  std::transform(indices.begin(), indices.end(), dummy.begin(), std::ref(calculator));

  return std::abs(calculator.area) / 2.0;
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

  struct EdgeCalculator
  {
    double operator()(size_t i) const
    {
      size_t j = (i + 1) % poly.points.size();
      double dx = poly.points[j].x - poly.points[i].x;
      double dy = poly.points[j].y - poly.points[i].y;
      return std::sqrt(dx * dx + dy * dy);
    }

    const Polygon& poly;
  };

  struct ScaleTransformer
  {
    double operator()(double length) const
    {
      return length * scale;
    }

    double scale;
  };

  struct ShiftChecker
  {
    bool operator()() const
    {
      for (size_t i = 0; i < n; ++i)
      {
        size_t index = reverse ? (shift + n - i) % n : (i + shift) % n;
        if (std::abs(A[i] - B[index]) > 1e-6) return false;
      }
      return true;
    }

    const std::vector<double>& A;
    const std::vector<double>& B;
    size_t n;
    size_t shift;
    bool reverse;
  };

  EdgeCalculator calcA{ a }, calcB{ b };
  std::vector< size_t > indices(n);
  std::iota(indices.begin(), indices.end(), 0);

  std::vector< double > A(n), B(n);
  std::transform(indices.begin(), indices.end(), A.begin(), std::ref(calcA));
  std::transform(indices.begin(), indices.end(), B.begin(), std::ref(calcB));

  double scale = A[0] / B[0];
  ScaleTransformer scaler{ scale };
  std::transform(B.begin(), B.end(), B.begin(), std::ref(scaler));

  std::vector< size_t > shifts(n);
  std::iota(shifts.begin(), shifts.end(), 0);

  struct AnyShiftChecker
  {
    bool operator()(size_t shift) const
    {
      ShiftChecker forward{ A, B, n, shift, false };
      if (forward()) return true;

      ShiftChecker reverse{ A, B, n, shift, true };
      return reverse();
    }

    const std::vector<double>& A;
    const std::vector<double>& B;
    size_t n;
  };

  AnyShiftChecker shiftChecker{ A, B, n };
  return std::any_of(shifts.begin(), shifts.end(), std::ref(shiftChecker));
}

bool asafov::edgesIntersect(const Point& a1, const Point& a2, const Point& b1, const Point& b2)
{
  struct OrientationFunctor
  {
    int operator()(const Point& p, const Point& q, const Point& r) const\
    {
      int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
      if (val == 0)
      {
        return 0;
      }
      return (val > 0) ? 1 : 2;
    }
  };

  struct SegmentFunctor
  {
    bool operator()(const Point& p, const Point& q, const Point& r) const
    {
      using std::min;
      using std::max;

      if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
      {
        return true;
      }
      return false;
    }
  };

  OrientationFunctor orientation;
  SegmentFunctor segf;

  int o1 = orientation(a1, a2, b1);
  int o2 = orientation(a1, a2, b2);
  int o3 = orientation(b1, b2, a1);
  int o4 = orientation(b1, b2, a2);

  if (o1 != o2 && o3 != o4)
  {
    return true;
  }

  if (o1 == 0 && segf(a1, b1, a2))
  {
    return true;
  }
  if (o2 == 0 && segf(a1, b2, a2))
  {
    return true;
  }
  if (o3 == 0 && segf(b1, a1, b2))
  {
    return true;
  }
  if (o4 == 0 && segf(b1, a2, b2))
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
      EdgeFunctor edgeChecker{ a, b, i };
      std::vector< size_t > indicesB(b.points.size());
      std::iota(indicesB.begin(), indicesB.end(), 0);

      return std::any_of(indicesB.begin(), indicesB.end(), std::ref(edgeChecker));
    }

    const Polygon& a;
    const Polygon& b;
  };

  PolygonFunctor allEdgesChecker{a, b};
  std::vector< size_t > indicesA(a.points.size());
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
      bool halfCondition = (point.x < (p2.x - p1.x) * (point.y - p1.y) / static_cast< double >(p2.y - p1.y) + p1.x);
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
