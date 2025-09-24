#include "polygon.hpp"
#include <algorithm>
#include <numeric>
#include <iterator>
#include <vector>
#include <cmath>
#include <functional>
#include <streamGuard.hpp>

std::istream& shramko::operator>>(std::istream& in, shramko::Point& point)
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

std::istream& shramko::operator>>(std::istream& in, shramko::Polygon& polygon)
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
  polygon.points.clear();
  polygon.points.resize(numPoints);
  auto endIter = std::copy_n(std::istream_iterator< shramko::Point >(in), numPoints, polygon.points.begin());
  if (endIter != polygon.points.end() || !in)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  auto adjacentEqual = std::bind(std::equal_to< shramko::Point >{}, std::placeholders::_1, std::placeholders::_2);
  if (std::adjacent_find(polygon.points.begin(), polygon.points.end(), adjacentEqual) != polygon.points.end())
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (polygon.points.front() == polygon.points.back())
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  return in;
}

bool shramko::operator==(const shramko::Point& a, const shramko::Point& b)
{
  return a.x == b.x && a.y == b.y;
}

shramko::Point shramko::operator-(const shramko::Point& a, const shramko::Point& b)
{
  return {a.x - b.x, a.y - b.y};
}

int shramko::dot(const shramko::Point& a, const shramko::Point& b)
{
  return a.x * b.x + a.y * b.y;
}

double xProduct(const shramko::Point& p1, const shramko::Point& p2)
{
  return static_cast< double >(p1.x) * p2.y;
}

double yProduct(const shramko::Point& p1, const shramko::Point& p2)
{
  return static_cast< double >(p1.y) * p2.x;
}

double shramko::getPolygonArea(const shramko::Polygon& polygon)
{
  const auto& points = polygon.points;
  size_t n = points.size();
  if (n < 3)
  {
    return 0.0;
  }

  double sum1 = std::inner_product(points.begin(), points.end() - 1, points.begin() + 1, 0.0, std::plus< double >{}, xProduct);
  sum1 += xProduct(points.back(), points.front());

  double sum2 = std::inner_product(points.begin(), points.end() - 1, points.begin() + 1, 0.0, std::plus< double >{}, yProduct);
  sum2 += yProduct(points.back(), points.front());

  return 0.5 * std::abs(sum1 - sum2);
}
