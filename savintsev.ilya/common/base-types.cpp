#include "base-types.hpp"
#include "io-delimiter.hpp"

std::istream & savintsev::operator>>(std::istream & in, point_t & dest)
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

std::ostream & savintsev::operator<<(std::ostream & out, const point_t & dest)
{
  out << dest.x << ' ' << dest.y;
  return out;
}
