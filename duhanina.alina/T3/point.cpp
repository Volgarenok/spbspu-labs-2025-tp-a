#include "point.hpp"
#include <iostream>
#include <dataStruct.hpp>

bool duhanina::operator==(const duhanina::Point& lhs, const duhanina::Point& rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool duhanina::operator!=(const duhanina::Point& lhs, const duhanina::Point& rhs)
{
  return !(lhs == rhs);
}

std::istream& duhanina::operator>>(std::istream& in, duhanina::Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{ '(' } >> point.x >> DelimiterIO{ ';' } >> point.y >> DelimiterIO{ ')' };
  return in;
}

