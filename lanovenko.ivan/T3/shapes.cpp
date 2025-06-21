#include "shapes.hpp"

std::istream& lanovenko::operator>>(std::istream& in, Point& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  in >> DelimiterIO{ '(' };
  in >> dest.x >> DelimiterIO{ ';' };
  in >> dest.y >> DelimiterIO{ ')' };
  return in;
}

std::istream& lanovenko::operator>>(std::istream& in, Polygon& dest)
{
  std::istream::sentry s(in);
  if (!s)
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
  if (!in)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  dest.points = std::move(temp);
  return in;
}

bool lanovenko::Point::operator==(const Point& rhs) const noexcept
{
  return x == rhs.x && y == rhs.y;
}

lanovenko::Point lanovenko::Point::operator-(const Point& rhs) const noexcept
{
  return { x - rhs.x, y - rhs.y };
}

bool lanovenko::Polygon::operator==(const Polygon& rhs) const noexcept
{
  return points == rhs.points;
}
