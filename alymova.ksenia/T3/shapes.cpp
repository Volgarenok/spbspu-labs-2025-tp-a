#include "shapes.hpp"
#include <algorithm>
#include <iterator>
#include <functional>
#include <string>
#include <delimiter-io.hpp>

std::istream& alymova::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  Point tested;
  in >> DelimiterIO{'('} >> tested.x >> DelimiterIO{';'} >> tested.y >> DelimiterIO{')'};
  if (in)
  {
    point = tested;
  }
  return in;
}

std::istream& alymova::operator>>(std::istream& in, Polygon& polygon)
{
  using namespace std::placeholders;

  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  size_t cnt_points;
  in >> cnt_points;
  if (!in || cnt_points < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > tested;
  auto it_find = std::find_if(
    std::istream_iterator< Point >(in),
    std::istream_iterator< Point >(),
    std::bind(insertIfNotNextEnter, std::ref(in), _1, std::ref(tested))
  );
  if (in)
  {
    tested.push_back(*it_find);
  }
  if (tested.size() == cnt_points)
  {
    polygon.points = std::move(tested);
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

bool alymova::insertIfNotNextEnter(std::istream& in, const Point& point, std::vector< Point >& tested)
{
  char c = in.peek();
  if (c != '\n')
  {
    tested.push_back(point);
    return false;
  }
  return true;
}
