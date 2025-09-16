#include "delimiter_io.hpp"
#include <cctype>

std::istream& cherepkov::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c = '0';
  in >> c;
  c = std::tolower(c);

  if (in && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
