#include "processors.hpp"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <functional>
#include <vector>
#include <stdexcept>

namespace rychkov
{
  static auto get_points = std::bind(std::mem_fn(&Polygon::points), std::placeholders::_1);
  static auto get_size = std::bind(&decltype(Polygon::points)::size, get_points);
  static auto iseven = std::bind(std::equal_to<>{}, std::bind(std::modulus<>{}, get_size, 2), 0);
  static auto isodd = std::bind(std::equal_to<>{}, std::bind(std::modulus<>{}, get_size, 2), 1);
}

bool rychkov::AreaProcessor::even(ParserContext& context)
{
  if (!eol(context.in))
  {
    return false;
  }
  using namespace std::placeholders;
  std::vector< double > temp(polygons_.size(), 0);
  auto conditional_area = std::bind(if_statement< double >{}, iseven, std::bind(get_area{}, _1), 0);
  std::transform(polygons_.begin(), polygons_.end(), temp.begin(), conditional_area);
  context.out << std::accumulate(temp.begin(), temp.end(), 0.0) << '\n';
  return true;
}
bool rychkov::AreaProcessor::odd(ParserContext& context)
{
  if (!eol(context.in))
  {
    return false;
  }
  using namespace std::placeholders;
  std::vector< double > temp(polygons_.size(), 0);
  auto conditional_area = std::bind(if_statement< double >{}, isodd, std::bind(get_area{}, _1), 0);
  std::transform(polygons_.begin(), polygons_.end(), temp.begin(), conditional_area);
  context.out << std::accumulate(temp.begin(), temp.end(), 0.0) << '\n';
  return true;
}
bool rychkov::AreaProcessor::count(ParserContext& context)
{
  size_t number = parse_as_count(context);
  if (number == ~0ULL)
  {
    return false;
  }
  if ((number < 3) || !eol(context.in))
  {
    context.parse_error();
    return true;
  }
  using namespace std::placeholders;
  std::vector< double > temp(polygons_.size(), 0);
  auto equal_size = std::bind(std::equal_to<>{}, number, get_size);
  auto conditional_area = std::bind(if_statement< double >{}, equal_size, std::bind(get_area{}, _1), 0);
  std::transform(polygons_.begin(), polygons_.end(), temp.begin(), conditional_area);
  context.out << std::accumulate(temp.begin(), temp.end(), 0.0) << '\n';
  return true;
}
bool rychkov::AreaProcessor::mean(ParserContext& context)
{
  if (polygons_.size() == 0)
  {
    throw std::logic_error("arithmetic mean from 0 elements");
  }
  if (!eol(context.in))
  {
    return false;
  }
  std::vector< double > temp(polygons_.size(), 0);
  std::transform(polygons_.begin(), polygons_.end(), temp.begin(), get_area{});
  context.out << std::accumulate(temp.begin(), temp.end(), 0.0) / polygons_.size() << '\n';
  return true;
}

bool rychkov::CountProcessor::even(ParserContext& context)
{
  if (!eol(context.in))
  {
    return false;
  }
  context.out << std::count_if(polygons_.begin(), polygons_.end(), iseven) << '\n';
  return true;
}
bool rychkov::CountProcessor::odd(ParserContext& context)
{
  if (!eol(context.in))
  {
    return false;
  }
  context.out << std::count_if(polygons_.begin(), polygons_.end(), isodd) << '\n';
  return true;
}
bool rychkov::CountProcessor::count(ParserContext& context)
{
  size_t number = parse_as_count(context);
  if (number == ~0ULL)
  {
    return false;
  }
  if ((number < 3) || !eol(context.in))
  {
    context.parse_error();
    return true;
  }
  auto equal_size = std::bind(std::equal_to<>{}, number, get_size);
  context.out << std::count_if(polygons_.begin(), polygons_.end(), equal_size) << '\n';
  return true;
}

