#include "polygon.hpp"
#include <algorithm>
#include <iterator>
#include <numeric>
#include <delimiter.hpp>

std::istream &mazitov::operator>>(std::istream &in, Point &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  Point temp{0, 0};
  in >> DelimiterInput{'('} >> temp.x >> DelimiterInput{';'} >> temp.y >> DelimiterInput{')'};
  if (in)
  {
    dest = temp;
  }
  return in;
}

std::ostream &mazitov::operator<<(std::ostream &out, const Point &src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << '(' << src.x << ';' << src.y << ')';
  return out;
}

std::istream &mazitov::operator>>(std::istream &in, Polygon &dest)
{
  using pointIt = std::istream_iterator< Point >;
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
  std::copy_n(pointIt{in}, vertex_num - 1, std::back_inserter(dest.points));
  if (in.peek() != '\n')
  {
    std::copy_n(pointIt{in}, 1, std::back_inserter(dest.points));
  }

  if (!in || dest.points.size() != vertex_num || in.peek() != '\n')
  {
    dest.points.clear();
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::ostream &mazitov::operator<<(std::ostream &out, const Polygon &src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  out << src.points.size();

  if (!src.points.empty())
  {
    out << ' ';
    std::copy(src.points.begin(), src.points.end(), std::ostream_iterator< Point >(out, " "));
  }

  return out;
}

bool mazitov::operator==(const Point &lhs, const Point &rhs)
{
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

bool mazitov::operator<(const Point &lhs, const Point &rhs)
{
  return (lhs.x < rhs.x) || (lhs.x == rhs.x && lhs.y < rhs.y);
}

bool mazitov::operator==(const Polygon &lhs, const Polygon &rhs)
{
  return lhs.points == rhs.points;
}

double mazitov::getPolygonArea(const Polygon &poly)
{
  if (poly.points.size() < 3)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  double area = std::accumulate(poly.points.cbegin(), poly.points.cend(), 0.0, PolygonAreaAccumulator(poly)) / 2.0;
  return std::abs(area);
}

double mazitov::areaSumOperator(double init, const Polygon &poly)
{
  return init + getPolygonArea(poly);
}

bool mazitov::isEvenVertexNum(const Polygon &poly)
{
  return (poly.points.size() % 2) == 0;
}

bool mazitov::isOddVertexNum(const Polygon &poly)
{
  return (poly.points.size() % 2) == 1;
}

bool mazitov::isEqualVertexNum(std::size_t n, const Polygon &poly)
{
  return n == poly.points.size();
}
