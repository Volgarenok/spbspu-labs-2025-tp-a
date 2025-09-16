#include <iomanip>
#include "point.h"

khokhryakova::StreamGuard::StreamGuard(std::ios& stream):
  stream_(stream),
  flags_(stream.flags())
{}

khokhryakova::StreamGuard::~StreamGuard()
{
  stream_.flags(flags_);
}

bool khokhryakova::Point::operator==(const Point& other) const
{
  return x == other.x && y == other.y;
}

std::istream& khokhryakova::operator>>(std::istream& in, DelimiterIO&& dest)
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

std::istream& khokhryakova::operator>>(std::istream& in, Point& p)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  StreamGuard guard(in);
  in >> khokhryakova::DelimiterIO{ '(' };
  in >> p.x;
  in >> khokhryakova::DelimiterIO{ ';' };
  in >> p.y;
  in >> khokhryakova::DelimiterIO{ ')' };
  return in;
}

std::ostream& khokhryakova::operator<<(std::ostream& out, const Point& p)
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
