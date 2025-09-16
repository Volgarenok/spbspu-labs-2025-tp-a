#include "delimiter.hpp"

#include <istream>

std::istream& kostyukov::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (std::tolower(c) != std::tolower(dest.expected)))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

