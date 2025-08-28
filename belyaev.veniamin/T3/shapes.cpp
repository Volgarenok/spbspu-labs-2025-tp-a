#include "shapes.hpp"
#include <algorithm>
#include <iterator>
#include <functional>
#include <stream-guard.hpp>
#include <delimiter.hpp>

std::istream& belyaev::operator>>(std::istream& in, Point& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  Point newPoint;
  in >> DelimiterIO{'('};
  in >> newPoint.x;
  in >> DelimiterIO{';'};
  in >> newPoint.y;
  in >> DelimiterIO{')'};
  if (in)
  {
    dest = newPoint;
  }
  return in;
}

std::ostream& belyaev::operator<<(std::ostream& out, const Point& src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  belyaev::StreamGuard guard(out);
  out << "(" << src.x << ";" << src.y << ")";
  return out;
}

std::istream& belyaev::operator>>(std::istream& in, Polygon& dest)
{
  using namespace std::placeholders;

  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  size_t pointsAmount;
  in >> pointsAmount;
  if (!in || pointsAmount < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  using istreamPnt = std::istream_iterator< Point >;
  std::vector< Point > newPoints(pointsAmount, Point{0, 0});
  std::copy_n(istreamPnt(in), pointsAmount, newPoints.begin());
  if (in && pointsAmount == newPoints.size())
  {
    dest.points = std::move(newPoints);
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::ostream& belyaev::operator<<(std::ostream& out, const Polygon& src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  belyaev::StreamGuard guard(out);
  using ostreamPnt = std::ostream_iterator< Point >;
  std::copy(src.points.begin(), src.points.end(), ostreamPnt{out, " "});
  return out;
}

bool belyaev::operator==(const Point& lhs, const Point& rhs)
{
  return (lhs.x == rhs.x && lhs.y == rhs.y);
}

bool belyaev::operator==(const Polygon& lhs, const Polygon& rhs)
{
  if (lhs.points.size() != rhs.points.size())
  {
    return false;
  }
  return std::is_permutation(lhs.points.begin(), lhs.points.end(), rhs.points.begin());
}
