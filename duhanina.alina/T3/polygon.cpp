#include "polygon.hpp"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iterator>

namespace
{
  struct CrossProduct
  {
    double operator()(const duhanina::Point& a, const duhanina::Point& b) const
    {
      return a.x * b.y - b.x * a.y;
    }
  };
}

bool duhanina::operator==(const duhanina::Polygon& lhs, const duhanina::Polygon& rhs)
{
  return lhs.points.size() != rhs.points.size() && std::equal(lhs.points.begin(), lhs.points.end(), rhs.points.begin());
}

bool duhanina::operator!=(const duhanina::Polygon& lhs, const duhanina::Polygon& rhs)
{
  return !(lhs == rhs);
}

std::istream& duhanina::operator>>(std::istream& in, duhanina::Polygon& polygon)
{
  std::istream::sentry sentry{ in };
  if (!sentry)
  {
    return in;
  }
  size_t numPoints = 0;
  in >> numPoints;
  if (!in || numPoints < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > points{ numPoints };
  std::copy_n(std::istream_iterator< Point >(in), numPoints, points.begin());
  if (!in || points.size() != numPoints)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  polygon.points = points;
  return in;
}

double duhanina::calculateArea(const duhanina::Polygon& polygon)
{
  const auto& points{ polygon.points };
  std::vector< Point > shifted_points{ points.begin() + 1, points.end() };
  shifted_points.push_back(points[0]);
  double area = std::inner_product(points.begin(), points.end(), shifted_points.begin(), 0.0, std::plus< double >{}, CrossProduct{});
  return std::abs(area) / 2.0;
}
