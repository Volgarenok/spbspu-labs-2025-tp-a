#include "polygon.hpp"
#include <numeric>
#include <algorithm>
#include <functional>
#include <iterator>
#include "streamguard.hpp"

std::istream& klimova::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (c != dest.exp)) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& klimova::operator>>(std::istream& in, Point& point)
{
  using sep = DelimiterIO;
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  int x = 0;
  int y = 0;
  in >> sep{ '(' } >> x >> sep{ ';' };
  in >> y >> sep{ ')' };
  if (!in) {
    in.setstate(std::ios::failbit);
    return in;
  }
  point.x = x;
  point.y = y;
  return in;
}

std::ostream& klimova::operator<<(std::ostream& out, const Point& point)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  Streamguard fmtguard(out);
  out << "(" << point.x << ';' << point.y << ")";
  return out;
}

std::istream& klimova::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  int vertexes = 0;
  if (!(in >> vertexes) || vertexes < 3) {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > points(vertexes, Point{ 0, 0 });
  using inputIt = std::istream_iterator< Point >;
  std::copy_n(inputIt{ in }, vertexes, points.begin());
  polygon.points = points;
  return in;
}

std::ostream& klimova::operator<<(std::ostream& out, const Polygon& polygon)
{
  std::ostream::sentry s(out);
  if (!s) {
    return out;
  }
  out << polygon.points.size();
  using outputIt = std::ostream_iterator< Point >;
  std::copy(std::begin(polygon.points), std::end(polygon.points), outputIt{ out, "" });
  return out;
}
