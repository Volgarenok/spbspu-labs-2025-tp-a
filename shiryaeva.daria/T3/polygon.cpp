#include "polygon.hpp"
#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>

std::istream& shiryaeva::operator>>(std::istream& in, Point& point)
{
  Point temp{0, 0};
  in >> DelimiterIO{'('} >> temp.x >> DelimiterIO{';'} >> temp.y >> DelimiterIO{')'};
  if (in)
  {
    point = temp;
  }
  return in;
}

std::istream& shiryaeva::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  size_t vertexCount = 0;
  in >> vertexCount;

  if (!in || vertexCount < MIN_VERTEX_COUNT)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  using input_it_t = std::istream_iterator< Point >;
  std::vector< Point > points;
  std::copy_n(input_it_t{in}, vertexCount, std::back_inserter(points));
  if (!in || points.size() != vertexCount)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::sort(points.begin(), points.end());
  if (std::unique(points.begin(), points.end()) != points.end())
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  polygon.points = points;
  return in;
}

double shiryaeva::getArea(const Polygon& polygon)
{
  const std::vector<Point>& points = polygon.points;
  std::size_t n = points.size();

  if (n < MIN_VERTEX_COUNT)
  {
    return 0.0;
  }

  std::vector< double > products(n);
  VectorProduct vectorProduct;

  std::transform(points.begin(), points.end() - 1, points.begin() + 1, products.begin(), vectorProduct);
  products[n - 1] = vectorProduct(points[n - 1], points[0]);

  double area = std::accumulate(products.begin(), products.end(), 0.0);
  return std::abs(area) / 2.0;
}

bool shiryaeva::operator==(const Point& lhs, const Point& rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool shiryaeva::operator<(const Point& lhs, const Point& rhs)
{
  return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
}
