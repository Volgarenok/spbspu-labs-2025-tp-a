#include "geometry.h"
#include <cmath>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iterator>

namespace
{
  double crossProduct(const asafov::Point& a, const asafov::Point& b, const asafov::Point& c)
  {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
  }

  bool isPointOnSegment(const asafov::Point& p, const asafov::Point& a, const asafov::Point& b)
  {
    using std::min;
    using std::max;
    return crossProduct(a, b, p) == 0 && min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) && min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
  }

  class ComputeProductFunctor
  {
  public:
    ComputeProductFunctor(const asafov::Polygon& poly, size_t n):
      poly(poly),
      n(n),
      index(0)
    {}

    double operator()(const asafov::Point& p1, const asafov::Point& p2)
    {
      const asafov::Point& next = poly.points[(index + 1) % n];
      double product = (p1.x * next.y) - (next.x * p1.y);
      ++index;
      return product;
    }

  private:
    const asafov::Polygon& poly;
    size_t n;
    mutable size_t index;
  };

  class ComputeAreaFunctor
  {
  public:
    double operator()(const asafov::Polygon& poly) const
    {
      if (poly.points.size() < 3)
      {
        return 0.0;
      }

      const size_t n = poly.points.size();
      std::vector<double> products(n);

      ComputeProductFunctor computeProduct(poly, n);
      std::transform(poly.points.begin(), poly.points.end(), poly.points.begin(), products.begin(), computeProduct);

      double area = std::accumulate(products.begin(), products.end(), 0.0);
      return std::abs(area) / 2.0;
    }
  };

  class ComputeEdgeLengthFunctor
  {
  public:
    ComputeEdgeLengthFunctor(const asafov::Polygon& poly, size_t n):
      poly(poly),
      n(n),
      index(0)
    {}

    double operator()(const asafov::Point& point)
    {
      size_t j = (index + 1) % n;
      double dx = poly.points[j].x - point.x;
      double dy = poly.points[j].y - point.y;
      ++index;
      return std::sqrt(dx*dx + dy*dy);
    }

  private:
    const asafov::Polygon& poly;
    size_t n;
    mutable size_t index;
  };

  class GetEdgeLengthsFunctor
  {
  public:
    std::vector<double> operator()(const asafov::Polygon& poly, size_t n) const
    {
      std::vector<double> lengths;
      lengths.reserve(n);

      ComputeEdgeLengthFunctor computeLength(poly, n);
      std::transform(poly.points.begin(), poly.points.end(), std::back_inserter(lengths), computeLength);

      return lengths;
    }
  };

  class CheckLengthMatchFunctor
  {
  public:
    CheckLengthMatchFunctor(const std::vector<double>& B, size_t n, size_t shift, bool reverse):
      B(B),
      n(n),
      shift(shift),
      reverse(reverse),
      i(0)
    {}

    bool operator()(double a_val)
    {
      size_t index = reverse ? (shift + n - i) % n : (i + shift) % n;
      bool match = std::abs(a_val - B[index]) <= 1e-6;
      ++i;
      return match;
    }

  private:
    const std::vector<double>& B;
    size_t n;
    size_t shift;
    bool reverse;
    mutable size_t i;
  };

  class CheckShiftFunctor
  {
  public:
    bool operator()(const std::vector<double>& A, const std::vector<double>& B, size_t n, size_t shift, bool reverse) const
    {
      CheckLengthMatchFunctor checkMatch(B, n, shift, reverse);
      return std::all_of(A.begin(), A.end(), checkMatch);
    }
  };

  class ScaleLengthFunctor
  {
  public:
    ScaleLengthFunctor(double scale):
      scale(scale)
    {}

    void operator()(double& l) const
    {
      l *= scale;
    }

  private:
    double scale;
  };

  class CheckShiftAnyFunctor
  {
  public:
    CheckShiftAnyFunctor(const std::vector<double>& A, const std::vector<double>& B, size_t n, CheckShiftFunctor& checkShift):
      A(A),
      B(B),
      n(n),
      checkShift(checkShift),
      shift(0)
    {}

    bool operator()(double)
    {
      bool result = checkShift(A, B, n, shift, false) || checkShift(A, B, n, shift, true);
      ++shift;
      return result;
    }

  private:
    const std::vector<double>& A;
    const std::vector<double>& B;
    size_t n;
    CheckShiftFunctor& checkShift;
    mutable size_t shift;
  };

  class ArePolygonsSameFunctor
  {
  public:
    bool operator()(const asafov::Polygon& a, const asafov::Polygon& b) const
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

      GetEdgeLengthsFunctor getLengths;
      auto A = getLengths(a, n);
      auto B = getLengths(b, n);

      double scale = A[0] / B[0];
      ScaleLengthFunctor scaleFunc(scale);
      std::for_each(B.begin(), B.end(), scaleFunc);

      CheckShiftFunctor checkShift;
      CheckShiftAnyFunctor checkAny(A, B, n, checkShift);

      return std::any_of(A.begin(), A.end(), checkAny);
    }
  };

  class OrientationFunctor
  {
  public:
    int operator()(const asafov::Point& p, const asafov::Point& q, const asafov::Point& r) const
    {
      int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
      if (val == 0)
      {
        return 0;
      }
      return (val > 0) ? 1 : 2;
    }
  };

  class OnSegmentFunctor
  {
  public:
    bool operator()(const asafov::Point& p, const asafov::Point& q, const asafov::Point& r) const
    {
      if (q.x <= std::min(p.x, r.x) && q.x >= std::max(p.x, r.x) && q.y <= std::min(p.y, r.y) && q.y >= std::max(p.y, r.y))
      {
        return true;
      }
      return false;
    }
  };

  class EdgesIntersectFunctor
  {
  public:
    bool operator()(const asafov::Point& a1, const asafov::Point& a2, const asafov::Point& b1, const asafov::Point& b2) const
    {
      OrientationFunctor orientation;
      OnSegmentFunctor onSegment;

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
  };

  class CheckEdgeIntersectFunctor
  {
    using asafov::Point;
  public:
    CheckEdgeIntersectFunctor(const asafov::Polygon& b, const EdgesIntersectFunctor& edgesIntersect, const Point& a1, const Point& a2):
      b(b),
      edgesIntersect(edgesIntersect),
      a1(a1),
      a2(a2),
      j(0)
    {}

    bool operator()(const asafov::Point& b1)
    {
      const asafov::Point& b2 = b.points[(j + 1) % b.points.size()];
      bool intersects = edgesIntersect(a1, a2, b1, b2);
      ++j;
      return intersects;
    }

  private:
    const asafov::Polygon& b;
    const EdgesIntersectFunctor& edgesIntersect;
    const asafov::Point& a1;
    const asafov::Point& a2;
    mutable size_t j;
  };

  class CheckPolygonIntersectFunctor
  {
  public:
    CheckPolygonIntersectFunctor(const asafov::Polygon& a, const asafov::Polygon& b, const EdgesIntersectFunctor& edgesIntersect):
      a(a),
      b(b),
      edgesIntersect(edgesIntersect),
      i(0)
    {}

    bool operator()(const asafov::Point& a1)
    {
      const asafov::Point& a2 = a.points[(i + 1) % a.points.size()];
      CheckEdgeIntersectFunctor checkEdge(b, edgesIntersect, a1, a2);
      bool found = std::any_of(b.points.begin(), b.points.end(), checkEdge);
      ++i;
      return found;
    }

  private:
    const asafov::Polygon& a;
    const asafov::Polygon& b;
    const EdgesIntersectFunctor& edgesIntersect;
    mutable size_t i;
  };

  class DoPolygonsIntersectFunctor
  {
  public:
    bool operator()(const asafov::Polygon& a, const asafov::Polygon& b) const
    {
      const size_t n = a.points.size();
      const size_t m = b.points.size();

      EdgesIntersectFunctor edgesIntersect;

      CheckPolygonIntersectFunctor checkIntersect(a, b, edgesIntersect);
      bool edgesIntersectResult = std::any_of(a.points.begin(), a.points.end(), checkIntersect);

      if (edgesIntersectResult)
      {
        return true;
      }

      if (asafov::isPointInPolygon(a.points[0], b) || asafov::isPointInPolygon(b.points[0], a))
      {
        return true;
      }

      return false;
    }
  };

  class CheckPointInPolygonFunctor
  {
  public:
    CheckPointInPolygonFunctor(const asafov::Point& point, const asafov::Polygon& poly):
      point(point),
      poly(poly),
      i(0)
    {}

    bool operator()(const asafov::Point& p1, const asafov::Point& p2)
    {
      if (isPointOnSegment(point, p1, p2))
      {
        return true;
      }

      bool halfCondition = (point.x < (p2.x - p1.x) * (point.y - p1.y) / static_cast< double >(p2.y - p1.y) + p1.x);
      bool condition = ((p1.y > point.y) != (p2.y > point.y)) && halfCondition;
      ++i;
      return condition;
    }

  private:
    const asafov::Point& point;
    const asafov::Polygon& poly;
    mutable size_t i;
  };

  class IsPointInPolygonFunctor
  {
  public:
    bool operator()(const asafov::Point& point, const asafov::Polygon& poly) const
    {
      if (poly.points.size() < 3)
      {
        return false;
      }

      const size_t n = poly.points.size();
      std::vector<bool> insideFlags(n);

      CheckPointInPolygonFunctor checkPoint(point, poly);
      std::transform(poly.points.begin(), poly.points.end(), poly.points.rbegin(), insideFlags.begin(), checkPoint);

      return std::count(insideFlags.begin(), insideFlags.end(), true) % 2 == 1;
    }
  };
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
  ComputeAreaFunctor computeArea;
  return computeArea(poly);
}

bool asafov::arePolygonsSame(const Polygon& a, const Polygon& b)
{
  ArePolygonsSameFunctor arePolygonsSame;
  return arePolygonsSame(a, b);
}

bool asafov::edgesIntersect(const Point& a1, const Point& a2, const Point& b1, const Point& b2)
{
  EdgesIntersectFunctor edgesIntersect;
  return edgesIntersect(a1, a2, b1, b2);
}

bool asafov::doPolygonsIntersect(const Polygon& a, const Polygon& b)
{
  DoPolygonsIntersectFunctor doPolygonsIntersect;
  return doPolygonsIntersect(a, b);
}

bool asafov::isPointInPolygon(const Point& point, const Polygon& poly)
{
  IsPointInPolygonFunctor isPointInPolygon;
  return isPointInPolygon(point, poly);
}
