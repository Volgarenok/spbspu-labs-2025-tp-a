#include <algorithm>
#include <cmath>
#include <numeric>
#include <iomanip>
#include <iterator>
#include "poligon.h"

namespace khokhryakova
{
  bool Polygon::operator==(const Polygon& other) const
  {
    return points == other.points;
  }

  double getArea(const Polygon& poly)
  {
    double area = 0.0;
    for (size_t i = 0; i < poly.points.size(); ++i)
    {
      const auto& point1 = poly.points[i];
      const auto& point2 = poly.points[(i + 1) % poly.points.size()];
      area += (point1.x * point2.y) - (point2.x * point1.y);
    }
    return std::abs(area) / 2.0;
  }

  std::istream& operator>>(std::istream& in, Polygon& poly)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    StreamGuard guard(in);
    size_t len = 0;
    if (!(in >> len) || len < 3)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    std::vector< Point > temp(len);
    temp.reserve(len);
    std::copy_n(std::istream_iterator< Point >(in), len, temp.begin());
    if (in)
    {
      poly.points = std::move(temp);
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const Polygon& poly)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    StreamGuard guard(out);
    out << poly.points.size();
    for (const auto& p : poly.points)
    {
      out << " " << p;
    }
    return out;
  }
}
