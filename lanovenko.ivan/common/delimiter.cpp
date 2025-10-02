#include "delimiter.hpp"

std::istream& lanovenko::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  char c = 0;
  in >> c;
  if (!in && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
