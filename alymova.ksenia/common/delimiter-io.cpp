#include "delimiter-io.hpp"

std::istream& alymova::operator>>(std::istream& in, DelimiterIO&& object)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char delim;
  if ((in >> delim) && delim != object.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
