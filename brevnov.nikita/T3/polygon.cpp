#include "polygon.hpp"
#include <algorithm>
#include <cmath>
#include <istream>
#include <iterator>
#include <vector>
#include <numeric>
#include <functional>
#include <streamGuard.hpp>
#include <delimeters.hpp>

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
    brevnov::Point newSide{p1.x - p2.x, p1.y - p2.y};
    return newSide;
  }

  bool is_right_angle(const brevnov::Point &side1, const brevnov::Point &side2)
  {
    return (side1.x * side2.x + side1.y * side2.y) == 0;
  }

  struct VertexAngleChecker
  {
    bool operator()(size_t vertex_index) const
    {
      const auto& points = polygon.points;
      const size_t size = points.size();
      const brevnov::Point& prev = points[(vertex_index + size - 1) % size];
      const brevnov::Point& curr = points[vertex_index];
      const brevnov::Point& next = points[(vertex_index + 1) % size];
      const brevnov::Point a = get_side(prev, curr);
      const brevnov::Point b = get_side(curr, next);
      return is_right_angle(a, b);
    }
    const brevnov::Polygon& polygon;
  };
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
    points.begin(), points.end() - 1, points.begin() + 1, Calc_area_term{}(last, first), std::plus< double >{}, Calc_area_term{});
  return std::abs(area) / 2.0;
}

bool brevnov::has_right_angle(const Polygon& polygon)
{
  if (polygon.points.size() < 3)
  {
    return false;
  }
  std::vector< size_t > indices(polygon.points.size());
  std::iota(indices.begin(), indices.end(), 0);
  VertexAngleChecker checker{polygon};
  return std::any_of(indices.begin(), indices.end(), checker);
}
