#include "polygon.hpp"
#include <algorithm>
#include <istream>
#include <iterator>
#include <stream-guard.hpp>

namespace kizhin {
  struct Delimiter;
  std::istream& operator>>(std::istream&, Delimiter&&);
  std::istream& operator>>(std::istream&, Point&);
}

bool kizhin::operator==(const Polygon& lhs, const Polygon& rhs)
{
  return lhs.points == rhs.points;
}

bool kizhin::operator==(const Point& lhs, const Point& rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

std::istream& kizhin::operator>>(std::istream& in, Polygon& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  const StreamGuard guard(in);
  in >> std::noskipws;
  std::size_t size = 0;
  if (!(in >> size) || size < 3) {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::istream& s = in;
  using InIt = std::istream_iterator< Point >;
  PointContainer points;
  points.reserve(size);
  points.insert(points.end(), InIt{ s }, InIt{});
  if (points.size() == size) {
    dest.points = std::move(points);
    in.clear();
  }
  return in;
}

struct kizhin::Delimiter
{
  char val = 0;
};

std::istream& kizhin::operator>>(std::istream& in, Point& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  Point input;
  in >> Delimiter{ ' ' };
  in >> Delimiter{ '(' } >> input.x >> Delimiter{ ';' };
  if (in >> input.y >> Delimiter{ ')' }) {
    dest = input;
  }
  return in;
}

std::istream& kizhin::operator>>(std::istream& in, Delimiter&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  if (in.get() != dest.val) {
    in.unget();
    in.setstate(std::ios::failbit);
  }
  return in;
}

