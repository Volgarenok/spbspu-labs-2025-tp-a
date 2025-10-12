#include "polygon.hpp"
#include <algorithm>
#include <iterator>
#include <delimiterIO.hpp>

std::istream &pilugina::operator>>(std::istream &in, Point &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  Point temp{0, 0};
  in >> DelimiterIO{'('} >> temp.x >> DelimiterIO{';'} >> temp.y >> DelimiterIO{')'};
  if (in)
  {
    dest = temp;
  }
  return in;
}

std::istream &pilugina::operator>>(std::istream &in, Polygon &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::size_t vertex_num = 0;
  in >> vertex_num;
  if (!in || vertex_num < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  dest.points.clear();
  std::copy_n(std::istream_iterator< Point >{in}, vertex_num - 1, std::back_inserter(dest.points));
  if (in.peek() != '\n')
  {
    std::copy_n(std::istream_iterator< Point >{in}, 1, std::back_inserter(dest.points));
  }
  if (!in || dest.points.size() != vertex_num || in.peek() != '\n')
  {
    dest.points.clear();
    in.setstate(std::ios::failbit);
  }
  return in;
}

bool pilugina::operator==(const Point &lhs, const Point &rhs)
{
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

bool pilugina::operator<(const Point &lhs, const Point &rhs)
{
  return (lhs.x < rhs.x) || (lhs.x == rhs.x && lhs.y < rhs.y);
}

bool pilugina::operator==(const Polygon &lhs, const Polygon &rhs)
{
  return lhs.points == rhs.points;
}

bool pilugina::operator<(const Polygon &lhs, const Polygon &rhs)
{
  return lhs.points.size() < rhs.points.size();
}
