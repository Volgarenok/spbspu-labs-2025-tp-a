#include "Point.h"
#include <iomanip>
#include <iostream>
#include <Delimiter.h>

bool horoshilov::Point::operator==(const Point& other) const
{
  return x == other.x && y == other.y;
}
bool horoshilov::Point::operator<(const Point& other) const
{
  return x < other.x || (x == other.x && y < other.y);
}

std::istream& horoshilov::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{ '(' } >> point.x >> DelimiterIO{ ';' } >> point.y >> DelimiterIO{ ')' };
  return in;
}
