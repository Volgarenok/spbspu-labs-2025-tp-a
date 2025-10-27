#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iterator>
#include <numeric>
#include <utility>

#include "polygon.hpp"
#include "streamGuard.hpp"

namespace ivanova
{
  bool Point::operator==(const Point& other) const
  {
    return x == other.x && y == other.y;
  }

  Point Point::operator-(const Point& other) const
  {
    return { x - other.x, y - other.y };
  }

  Point Point::operator-() const
  {
    return { -x, -y };
  }

  std::size_t Polygon::size() const
  {
    return points.size();
  }

  double Polygon::area() const
  {
    if (points.size() < 3)
    {
      return 0.0;
    }
    double sum = std::inner_product(
      points.begin(), points.end() - 1,
      points.begin() + 1,
      0.0,
      std::plus<>(),
      crossMultiply
    );
    sum += crossMultiply(points.back(), points.front());
    return std::abs(sum) * 0.5;
  }

  bool Polygon::operator==(const Polygon& other) const
  {
    return points == other.points;
  }

  double Polygon::crossMultiply(const Point& a, const Point& b)
  {
    return a.x * b.y - a.y * b.x;
  }

  std::istream& operator>>(std::istream& in, Point& point)
  {
    std::istream::sentry sent(in);
    if (!sent)
    {
      return in;
    }

    StreamGuard guard(in);
    char c1 = 0;
    char c2 = 0;
    char c3 = 0;
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

  std::istream& operator>>(std::istream& in, Polygon& polygon)
  {
    std::istream::sentry sent(in);
    if (!sent)
    {
      return in;
    }
    std::size_t count = 0;
    if (!(in >> count) || count < 3)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    std::vector< Point > temp;
    temp.reserve(count);
    std::copy_n(std::istream_iterator< Point >(in), count, std::back_inserter(temp));
    if (!in || temp.size() != count)
    {
      in.setstate(std::ios::failbit);
    }
    else
    {
      polygon.points = std::move(temp);
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const Polygon& poly)
  {
    out << poly.size() << " ";
    std::ostream_iterator< Point > it(out, " ");
    std::copy(poly.points.begin(), poly.points.end(), it);
    return out;
  }
}
