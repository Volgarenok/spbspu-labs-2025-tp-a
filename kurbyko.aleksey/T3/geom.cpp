#include "geom.hpp"
#include <algorithm>
#include <iterator>
#include "iofmtguard.hpp"
#include "DelimiterIO.hpp"

std::istream& kurbyko::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  using sep = DelimiterIO;
  return in >> sep{ '(' } >> point.x >> sep{ ';' } >> point.y >> sep{ ')' };
}

std::istream& kurbyko::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t vertexesCount = 0;
  if (!(in >> vertexesCount) || vertexesCount < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > points(vertexesCount);
  std::copy_n(std::istream_iterator< Point >(in), vertexesCount, points.begin());
  if (!in)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  polygon.points = points;
  return in;
}

std::ostream& kurbyko::operator<<(std::ostream& out, const Point& point)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  iofmtguard guard(out);
  return out << '(' << point.x << ';' << point.y << ')';
}

std::ostream& kurbyko::operator<<(std::ostream& out, const Polygon& polygon)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  iofmtguard guard(out);
  out << polygon.points.size();
  std::copy_n(polygon.points.begin(), polygon.points.size() - 1, std::ostream_iterator< Point >(out, " "));
  out << polygon.points[polygon.points.size() - 1];
  return out;
}

bool kurbyko::operator==(const Point& point1, const Point& point2)
{
  return point1.x == point2.x && point1.y == point2.y;
}

bool kurbyko::operator==(const Polygon& polygon1, const Polygon& polygon2)
{
  return polygon1.points == polygon2.points;
}
