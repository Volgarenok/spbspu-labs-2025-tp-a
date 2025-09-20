#include "geometry.hpp"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iterator>
#include <limits>
#include <ios>
#include <delimiter.hpp>

namespace geom
{
  struct PointReader
  {
    std::istream& in;
    Point operator()() const
    {
      Point p;
      if (!(in >> p))
      {
        throw std::ios_base::failure("Error reading point");
      }
      return p;
    }
  };

  bool operator==(const Point& p1, const Point& p2)
  {
    return p1.x == p2.x && p1.y == p2.y;
  }

  bool operator>(const Point& p, const Point& other)
  {
    return (p.x > other.x && p.y > other.y);
  }

  bool operator<(const Point& p, const Point& other)
  {
    return (p.x < other.x && p.y < other.y);
  }

  std::istream& operator>>(std::istream& in, Point& p)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    using del = io::DelimiterIO;
    return in >> del{ '(' } >> p.x >> del{ ';' } >> p.y >> del{ ')' };
  }

  std::istream& operator>>(std::istream& in, Polygon& poly)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    size_t count;
    if (!(in >> count) || count < 3)
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    std::vector< Point > pts;
    pts.reserve(count);
    std::generate_n(std::back_inserter(pts), count, PointReader{ in });

    if (!in)
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    poly.points = std::move(pts);
    return in;
  }

  double getDist(const Point& a, const Point& b)
  {
    return std::sqrt(std::pow((a.x - b.x), 2) + std::pow((a.y - b.y), 2));
  }

  double getAreaOfTrg(const Polygon& poly)
  {
    double side1 = getDist(poly.points[0], poly.points[1]);
    double side2 = getDist(poly.points[0], poly.points[2]);
    double side3 = getDist(poly.points[2], poly.points[1]);
    double p = (side1 + side2 + side3) / 2;
    return std::sqrt(p * (p - side1) * (p - side2) * (p - side3));
  }

  Polygon createTrg(size_t i, const std::vector< Point >& points)
  {
    return Polygon{ std::vector< Point >{ points[0], points[i + 1], points[i + 2] } };
  }

  Polygon TrgGenerator::operator()()
  {
    return createTrg(i_++, points_);
  }

  std::vector < Polygon > polyToTrg(const Polygon& poly)
  {
    size_t size = poly.points.size() - 2;
    size_t ind = 0;
    std::vector< Polygon > triangles(size);
    std::generate(triangles.begin(), triangles.end(), TrgGenerator{ ind, poly.points });
    return triangles;
  }

  double getPolygonArea(const Polygon& poly)
  {
    std::vector< double > areas;
    std::vector< Polygon > triangles = polyToTrg(poly);
    std::transform(triangles.begin(), triangles.end(), std::back_inserter(areas), getAreaOfTrg);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }
}
