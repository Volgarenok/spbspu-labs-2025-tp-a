#include "point.hpp"

bool guseynov::Point::operator==(const Point& other) const
{
  return x == other.x && y == other.y;
}
