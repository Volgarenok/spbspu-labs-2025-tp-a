#include "shapes.hpp"
#include <algorithm>
#include <iterator>
#include <numeric>
#include <functional>
#include <delimiter.hpp>

double karnauhova::CalcAreaPoint::operator()(const karnauhova::Point& p1, const karnauhova::Point& p2)
{
  return p1.x * p2.y - p2.x * p1.y;
}

namespace
{
  struct AreaCalculator
  {
    karnauhova::Point& last_point;
    karnauhova::CalcAreaPoint& calculator;
    double operator()(double sum, const karnauhova::Point& current)
    {
      double term = calculator(last_point, current);
      last_point = current;
      return sum + term;
    }
  };
}

std::istream& karnauhova::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  using del =  DelimiterIO;
  in >> del{ '(' } >> point.x >> del{ ';' } >> point.y >> del{ ')' };
  return in;
}

std::istream& karnauhova::operator>>(std::istream& in, Polygon& pol)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t count = 0;
  in >> count;
  if (!in || count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > temp(count);
  std::copy_n(std::istream_iterator< Point >(in), count, temp.begin());
  if (!in || temp.size() != count)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  pol.points = temp;
  return in;
}

double karnauhova::getArea(const Polygon& polygon)
{
  if (polygon.points.size() < 3)
  {
    return 0.0;
  }
  CalcAreaPoint calc;
  Point last = polygon.points.back();
  AreaCalculator acc{last, calc};
  double area = std::accumulate(polygon.points.begin(), polygon.points.end(), 0.0, std::ref(acc));
  return std::abs(area) / 2.0;
}

bool karnauhova::Point::operator==(const Point& rhs) const
{
  return x == rhs.x && y == rhs.y;
}

bool karnauhova::Polygon::operator==(const Polygon& rhs) const
{
  return points == rhs.points;
}
