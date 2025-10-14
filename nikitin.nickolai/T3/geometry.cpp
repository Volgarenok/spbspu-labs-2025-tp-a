#include "geometry.hpp"
#include <delimiter.hpp>

bool nikitin::operator<(const Point& lhs, const Point& rhs)
{
  return (lhs.x < rhs.x) || (lhs.x == rhs.x && lhs.y < rhs.y);
}

bool nikitin::operator==(const Point& lhs, const Point& rhs)
{
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

std::istream& nikitin::operator>>(std::istream& in, Point& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterI{'('} >> dest.x >> DelimiterI{';'} >> dest.y >> DelimiterI{')'};
  return in;
}

std::ostream& nikitin::operator<<(std::ostream& out, const Point& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << '(' << dest.x << ';' << dest.y << ')';
  return out;
}

bool nikitin::operator==(const Polygon& lhs, const Polygon& rhs)
{
  return lhs.points == rhs.points;
}

std::istream& nikitin::operator>>(std::istream& in, Polygon& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::size_t cnt = 0;
  if (!(in >> cnt) || (cnt < 3))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::vector< Point > temp(cnt);
  std::copy_n(std::istream_iterator< Point >(in), cnt, temp.begin());
  if (!in)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  dest.points = std::move(temp);
  return in;
}

std::ostream& nikitin::operator<<(std::ostream& out, const Polygon& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  out << dest.points.size();

  if (!dest.points.empty())
  {
    out << ' ';
    std::copy(dest.points.begin(), dest.points.end(), std::ostream_iterator< Point >(out, " "));
  }
  return out;
}
