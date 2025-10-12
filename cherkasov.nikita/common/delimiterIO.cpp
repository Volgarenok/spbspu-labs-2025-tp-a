#include "delimiterIO.hpp"
#include <istream>

std::istream& operator>>(std::istream& in, const DelimiterIO& d)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  char c = 0;
  in >> c;
  if (!in)
  {
    return in;
  }
  if (c != d.ch_)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
