#include "polygon.hpp"
#include "delimiter.hpp"
#include <iterator>
#include <algorithm>

std::istream & guseynov::operator>>(std::istream & in, Point & point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = 0;
  in >> c;
  if (!in || c != '(')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  in >> point.x;
  if (!in)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  in >> c;
  if (!in || c != ';')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  in >> point.y;
  if (!in)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  in >> c;
  if (!in || c != ')')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  return in;
}

std::ostream & guseynov::operator<<(std::ostream & out, const Point & point)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << '(' << point.x << ';' << point.y << ')';
  return out;
}

bool guseynov::operator==(const Point & lhs, const Point & rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

std::istream & guseynov::operator>>(std::istream & in, Polygon & polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t num = 0;
  if (!(in >> num) || num < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector<Point> points;
  points.reserve(num);
  for (size_t i = 0; i < num; ++i)
  {
    Point point;
    if (in >> point)
    {
      points.push_back(point);
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }
  if (in)
  {
    polygon.points = std::move(points);
  }
  return in;
}

bool guseynov::operator==(const Polygon & lhs, const Polygon & rhs)
{
  return lhs.points == rhs.points;
}
