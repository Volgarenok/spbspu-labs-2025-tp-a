#include "polygon.hpp"
#include "input_struct.hpp"
#include <algorithm>
#include <cmath>
#include <istream>

namespace shiryaeva
{
  std::istream& operator>>(std::istream& in, Point& point)
  {
    Point temp{0, 0};
    in >> DelimiterIO{'('} >> temp.x >> DelimiterIO{';'} >> temp.y >> DelimiterIO{')'};
    if (in)
    {
      point = temp;
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, Polygon& polygon)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    size_t vertexCount = 0;
    in >> vertexCount;

    if (!in || vertexCount < MIN_VERTEX_COUNT)
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    std::vector< Point > points;
    points.reserve(vertexCount);
    for (size_t i = 0; i < vertexCount; ++i)
    {
      Point p;
      if (!(in >> p))
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      if (std::find(points.begin(), points.end(), p) != points.end())
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      points.push_back(p);
    }

    if (points.size() != vertexCount)
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    polygon.points = std::move(points);
    return in;
  }

  double getArea(const Polygon& polygon)
  {
    double area = 0.0;
    const auto& points = polygon.points;
    size_t n = points.size();

    if (n < MIN_VERTEX_COUNT)
    {
      return 0.0;
    }

    for (size_t i = 0; i < n; ++i)
    {
      const auto& p1 = points[i];
      const auto& p2 = points[(i + 1) % n];
      area += (p1.x * p2.y - p2.x * p1.y);
    }

    return std::abs(area) / 2.0;
  }
}
