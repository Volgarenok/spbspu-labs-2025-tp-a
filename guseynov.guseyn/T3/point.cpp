#include "point.hpp"
#include <iostream>
#include <sstream>

bool guseynov::Point::operator==(const Point& other) const
{
  return x == other.x && y == other.y;
}

std::istream& guseynov::operator>>(std::istream& in, Point& point)
{
  char open = '\0';
  char separator = '\0';
  char close = '\0';
  if (!(in >> open >> point.x >> separator >> point.y >> close))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (open != '(' || separator != ';' || close != ')')
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
