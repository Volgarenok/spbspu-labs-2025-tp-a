#include "polygon.hpp"
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include "DelimIO.hpp"

namespace
{
  int sumPointPairForArea(const holodilov::Point& point, const holodilov::Point& prevPoint)
  {
    return point.y * prevPoint.x - point.x * prevPoint.y;
  }
}

double holodilov::Polygon::getArea() const
{
  if (points.size() < 3)
  {
    return 0;
  }
  std::vector< int > summands(points.size());
  std::transform(points.begin() + 1, points.end(), points.begin(), summands.begin(), sumPointPairForArea);
  summands.push_back(points[points.size() - 1].x * points[0].y - points[points.size() - 1].y * points[0].x);
  return std::abs(std::accumulate(summands.begin(), summands.end(), 0)) / 2.0;
}

bool holodilov::Point::operator==(const Point& rhs) const
{
  return x == rhs.x && y == rhs.y;
}

bool holodilov::Polygon::operator==(const Polygon& rhs) const
{
  return std::equal(points.begin(), points.end(), rhs.points.begin());
}

std::istream& holodilov::operator>>(std::istream& is, Point& point)
{
  std::istream::sentry sentry(is);
  if (!sentry)
  {
    return is;
  }

  Point pointTemp{ 0, 0 };
  is >> DelimIO{ '(' };
  is >> pointTemp.x >> DelimIO{ ';' };
  is >> pointTemp.y >> DelimIO{ ')' };
  if (is)
  {
    point = pointTemp;
  }
  return is;
}

std::istream& holodilov::operator>>(std::istream& is, Polygon& polygon)
{
  std::istream::sentry sentry(is);
  if (!sentry)
  {
    return is;
  }

  size_t amountPoints = 0;
  is >> amountPoints;
  if (!is)
  {
    return is;
  }

  Polygon polygonTemp;
  using istreamIter = std::istream_iterator< Point >;
  std::copy_n(istreamIter(is), amountPoints, std::back_inserter(polygonTemp.points));
  if (!is)
  {
    return is;
  }
  if ((polygonTemp.points.size() != amountPoints) || (polygonTemp.points.size() < 3))
  {
    is.setstate(std::ios::failbit);
  }
  if (is)
  {
    polygon = polygonTemp;
  }
  return is;
}
