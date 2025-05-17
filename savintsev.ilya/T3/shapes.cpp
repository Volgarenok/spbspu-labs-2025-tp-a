#include "shapes.h"
#include "utils.h"

std::istream & savintsev::operator>>(std::istream & in, Point & dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  int x = 0, y = 0;
  in >> DelimiterIO{'('};
  in >> x;
  in >> DelimiterIO{';'};
  in >> y;
  in >> DelimiterIO{')'};

  if (!in)
  {
    return in;
  }

  dest.x = x;
  dest.y = y;

  return in;
}

std::ostream & savintsev::operator<<(std::ostream & out, const Point & dest)
{
  out << '(' << dest.x << ';' << dest.y << ')';
  return out;
}

std::istream & savintsev::operator>>(std::istream & in, Polygon & dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  size_t n = 0;
  in >> n;

  if (!in)
  {
    return in;
  }

  Polygon data;

  for (size_t i = 0; i < n; ++i)
  {
    Point p;
    in >> p;
    if (!in)
    {
      return in;
    }
    data.points.push_back(p);
  }

  dest = data;

  return in;
}

std::ostream & savintsev::operator<<(std::ostream & out, const Polygon & dest)
{
  out << dest.points.size();
  for (size_t i = 0; i < dest.points.size(); ++i)
  {
    std::cout << ' ' << dest.points[i];
  }
  return out;
}
