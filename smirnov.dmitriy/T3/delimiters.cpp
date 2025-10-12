#include "delimiters.hpp"
#include <algorithm>
#include "scopeguard.hpp"

std::istream& smirnov::operator>>(std::istream& in, DelimiterString&& exp)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return in;
  }

  size_t i = 0;
  while (exp.expected[i] != '\0')
  {
    in >> DelimiterChar({ exp.expected[i++] });
  }
  return in;
}

std::istream& smirnov::operator>>(std::istream& in, DelimiterChar&& exp)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  smirnov::ScopeGuard guard(in);
  char c = 0;
  in >> c;
  if (!(c == exp.expected || c == std::tolower(exp.expected)))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
