#include "polygon.hpp"
#include <algorithm>
#include <cmath>
#include <istream>
#include <iterator>
#include <vector>
#include <numeric>
#include <functional>
#include "scopeGuard.hpp"
#include "delimeters.hpp"

namespace
{
  struct CalcAreaTerm
  {
    double operator()(const kiselev::Point& p1, const kiselev::Point& p2)
    {
      return p1.x * p2.y - p2.x * p1.y;
    }
  };

  double distance(const kiselev::Point p1, const kiselev::Point& p2)
  {
    return std::sqrt(std::pow((p1.x - p2.x), 2) + std::pow((p1.y - p2.y), 2));
  }
}

std::istream& kiselev::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  detail::ScopeGuard scope(in);
  in >> DelimeterIO{ '(' } >> point.x >> DelimeterIO{ ';' } >> point.y >> DelimeterIO{ ')' };
  return in;
}


std::istream& kiselev::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  detail::ScopeGuard scope(in);
  size_t count = 0;
  if (!(in >> count) || count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > temp(count);
  std::copy_n(std::istream_iterator< Point >(in), count, temp.begin());
  if (in)
  {
    polygon.points = std::move(temp);
  }
  return in;
}

double kiselev::getArea(const Polygon& polygon)
{
  const auto points = polygon.points;
  const Point first = points.front();
  const Point last = points.back();
  double area = std::inner_product(
    points.begin(), points.end() - 1, points.begin() + 1, CalcAreaTerm()(last, first), std::plus< double >(), CalcAreaTerm());
  return std::abs(area) / 2.0;
}

bool kiselev::isRect(const Polygon& poly)
{
  return (poly.points.size() == 4) && (distance(poly.points[0], poly.points[2]) == distance(poly.points[1], poly.points[3]));
}
