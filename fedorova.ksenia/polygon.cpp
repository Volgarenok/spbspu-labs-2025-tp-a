#include "polygon.h"

bool fedorova::operator==(const Point& lhs, const Point& rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool fedorova::operator==(const Polygon& lhs, const Polygon& rhs)
{
  return lhs.points == rhs.points;
}
