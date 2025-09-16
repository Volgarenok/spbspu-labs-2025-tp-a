#include <algorithm>
#include <iterator>
#include <cmath>
#include "streamGuard.hpp"
#include "polygon.hpp"

namespace ivanova
{
  bool Point::operator==(const Point& other) const
  {
    return x == other.x && y == other.y;
  }

  bool Polygon::operator==(const Polygon& other) const
  {
    return points == other.points;
  }

  std::istream& operator>>(std::istream& in, Point& point)
  {
    std::istream::sentry sent(in);
    if (!sent) return in;

    StreamGuard guard(in);
    char c1, c2, c3;
    if (!(in >> c1 >> point.x >> c2 >> point.y >> c3) || c1 != '(' || c2 != ';' || c3 != ')')
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const Point& point)
  {
    out << '(' << point.x << ';' << point.y << ')';
    return out;
  }

  std::istream& operator>>(std::istream& in, Polygon& poly)
  {
    std::istream::sentry sent(in);
    if (!sent) return in;

    StreamGuard guard(in);
    size_t count = 0;
    if (!(in >> count) || count < 3)
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    std::vector<Point> temp;
    temp.reserve(count);
    std::copy_n(std::istream_iterator<Point>(in), count, std::back_inserter(temp));

    if (in && temp.size() == count)
    {
      poly.points = std::move(temp);
    }
    else
    {
      in.setstate(std::ios::failbit);
    }

    return in;
  }

  std::ostream& operator<<(std::ostream& out, const Polygon& poly)
  {
    out << poly.points.size() << ' ';
    std::copy(poly.points.begin(), poly.points.end(), std::ostream_iterator<Point>(out, " "));
    return out;
  }

  double calculateArea(const Polygon& poly)
  {
    double area = 0.0;
    size_t n = poly.points.size();
    for (size_t i = 0; i < n; ++i)
    {
      size_t j = (i + 1) % n;
      area += (poly.points[i].x * poly.points[j].y) - (poly.points[j].x * poly.points[i].y);
    }
    return std::abs(area) / 2.0;
  }
}
