#include "delimiter.hpp"

std::istream & guseynov::operator>>(std::istream & in, DelimiterI && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = in.get();
  if (!in || c != dest.exp)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  return in;
}
