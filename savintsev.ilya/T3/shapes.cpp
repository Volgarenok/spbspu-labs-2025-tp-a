#include "shapes.h"
#include <algorithm>
#include <iterator>
#include <io-delimiter.hpp>

bool savintsev::operator<(const Point & a, const Point & b)
{
  return (a.x != b.x) ? a.x < b.x : a.y < b.y;
}

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
  if (n <= 2)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  Polygon data;
  data.points.resize(n);

  std::copy_n(std::istream_iterator< Point >(in), n, data.points.begin());

  if (in.fail())
  {
    data.points.clear();
    return in;
  }

  dest = data;

  return in;
}

std::ostream & savintsev::operator<<(std::ostream & out, const Polygon & dest)
{
  out << dest.points.size();
  if (!dest.points.empty())
  {
    out << ' ' << dest.points.front();
    std::copy(dest.points.begin() + 1, dest.points.end(), std::ostream_iterator< Point >(out, " "));
  }
  return out;
}