bool rychkov::MaxProcessor::area(ParserContext& context)
{
  if (polygons_.size() == 0)
  {
    throw std::logic_error("max from 0 elements");
  }
  if (!eol(context.in))
  {
    return false;
  }
  std::vector< double > areas(polygons_.size(), 0);
  std::transform(polygons_.begin(), polygons_.end(), areas.begin(), get_area{});
  context.out << *std::max_element(areas.begin(), areas.end()) << '\n';
  return true;
}
bool rychkov::MaxProcessor::count(ParserContext& context)
{
  if (polygons_.size() == 0)
  {
    throw std::logic_error("max from 0 elements");
  }
  if (!eol(context.in))
  {
    return false;
  }
  using namespace std::placeholders;
  static auto compare = std::bind(std::less<>{}, std::bind(get_size, _1), std::bind(get_size, _2));
  context.out << get_size(*std::max_element(polygons_.begin(), polygons_.end(), compare)) << '\n';
  return true;
}
bool rychkov::MinProcessor::area(ParserContext& context)
{
  if (polygons_.size() == 0)
  {
    throw std::logic_error("min from 0 elements");
  }
  if (!eol(context.in))
  {
    return false;
  }
  std::vector< double > areas(polygons_.size(), 0);
  std::transform(polygons_.begin(), polygons_.end(), areas.begin(), get_area{});
  context.out << *std::min_element(areas.begin(), areas.end()) << '\n';
  return true;
}
bool rychkov::MinProcessor::count(ParserContext& context)
{
  if (polygons_.size() == 0)
  {
    throw std::logic_error("min from 0 elements");
  }
  if (!eol(context.in))
  {
    return false;
  }
  using namespace std::placeholders;
  static auto compare = std::bind(std::less<>{}, std::bind(get_size, _1), std::bind(get_size, _2));
  context.out << get_size(*std::min_element(polygons_.begin(), polygons_.end(), compare)) << '\n';
  return true;
}

bool rychkov::MainProcessor::remove_repeates(ParserContext& context)
{
  Polygon polygon;
  if (!(context.in >> polygon) || !eol(context.in))
  {
    return false;
  }
  struct equal
  {
    bool operator()(const Polygon& lhs, const Polygon& rhs)
    {
      using namespace std::placeholders;
      static auto get_x = std::bind(std::mem_fn(&Point::x), _1);
      static auto get_y = std::bind(std::mem_fn(&Point::y), _1);
      static auto x_equal = std::bind(std::equal_to<>{}, std::bind(get_x, _1), std::bind(get_x, _2));
      static auto y_equal = std::bind(std::equal_to<>{}, std::bind(get_y, _1), std::bind(get_y, _2));
      static auto points_equal = std::bind(std::logical_and<>{}, x_equal, y_equal);
      return std::equal(lhs.points.begin(), lhs.points.end(),rhs.points.begin(), points_equal);
    }
  };
  decltype(polygons_)::iterator new_end = std::unique(polygons_.begin(), polygons_.end(), equal{});
  polygons_.erase(new_end, polygons_.end());
  return true;
}
bool rychkov::MainProcessor::rectangles(ParserContext& context)
{
  if (!eol(context.in))
  {
    return false;
  }
  struct is_rect
  {
    bool operator()(const Polygon& p)
    {
      if (p.points.size() != 4)
      {
        return false;
      }
      bool horis1 = (p.points[0].x == p.points[1].x) && (p.points[2].x == p.points[3].x);
      bool vert1 = (p.points[1].y == p.points[2].y) && (p.points[3].y == p.points[0].y);
      bool horis2 = (p.points[1].x == p.points[2].x) && (p.points[3].x == p.points[0].x);
      bool vert2 = (p.points[0].y == p.points[1].y) && (p.points[2].y == p.points[3].y);
      return (horis1 && vert1) || (horis2 && vert2);
    }
  };
  context.out << std::count_if(polygons_.begin(), polygons_.end(), is_rect{}) << '\n';
  return true;
}
double rychkov::get_area::operator()(const Polygon& polygon)
{
  if (polygon.points.size() < 3)
  {
    return 0;
  }
  using namespace std::placeholders;
  area_accumulator accumulator;
  std::vector< double > temp(polygon.points.size(), 0);
  temp[0] = accumulator(polygon.points.back(), polygon.points.front());
  std::transform(polygon.points.begin(), std::prev(polygon.points.end()), std::next(polygon.points.begin()),
        std::next(temp.begin()), accumulator);
  double result = std::accumulate(temp.begin(), temp.end(), 0.0) / 2;
  return (result < 0 ? -result : result);
}
double rychkov::area_accumulator::operator()(const Point& prev, const Point& cur)
{
  double result = prev.x * cur.y - cur.x * prev.y;
  return result;
}
