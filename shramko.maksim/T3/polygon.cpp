#include "polygon.hpp"
#include <algorithm>
#include <numeric>
#include <iterator>
#include <vector>
#include <cmath>
#include <functional>
#include <streamGuard.hpp>
#include <set>

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

bool shramko::operator<(const shramko::Point& a, const shramko::Point& b)
{
  return a.x < b.x || (a.x == b.x && a.y < b.y);
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
  if (numPoints < 3 || in.fail())
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  polygon.points.clear();
  polygon.points.resize(numPoints);

  std::copy_n(std::istream_iterator< shramko::Point >(in), numPoints, polygon.points.begin());

  if (!in)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::set<shramko::Point> uniquePoints(polygon.points.begin(), polygon.points.end());
  if (uniquePoints.size() != polygon.points.size())
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

long long shramko::dot(const shramko::Point& a, const shramko::Point& b)
{
  return static_cast< long long >(a.x) * b.x + static_cast< long long >(a.y) * b.y;
}

double shramko::getPolygonArea(const shramko::Polygon& polygon)
{
  const auto& points = polygon.points;
  size_t n = points.size();
  if (n < 3)
  {
    return 0.0;
  }

  double sum = 0.0;
  for (size_t i = 0; i < n; ++i)
  {
    const auto& current = points[i];
    const auto& next = points[(i + 1) % n];
    sum += static_cast< double >(current.x) * next.y - static_cast< double >(next.x) * current.y;
  }
  return std::abs(sum) / 2.0;
}
