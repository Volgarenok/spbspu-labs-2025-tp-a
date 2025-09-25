#include "delimiter.hpp"
#include <cctype>

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
    return in;
  }
  if (dest.exp == ';')
  {
    char next = in.peek();
    if (next == ')')
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }
  return in;
}
