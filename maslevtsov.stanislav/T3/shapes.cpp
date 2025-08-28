#include "shapes.hpp"
#include <algorithm>
#include <iterator>

namespace {
  struct DelimiterIn
  {
    char exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIn&& rhs)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != rhs.exp)) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
}

std::istream& maslevtsov::operator>>(std::istream& in, Point& rhs)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  Point temp{0, 0};
  in >> DelimiterIn{'('} >> temp.x >> DelimiterIn{';'} >> temp.y >> DelimiterIn{')'};
  if (in) {
    rhs = temp;
  }
  return in;
}

std::istream& maslevtsov::operator>>(std::istream& in, Polygon& rhs)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  std::size_t vertex_num = 0;
  in >> vertex_num;
  if (!in || vertex_num < 3) {
    in.setstate(std::ios::failbit);
    return in;
  }
  rhs.points.clear();
  std::copy_n(std::istream_iterator< Point >(in), vertex_num - 1, std::back_inserter(rhs.points));
  if (in.peek() != '\n') {
    std::copy_n(std::istream_iterator< Point >(in), 1, std::back_inserter(rhs.points));
  }
  if (!in || rhs.points.size() != vertex_num || in.peek() != '\n') {
    rhs.points.clear();
    in.setstate(std::ios::failbit);
  }
  return in;
}

bool maslevtsov::operator==(const Point& lhs, const Point& rhs)
{
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}
