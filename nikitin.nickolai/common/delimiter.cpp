#include "delimiter.hpp"

std::istream& nikitin::operator>>(std::istream& in, DelimiterI&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c = '\0';
  if ((in >> c) && (c != dest.exp_))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
