#include "delimeter-io.hpp"

std::istream &sveshnikov::operator>>(std::istream &in, DelimiterIO &&dest)
{
  std::istream::sentry sentry(in);
  if (sentry && in.get() != dest.exp)
  {
    in.unget();
    in.setstate(std::ios::failbit);
  }
  return in;
}
