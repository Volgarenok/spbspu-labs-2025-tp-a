#include "geometry.hpp"
#include <delimiterIO.hpp>
#include <numeric>
#include <algorithm>
#include <functional>
#include <iterator>
#include <cmath>

namespace cherkasov
{
  std::istream& operator>>(std::istream& in, Point& p)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    Point temp{ 0, 0 };
    in >> DelimiterIO{ '(' };
    in >> temp.x;
    in >> DelimiterIO{ ';' };
    in >> temp.y;
    in >> DelimiterIO{ ')' };
    if (in)
    {
      p = temp;
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, Polygon& polygon)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    size_t k = 0;
    in >> k;
    if (!in)
    {
      return in;
    }
    if (k < 3)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    std::vector< Point > points(k);
    for (size_t i = 0; i < k; ++i)
    {
      if (!(in >> points[i]))
      {
        in.setstate(std::ios::failbit);
        return in;
      }
    }
    polygon.points = points;
    return in;
  }

  bool Point::operator==(const Point& other) const
  {
    return x == other.x && y == other.y;
  }

  bool Point::operator<(const Point& other) const
  {
    return std::tie(x, y) < std::tie(other.x, other.y);
  }

  bool Polygon::operator==(const Polygon& other) const
  {
    return points == other.points;
  }

  bool VertexesCmp::operator()(const Polygon& polygon) const
  {
    return polygon.points.size() == k;
  }

  bool IntersectCmp::operator()(const Polygon& other) const
  {
    if (polygon.points.empty() || other.points.empty())
    {
      return false;
    }
    int minx1 = polygon.points.front().x, miny1 = polygon.points.front().y, maxx1 = minx1, maxy1 = miny1;
    for (size_t i = 0; i < polygon.points.size(); ++i)
    {
      minx1 = std::min(minx1, polygon.points[i].x);
      miny1 = std::min(miny1, polygon.points[i].y);
      maxx1 = std::max(maxx1, polygon.points[i].x);
      maxy1 = std::max(maxy1, polygon.points[i].y);
    }
    int minx2 = other.points.front().x, miny2 = other.points.front().y, maxx2 = minx2, maxy2 = miny2;
    for (size_t i = 0; i < other.points.size(); ++i)
    {
      minx2 = std::min(minx2, other.points[i].x);
      miny2 = std::min(miny2, other.points[i].y);
      maxx2 = std::max(maxx2, other.points[i].x);
      maxy2 = std::max(maxy2, other.points[i].y);
    }
    bool noOverlap = (maxx1 < minx2) || (maxx2 < minx1) || (maxy1 < miny2) || (maxy2 < miny1);
    return !noOverlap;
  }

  bool RightAngleCmp::operator()(const Polygon& poly) const
  {
    if (poly.points.size() < 3)
    {
      return false;
    }
    const std::vector< Point >& pts = poly.points;
    size_t n = pts.size();
    for (size_t i = 0; i < n; ++i)
    {
      const Point& a = pts[(i + n - 1) % n];
      const Point& b = pts[i];
      const Point& c = pts[(i + 1) % n];

      int abx = b.x - a.x;
      int aby = b.y - a.y;
      int bcx = c.x - b.x;
      int bcy = c.y - b.y;

      int dot = abx * bcx + aby * bcy;
      if (dot == 0)
      {
        return true;
      }
    }
    return false;
  }

  bool isEven(const Polygon &polygon)
  {
    return polygon.points.size() % 2 == 0;
  }

  bool isOdd(const Polygon &polygon)
  {
    return !isEven(polygon);
  }

  double getArea(const Polygon& polygon)
  {
    const std::vector< Point >& p = polygon.points;
    if (p.empty())
    {
      return 0.0;
    }
    double area = 0.0;
    for (size_t i = 0, j = p.size() - 1; i < p.size(); j = i++)
    {
      area += double(p[j].x) * double(p[i].y) - double(p[i].x) * double(p[j].y);
    }
    return std::abs(area) / 2.0;
  }

  double areaMean(const std::vector< Polygon >& polygons)
  {
    if (polygons.empty())
    {
      return 0.0;
    }
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), getArea);
    double res = std::accumulate(areas.begin(), areas.end(), 0.0);
    return res / polygons.size();
  }

  bool maxArea(const Polygon& p1, const Polygon& p2)
  {
    return getArea(p1) < getArea(p2);
  }

  bool maxVertexes(const Polygon& p1, const Polygon& p2)
  {
    return p1.points.size() < p2.points.size();
  }

  size_t countEven(const std::vector< Polygon >& polygons)
  {
    return std::count_if(polygons.begin(), polygons.end(), isEven);
  }

  size_t countOdd(const std::vector< Polygon >& polygons)
  {
    return std::count_if(polygons.begin(), polygons.end(), isOdd);
  }

  size_t countVertexes(const std::vector< Polygon >& polygons, size_t vert)
  {
    VertexesCmp cmp{ vert };
    return std::count_if(polygons.begin(), polygons.end(), cmp);
  }

  bool isPointsEqual(const Point& p1, const Point& p2)
  {
    return p1.x == p2.x && p1.y == p2.y;
  }

  bool isPolygonsEqual(const Polygon& p1, const Polygon& p2)
  {
    const std::vector< Point >& points1 = p1.points;
    const std::vector< Point >& points2 = p2.points;
    if (points1.size() != points2.size())
    {
      return false;
    }
    for (size_t i = 0; i < points1.size(); ++i)
    {
      if (!isPointsEqual(points1[i], points2[i]))
      {
        return false;
      }
    }
    return true;
  }

  int crossProduct(const Point &p1, const Point &p2)
  {
    return p1.x * p2.y - p1.y * p2.x;
  }
}
