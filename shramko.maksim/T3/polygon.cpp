#include "polygon.hpp"
#include <algorithm>
#include <numeric>
#include <iterator>
#include <vector>
#include <cmath>
#include <streamGuard.hpp>

std::istream& shramko::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char ch;
  in >> ch;
  if (ch != '(')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  in >> point.x;
  in >> ch;
  if (ch != ';')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  in >> point.y;
  in >> ch;
  if (ch != ')')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  return in;
}

std::istream& shramko::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t numPoints = 0;
  in >> numPoints;
  if (numPoints < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  polygon.points.resize(numPoints);
  auto endIter = std::copy_n(std::istream_iterator< Point >(in), numPoints, polygon.points.begin());
  if (endIter != polygon.points.end())
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (in)
  {
    return in;
  }
  in.setstate(std::ios::failbit);
  return in;
}

shramko::Point shramko::operator-(const Point& a, const Point& b)
{
  return {a.x - b.x, a.y - b.y};
}

int shramko::dot(const Point& a, const Point& b)
{
  return a.x * b.x + a.y * b.y;
}

double shramko::getPolygonArea(const Polygon& polygon)
{
  const auto& points = polygon.points;
  size_t n = points.size();
  if (n < 3)
  {
    return 0.0;
  }

  struct XProduct
  {
    double operator()(const Point& p1, const Point& p2) const
    {
      return static_cast<double>(p1.x) * p2.y;
    }
  };

  struct YProduct
  {
    double operator()(const Point& p1, const Point& p2) const
    {
      return static_cast<double>(p1.y) * p2.x;
    }
  };

  double sum1 = std::inner_product(points.begin(), points.end() - 1, points.begin() + 1, 0.0, std::plus<double>{}, XProduct{});
  sum1 += XProduct{}(points.back(), points.front());

  double sum2 = std::inner_product(points.begin(), points.end() - 1, points.begin() + 1, 0.0, std::plus<double>{}, YProduct{});
  sum2 += YProduct{}(points.back(), points.front());

  return 0.5 * std::abs(sum1 - sum2);
}
