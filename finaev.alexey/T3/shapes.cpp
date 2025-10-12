#include "shapes.hpp"
#include <iterator>
#include <algorithm>

std::istream& finaev::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{'('} >> point.x >> DelimiterIO{';'} >> point.y >> DelimiterIO{')'};
  return in;
}

std::ostream& finaev::operator<<(std::ostream& out, const Point& point)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << "(" << point.x << ";" << point.y << ")";
  return out;
}

std::istream& finaev::operator>>(std::istream& in, Polygon& poly)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
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
    poly.points = std::move(temp);
  }
  return in;
}

std::ostream& finaev::operator<<(std::ostream& out, const Polygon& poly)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  std::copy(poly.points.begin(), poly.points.end(), std::ostream_iterator< Point >{ out, " " });
  return out;
}
