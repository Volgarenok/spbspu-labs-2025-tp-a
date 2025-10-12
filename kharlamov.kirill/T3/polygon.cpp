#include "polygon.h"
#include <algorithm>
#include <iterator>
#include <guard.h>
#include "delimetr.h"

std::istream& kharlamov::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t count = 0;
  if (!(in >> count) || count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > temp(count);
  std::copy_n(std::istream_iterator< Point >(in), count, temp.begin());
  if (in)
  {
    polygon.points = std::move(temp);
  }
  return in;
}

bool kharlamov::Polygon::operator==(const Polygon& other) const
{
  return points == other.points;
}
