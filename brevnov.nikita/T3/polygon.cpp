#include "polygon.hpp"
#include <algorithm>
#include <cmath>
#include <istream>
#include <iterator>
#include <vector>
#include <numeric>
#include <functional>
#include "streamGuard.hpp"
#include "delimeters.hpp"

namespace
{
  struct Calc_area_term
  {
    double operator()(const brevnov::Point& p1, const brevnov::Point& p2)
    {
      return p1.x * p2.y - p2.x * p1.y;
    }
  };

  brevnov::Point get_side(const brevnov::Point &p1, const brevnov::Point &p2)
  {
    brevnov::Point newSide;
    newSide.x = p1.x - p2.x;
    newSide.y = p1.y - p2.y;
    return newSide;
  }

  bool is_right_angle(const brevnov::Point &side1, const brevnov::Point &side2)
  {
    return (side1.x * side2.x + side1.y * side2.y) == 0;
  }

  bool is_true(bool a)
  {
    return a != 0;
  }
}

std::istream& brevnov::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sent(in);
  if (!sent)
  {
    return in;
  }
  brevnov::StreamGuard stream(in);
  in >> DelimeterIO{ '(' } >> point.x >> DelimeterIO{ ';' } >> point.y >> DelimeterIO{ ')' };
  return in;
}


std::istream& brevnov::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sent(in);
  if (!sent)
  {
    return in;
  }
  brevnov::StreamGuard stream(in);
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

double brevnov::get_area(const Polygon& polygon)
{
  const auto points = polygon.points;
  const Point first = points.front();
  const Point last = points.back();
  double area = std::inner_product(
    points.begin(), points.end() - 1, points.begin() + 1, Calc_area_term()(last, first), std::plus< double >(), Calc_area_term());
  return std::abs(area) / 2.0;
}

bool brevnov::has_right_angle(const Polygon& polygon)
{
  std::vector<Point> sides(polygon.points.size());
  auto polygon_begin = polygon.points.cbegin();
  std::transform(polygon_begin + 1, polygon.points.cend(), polygon_begin, sides.begin(), get_side);
  sides[polygon.points.size() - 1] = get_side(polygon.points[0], polygon.points[polygon.points.size() - 1]);
  std::vector<bool> has_right_angle_vector(polygon.points.size());
  std::transform(sides.cbegin() + 1, sides.cend(), sides.cbegin(), has_right_angle_vector.begin(), is_right_angle);
  has_right_angle_vector[sides.size() - 1] = is_right_angle(sides[0], sides[sides.size() - 1]);
  return std::any_of(has_right_angle_vector.cbegin(), has_right_angle_vector.cend(), is_true);
}
