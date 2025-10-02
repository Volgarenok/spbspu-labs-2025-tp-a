#include "polygon.hpp"

bool averenkov::Point::operator==(const Point& other) const
{
  return x == other.x && y == other.y;
}

bool averenkov::Point::operator!=(const Point& other) const
{
  return !(*this == other);
}

std::istream& averenkov::operator>>(std::istream& in, Point& p)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> averenkov::DelimiterIO{'('};
  in >> p.x;
  in >> averenkov::DelimiterIO{';'};
  in >> p.y;
  in >> averenkov::DelimiterIO{')'};
  return in;
}

std::istream& averenkov::operator>>(std::istream& in, averenkov::Polygon& poly)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t numPoints = 0;
  in >> numPoints;
  if (!in || numPoints < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > points{ numPoints };
  std::copy_n(std::istream_iterator< Point >(in), numPoints, points.begin());
  if (!in || points.size() != numPoints)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  poly.points = points;
  return in;
}

bool averenkov::Polygon::operator==(const Polygon& other) const
{
  return points == other.points;
}

