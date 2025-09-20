#include "polygon.hpp"
#include <algorithm>
#include <numeric>
#include <vector>
#include <functional>
#include <iterator>
#include <data_input.hpp>
#include "polygon_utils.hpp"

double trukhanov::getArea(const Polygon& polygon)
{
  const auto& pts = polygon.points;
  std::size_t n = pts.size();
  if (n < 3)
  {
    return 0;
  }

  std::vector< double > areas(n-2);

  using namespace std::placeholders;
  auto calc = std::bind(VectorProduct, pts[0], _1, _2);

  std::transform(pts.begin() + 1, pts.end() - 1, pts.begin() + 2, areas.begin(), calc);

  return std::accumulate(areas.begin(), areas.end(), 0.0);
}

bool trukhanov::isEven(const Polygon& polygon)
{
  return polygon.points.size() != 0 && polygon.points.size() % 2 == 0;
}

bool trukhanov::isOdd(const Polygon& polygon)
{
  return polygon.points.size() != 0 && polygon.points.size() % 2 != 0;
}

bool trukhanov::isRight(const Polygon& polygon)
{
  const auto& pts = polygon.points;
  size_t n = pts.size();
  if (n < 3)
  {
    return false;
  }

  std::vector< size_t > indices(n);
  std::iota(indices.begin(), indices.end(), 0);

  std::vector< Angle > angles(pts.size());
  std::transform(indices.begin(), indices.end(), angles.begin(), MakeAngles{pts});

  return std::any_of(angles.begin(), angles.end(), HasRightAngle(polygon.points));
}

bool trukhanov::operator<(const Point& lhs, const Point& rhs)
{
  return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
}

bool trukhanov::operator==(const Point& lhs, const Point& rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

std::istream& trukhanov::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  in >> DelimiterIO{ '(' } >> point.x >> DelimiterIO{ ';' } >> point.y >> DelimiterIO{ ')' };
  return in;
}

std::ostream& trukhanov::operator<<(std::ostream& out, const Point& point)
{
  out << '(' << point.x << ';' << point.y << ')';
  return out;
}

std::istream& trukhanov::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  size_t n = 0;
  in >> n;
  if (!in || n < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  Polygon temp;
  temp.points.reserve(n);

  std::istream_iterator< Point > it(in);
  std::copy_n(it, n, std::back_inserter(temp.points));

  if (temp.points.size() != n || !in)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  polygon = std::move(temp);
  return in;
}

std::ostream& trukhanov::operator<<(std::ostream& out, const Polygon& polygon)
{
  out << polygon.points.size() << ' ';
  std::copy(polygon.points.begin(), polygon.points.end(), std::ostream_iterator< Point >(out, " "));
  return out;
}
