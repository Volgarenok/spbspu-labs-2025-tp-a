#include "geom.hpp"
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
  struct CalcAreaTerm
  {
    double operator()(const ageev::Point& p1, const ageev::Point& p2) const
    {
      return p1.x * p2.y - p2.x * p1.y;
    }
  };

  struct GetSide
  {
    ageev::Point operator()(const ageev::Point& p1, const ageev::Point& p2) const
    {
      return ageev::Point{p1.x - p2.x, p1.y - p2.y};
    }
  };

  struct IsRightAngle
  {
    bool operator()(const ageev::Point& side1, const ageev::Point& side2) const
    {
      return (side1.x * side2.x + side1.y * side2.y) == 0;
    }
  };

  struct VertexAngleChecker
  {
    explicit VertexAngleChecker(const ageev::Polygon& p): polygon(p) {}
    
    bool operator()(size_t vertex_index) const
    {
      const auto& points = polygon.points;
      const size_t size = points.size();
      const ageev::Point& prev = points[(vertex_index + size - 1) % size];
      const ageev::Point& curr = points[vertex_index];
      const ageev::Point& next = points[(vertex_index + 1) % size];
      
      GetSide get_side;
      IsRightAngle is_right_angle;
      
      const ageev::Point a = get_side(prev, curr);
      const ageev::Point b = get_side(curr, next);
      return is_right_angle(a, b);
    }
    
    const ageev::Polygon& polygon;
  };
}

std::istream& ageev::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sent(in);
  if (!sent)
  {
    return in;
  }
  ageev::StreamGuard stream(in);
  in >> DelimeterIO{ '(' } >> point.x >> DelimeterIO{ ';' } >> point.y >> DelimeterIO{ ')' };
  return in;
}

std::istream& ageev::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sent(in);
  if (!sent)
  {
    return in;
  }
  ageev::StreamGuard stream(in);
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

double ageev::get_area(const Polygon& polygon)
{
  const auto& points = polygon.points;
  if (points.empty())
  {
    return 0.0;
  }
  
  const Point& first = points.front();
  const Point& last = points.back();
  
  CalcAreaTerm calc_term;
  double area = std::inner_product(
    points.begin(), points.end() - 1, 
    points.begin() + 1, 
    calc_term(last, first), 
    std::plus< double >{}, 
    calc_term
  );
  
  return std::abs(area) / 2.0;
}

bool ageev::has_right_angle(const Polygon& polygon)
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