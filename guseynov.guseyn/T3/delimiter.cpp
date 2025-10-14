#include "delimiter.hpp"

std::istream & guseynov::operator>>(std::istream & in, DelimiterI && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = 0;
  in >> c;
  if (!in || c != dest.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
