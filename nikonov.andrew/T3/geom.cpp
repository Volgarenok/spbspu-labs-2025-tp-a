#include "geom.hpp"
#include <istream>
#include <ostream>
#include <iterator>
#include <vector>
#include <DelimetrIO.hpp>

std::istream& nikonov::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  using sep = DelimiterIO;
  return in >> sep{ '(' } >> point.x >> sep{ ';' } >> point.y >> sep{ ')' };
}

std::istream& nikonov::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t vertexCnt = 0;
  if (!(in >> vertexCnt) || vertexCnt < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > pts(vertexCnt);
  for (size_t i = 0; i < vertexCnt; ++i)
  {
    if (!(in >> pts[i]))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }
  polygon.points = std::move(pts);
  return in;
}

std::ostream& nikonov::operator<<(std::ostream& out, const Point& point)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  return out << '(' << point.x << ';' << point.y << ')';
}

std::ostream& nikonov::operator<<(std::ostream& out, const Polygon& polygon)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << polygon.points.size();
  for (size_t i = 0; i < polygon.points.size(); ++i)
  {
    out << ' ' << polygon.points[i];
  }
  return out;
}

bool nikonov::operator==(const Point& a, const Point& b)
{
  return (a.x == b.x && a.y == b.y);
}

bool nikonov::operator==(const Polygon& a, const Polygon& b)
{
  return a.points == b.points;
}
