#include "polygon.hpp"
#include <algorithm>
#include <iterator>
#include <numeric>
#include <vector>

bool shchadilov::Point::operator==(const Point& other) const
{
  return x == other.x && y == other.y;
}

bool shchadilov::Polygon::operator==(const Polygon& other) const
{
  if (points.size() != other.points.size()) return false;
  return std::is_permutation(points.begin(), points.end(), other.points.begin());
}

std::istream& shchadilov::operator>>(std::istream& in, Point& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{ '(' } >> dest.x >> DelimiterIO{ ';' } >> dest.y >> DelimiterIO{ ')' };
  return in;
}

std::istream& shchadilov::operator>>(std::istream& in, Polygon& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t count = 0;
  in >> count;
  if (count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > points;
  points.reserve(count);
  std::copy_n(std::istream_iterator< Point >(in), count, std::back_inserter(points));
  if (in && points.size() == count)
  {
    dest.points = std::move(points);
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

struct VectorProduct
{
  double operator()(const shchadilov::Point& a, const shchadilov::Point& b) const
  {
    return a.x * b.y - b.x * a.y;
  }
};

struct AreaCalculator
{
  double operator()(const shchadilov::Polygon& poly) const
  {
    const auto& points = poly.points;
    if (points.size() < 3)
    {
      return 0.0;
    }

    std::vector< shchadilov::Point > shifted_points;
    shifted_points.reserve(points.size());
    std::copy(points.begin() + 1, points.end(), std::back_inserter(shifted_points));
    shifted_points.push_back(points[0]);

    double area = std::inner_product(
      points.begin(), points.end(), shifted_points.begin(), 0.0,
      std::plus<double>(), VectorProduct());

    return std::abs(area) / 2.0;
  }
};

double shchadilov::getArea(const Polygon& poly)
{
  return AreaCalculator()(poly);
}
