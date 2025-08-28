#include "polygon.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>

static const std::size_t RECT_VERTICES = 4;

Point amine::operator-(const Point& a, const Point& b)
{
  return Point(a.x - b.x, a.y - b.y);
}

int amine::dot(const Point& a, const Point& b)
{
  return a.x * b.x + a.y * b.y;
}

int amine::cross(const Point& a, const Point& b)
{
  return a.x * b.y - a.y * b.x;
}

struct CrossProduct
{
  long long operator()(const Point& a, const Point& b) const
  {
    return static_cast< long long >(a.x) * b.y - static_cast< long long >(a.y) * b.x;
  }
};

double amine::compute_area(const Polygon& poly)
{
  const auto& pts = poly.points;
  int n = pts.size();
  if (n < 3)
    return 0.0;
  long long area2 =
    std::inner_product(pts.begin(), pts.end() - 1, pts.begin() + 1, 0LL, std::plus< long long >(), CrossProduct());
  const Point& last = pts.back();
  const Point& first = pts.front();
  area2 += static_cast< long long >(last.x) * first.y - static_cast< long long >(last.y) * first.x;
  return std::abs(area2) / 2.0;
}

bool amine::is_rectangle(const Polygon& poly)
{
  if (poly.points.size() != RECT_VERTICES)
    return false;
  const auto& p = poly.points;
  Point v0 = p[1] - p[0];
  Point v1 = p[2] - p[1];
  Point v2 = p[3] - p[2];
  Point v3 = p[0] - p[3];
  if (dot(v0, v1) != 0 || dot(v1, v2) != 0 || dot(v2, v3) != 0 || dot(v3, v0) != 0)
  {
    return false;
  }
  long long len0 = static_cast< long long >(v0.x) * v0.x + static_cast< long long >(v0.y) * v0.y;
  long long len1 = static_cast< long long >(v1.x) * v1.x + static_cast< long long >(v1.y) * v1.y;
  long long len2 = static_cast< long long >(v2.x) * v2.x + static_cast< long long >(v2.y) * v2.y;
  long long len3 = static_cast< long long >(v3.x) * v3.x + static_cast< long long >(v3.y) * v3.y;
  return len0 == len2 && len1 == len3;
}

long long amine::orient(const Point& p, const Point& q, const Point& r)
{
  return static_cast< long long >(q.x - p.x) * (r.y - p.y) - static_cast< long long >(q.y - p.y) * (r.x - p.x);
}

bool amine::on_segment(const Point& p, const Point& q, const Point& r)
{
  return q.x >= std::min(p.x, r.x) && q.x <= std::max(p.x, r.x) && q.y >= std::min(p.y, r.y) &&
         q.y <= std::max(p.y, r.y);
}

bool amine::seg_intersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2)
{
  long long o1 = orient(p1, q1, p2);
  long long o2 = orient(p1, q1, q2);
  long long o3 = orient(p2, q2, p1);
  long long o4 = orient(p2, q2, q1);

  if (o1 * o2 < 0 && o3 * o4 < 0)
  {
    return true;
  }
  if (o1 == 0 && on_segment(p1, p2, q1))
  {
    return true;
  }
  if (o2 == 0 && on_segment(p1, q2, q1))
  {
    return true;
  }
  if (o3 == 0 && on_segment(p2, p1, q2))
  {
    return true;
  }
  if (o4 == 0 && on_segment(p2, q1, q2))
  {
    return true;
  }

  return false;
}

bool amine::point_in_polygon(const Point& pt, const Polygon& poly)
{
  bool inside = false;
  const auto& p = poly.points;
  int n = p.size();
  if (n < 3)
    return false;

  struct PointInPolygonAccumulator
  {
    const Point& pt;
    bool& inside;

    PointInPolygonAccumulator(const Point& p, bool& ins):
      pt(p),
      inside(ins)
    {}

    std::pair< Point, bool > operator()(std::pair< Point, bool > acc, const Point& current) const
    {
      const Point& prev = acc.first;
      bool cond = (current.y > pt.y) != (prev.y > pt.y);
      bool intersect =
        cond &&
        (pt.x < static_cast< long double >(prev.x - current.x) * (pt.y - current.y) / (prev.y - current.y) + current.x);
      if (intersect)
        inside = !inside;
      return std::make_pair(current, inside);
    }
  };

  std::accumulate(p.begin(), p.end(), std::make_pair(p.back(), false), PointInPolygonAccumulator(pt, inside));

  return inside;
}

struct SegmentIntersectChecker
{
  const Polygon& a;
  const Polygon& b;
  int na;
  int nb;

  SegmentIntersectChecker(const Polygon& poly1, const Polygon& poly2, int n1, int n2):
    a(poly1),
    b(poly2),
    na(n1),
    nb(n2)
  {}

  bool operator()(const Point& p1) const
  {
    int i = &p1 - &a.points[0];
    struct InnerChecker
    {
      const Polygon& a;
      const Polygon& b;
      int i;
      int na;
      int nb;

      InnerChecker(const Polygon& poly1, const Polygon& poly2, int idx, int n1, int n2):
        a(poly1),
        b(poly2),
        i(idx),
        na(n1),
        nb(n2)
      {}

      bool operator()(const Point& p2) const
      {
        int j = &p2 - &b.points[0];
        return amine::seg_intersect(a.points[i], a.points[(i + 1) % na], b.points[j], b.points[(j + 1) % nb]);
      }
    };
    return std::any_of(b.points.begin(), b.points.end(), InnerChecker(a, b, i, na, nb));
  }
};

bool amine::polygons_intersect(const Polygon& a, const Polygon& b)
{
  int na = a.points.size();
  int nb = b.points.size();

  bool segment_intersection = std::any_of(a.points.begin(), a.points.end(), SegmentIntersectChecker(a, b, na, nb));

  if (segment_intersection)
    return true;
  if (!a.points.empty() && !b.points.empty())
  {
    return point_in_polygon(a.points[0], b) || point_in_polygon(b.points[0], a);
  }
  return false;
}

struct PointParser
{
  std::istringstream& iss;

  explicit PointParser(std::istringstream& stream):
    iss(stream)
  {}

  Point operator()() const
  {
    Point pt;
    char c;
    if (!(iss >> c) || c != '(')
    {
      return Point{};
    }
    if (!(iss >> pt.x))
    {
      return Point{};
    }
    if (!(iss >> c) || c != ';')
    {
      return Point{};
    }
    if (!(iss >> pt.y))
    {
      return Point{};
    }
    if (!(iss >> c) || c != ')')
    {
      return Point{};
    }
    return pt;
  }
};

bool amine::parse_polygon(const std::string& str, Polygon& poly)
{
  std::istringstream iss(str);
  int n;
  if (!(iss >> n) || n <= 0)
    return false;

  poly.points.clear();
  poly.points.reserve(n);

  std::generate_n(std::back_inserter(poly.points), n, PointParser(iss));

  iss >> std::ws;
  return !iss.fail() && iss.eof() && poly.points.size() == static_cast< size_t >(n);
}
