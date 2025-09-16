#include "Polygon.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <vector>
#include <numeric>
#include <functional>
#include <Delimiter.h>
#include <Guard.h>
namespace
{
  struct AreaCalculator
  {
    mezentsev::Point point;
    double operator()(double area, const mezentsev::Point& point2)
    {
      area += (point.x * point2.y) - (point2.x * point.y);
      point = point2;
      return area;
    }
  };
}

bool mezentsev::Point::operator==(const Point& other) const
{
  return x == other.x && y == other.y;
}

bool mezentsev::Polygon::operator==(const Polygon& other) const
{
  return points == other.points;
}

std::istream& mezentsev::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  mezentsev::Guard scope(in);
  in >> DelimiterIO{ '(' } >> point.x >> DelimiterIO{ ';' } >> point.y >> DelimiterIO{ ')' };
  return in;
}

std::istream& mezentsev::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  mezentsev::Guard scope(in);
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

double mezentsev::getArea(const Polygon& polygon)
{
  AreaCalculator calc{ polygon.points.back() };
  double area = std::accumulate(polygon.points.begin(), polygon.points.end(), 0.0, std::ref(calc));
  return std::abs(area) / 2.0;
}
