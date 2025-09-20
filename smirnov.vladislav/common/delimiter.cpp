#include "delimiter.hpp"

std::istream& io::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c;
  in >> c;
  if (!in || c != dest.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
