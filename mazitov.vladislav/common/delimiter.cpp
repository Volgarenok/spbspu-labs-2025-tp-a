#include "delimiter.hpp"

std::istream &mazitov::operator>>(std::istream &in, DelimiterInput &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c = 0;
  if ((in >> c) && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
