#include "Shapes.h"

#include <algorithm>
#include <cmath>
#include <iterator>

#include <Delimiter.h>
#include <IOFmtguard.h>

std::istream& voronina::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  Point input;
  in >> DelimiterIO{ '(' };
  in >> input.x;
  in >> DelimiterIO{ ';' };
  in >> input.y;
  in >> DelimiterIO{ ')' };

  if (!in.fail())
  {
    point = input;
  }
  return in;
}

std::istream& voronina::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  int vertexes = 0;
  const int MIN_AMOUNT_OF_VERTEXES = 3;

  in >> vertexes;
  if (!in || vertexes < MIN_AMOUNT_OF_VERTEXES)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::vector< Point > points(vertexes);
  std::copy_n(std::istream_iterator< Point >(in), vertexes, points.begin());
  if (!in)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  polygon.points = points;
  return in;
}

bool voronina::Point::operator==(const Point& point) const
{
  return (x == point.x && y == point.y);
}

voronina::Point voronina::operator-(const Point& left, const Point& right)
{
  return Point{ left.x - right.x, left.y - right.y };
}

double voronina::distance(const Point& left, const Point& right)
{
  int dx = left.x - right.x;
  int dy = left.y - right.y;
  return std::sqrt(dx * dx + dy * dy);
}

bool voronina::Polygon::operator==(const Polygon& polygon) const
{
  return (points == polygon.points);
}
