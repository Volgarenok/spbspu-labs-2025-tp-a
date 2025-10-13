#include <functional>
#include <algorithm>
#include <delimiter_io.hpp>
#include "shapes.hpp"

bool cherepkov::Point::operator==(const Point & rhs) const
{
  return x == rhs.x && y == rhs.y;
}

cherepkov::Point cherepkov::Point::operator-(const Point & rhs) const
{
  return {x - rhs.x, y - rhs.y};
}


std::istream& cherepkov::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sent(in);
  if (!sent)
  {
    return in;
  }

  Point tmp;
  in >> DelimiterIO{'('} >> tmp.x >> DelimiterIO{';'} >> tmp.y >> DelimiterIO{')'};

  if (in)
  {
    point = tmp;
  }
  return in;
}

std::istream& cherepkov::operator>>(std::istream& in, Polygon& polygon)
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
  if (!in)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  polygon.points = std::move(temp);
  return in;
}

cherepkov::Point cherepkov::handleNewLine(std::istream& in, const Point& point)
{
  if (in.peek() == '\n')
  {
    in.setstate(std::ios::eofbit);
  }
  return point;
}
