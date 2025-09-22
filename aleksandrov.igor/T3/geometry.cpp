#include "geometry.hpp"
#include <iostream>
#include <iterator>
#include <algorithm>
#include <stream-guard.hpp>
#include <input-struct.hpp>

std::istream& aleksandrov::operator>>(std::istream& in, Point& rhs)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  Point temp;
  using sep = DelimeterI;
  in >> sep{ '(' } >> temp.x >> sep{ ';' } >> temp.y >> sep{ ')' };
  if (in)
  {
    rhs = temp;
  }
  return in;
}

std::istream& aleksandrov::operator>>(std::istream& in, Polygon& rhs)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t verticesCount = 0;
  if (!(in >> verticesCount) || verticesCount < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > points(verticesCount);
  std::copy_n(std::istream_iterator< Point >(in), verticesCount, points.begin());
  if (!in)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  rhs.points = points;
  return in;
}

