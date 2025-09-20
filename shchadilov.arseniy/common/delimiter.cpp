#include "delimiter.hpp"
#include <iostream>

std::istream& shchadilov::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '\0';
  in >> c;
  if (in && std::tolower(c) != dest.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
