#include "input_delimiter.hpp"

#include <cctype>

std::istream &fedorov::operator>>(std::istream &in, DelimiterInput &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c;
  in.get(c);
  if (in && std::tolower(c) != std::tolower(dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
