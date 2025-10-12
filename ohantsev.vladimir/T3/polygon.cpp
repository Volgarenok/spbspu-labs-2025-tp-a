#include "polygon.h"
#include <vector>
#include <numeric>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <delimiter.h>

int ohantsev::areaGaussPairSum(const Point& lhs, const Point& rhs)
{
  return lhs.x * rhs.y - rhs.x * lhs.y;
}

double ohantsev::getArea(const Polygon& polygon)
{
  std::vector< int > counting;
  counting.reserve(polygon.size());
  std::transform
  (
    polygon.points.cbegin(),
    polygon.points.cend() - 1,
    polygon.points.cbegin() + 1,
    std::back_inserter(counting),
    areaGaussPairSum
  );
  counting.push_back(areaGaussPairSum(polygon.points.back(), polygon.points.front()));
  return std::abs(std::accumulate(counting.begin(), counting.end(), 0)) / 2.0;
}

std::size_t ohantsev::Polygon::size() const noexcept
{
  return points.size();
}

std::istream& ohantsev::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return (in >> DelimiterIO{ '(' } >> point.x >> DelimiterIO{ ';' } >> point.y >> DelimiterIO{ ')' });
}

std::istream& ohantsev::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::size_t size;
  if (in >> size)
  {
    if (size < 3)
    {
      in.setstate(std::ios::failbit);
    }
    polygon.points.clear();
    polygon.points.reserve(size);
    std::copy_n(std::istream_iterator< Point >(in), size, std::back_inserter(polygon.points));
  }
  return in;
}

bool ohantsev::Point::operator<(const Point& rhs) const noexcept
{
  if (x != rhs.x)
  {
    return x < rhs.x;
  }
  return y < rhs.y;
}

bool ohantsev::Point::operator==(const Point& rhs) const noexcept
{
  return x == rhs.x && y == rhs.y;
}

bool ohantsev::Polygon::operator==(const Polygon& rhs) const noexcept
{
  if (size() != rhs.size())
  {
    return false;
  }
  return std::equal(points.begin(), points.end(), rhs.points.begin());
}
