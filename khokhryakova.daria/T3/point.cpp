#include <iomanip>
#include "point.h"

namespace khokhryakova
{
  bool Point::operator==(const Point& other) const
  {
    return x == other.x && y == other.y;
  }
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c = '\0';
    in >> c;
    if (c != dest.exp)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
  std::istream& operator>>(std::istream& in, Point& p)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    StreamGuard guard(in);
    in >> DelimiterIO{ '(' } >> p.x >> DelimiterIO{ ';' } >> p.y >> DelimiterIO{ ')' };
    return in;
  }
  std::ostream& operator<<(std::ostream& out, const Point& p)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    StreamGuard guard(out);
    out << "(" << p.x << ";" << p.y << ")";
    return out;
  }
}
