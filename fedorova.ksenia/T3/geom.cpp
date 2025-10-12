#include <algorithm>
#include <iterator>
#include "geom.h"
#include <io-guard.h>
#include <delimiter.h>

bool fedorova::operator==(const Point& pointOne, const Point& pointTwo)
{
  return pointOne.x == pointTwo.x && pointOne.y == pointTwo.y;
}

bool fedorova::operator==(const Polygon& polygonOne, const Polygon& polygonTwo)
{
  return polygonOne.points == polygonTwo.points;
}

fedorova::Point fedorova::operator-(const Point& left, const Point& right)
{
  return Point{ left.x - right.x, left.y - right.y };
}

std::istream& fedorova::operator>>(std::istream& is, Point& point)
{
  std::istream::sentry sentry(is);
  if (!sentry)
  {
    return is;
  }

  return is >> DelimiterIO{ '(' } >> point.x >> DelimiterIO{ ';' } >> point.y >> DelimiterIO{ ')' };
}

std::istream& fedorova::operator>>(std::istream& is, Polygon& polygon)
{
  std::istream::sentry sentry(is);
  if (!sentry)
  {
    return is;
  }

  size_t countVert = 0;
  if (!(is >> countVert) || countVert <= 2)
  {
    is.setstate(std::ios::failbit);
    return is;
  }

  std::vector< Point > points(countVert);
  std::copy_n(std::istream_iterator< Point >(is), countVert, points.begin());

  if (!is)
  {
    is.setstate(std::ios::failbit);
    return is;
  }

  polygon.points = points;
  return is;
}

std::ostream& fedorova::operator<<(std::ostream& os, const Point& point)
{
  std::ostream::sentry sentry(os);
  if (!sentry)
  {
    return os;
  }

  IoGuard guard(os);
  return os << '(' << point.x << ';' << point.y << ')';
}

std::ostream& fedorova::operator<<(std::ostream& os, const Polygon& polygon)
{
  std::ostream::sentry sentry(os);
  if (!sentry)
  {
    return os;
  }

  IoGuard guard(os);
  os << polygon.points.size();
  std::copy_n(polygon.points.begin(), polygon.points.size() - 1, std::ostream_iterator< Point >(os, " "));
  os << polygon.points[polygon.points.size() - 1];
  return os;
}

