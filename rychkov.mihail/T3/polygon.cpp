#include "polygon.hpp"

#include <iostream>
#include <iterator>
#include <limits>
#include <algorithm>
#include <string>
#include <sstream>
#include <utility>
#include <skip_any_of.hpp>
#include "parser.hpp"

std::istream& rychkov::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return in >> iofmt::anyof("(") >> point.x >> iofmt::anyof(";") >> point.y >> iofmt::anyof(")");
}
std::istream& rychkov::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t count = 0;
  if (!(in >> count) || (count < 3))
  {
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    in.setstate(std::ios::failbit);
    return in;
  }
  Polygon result;
  result.points.reserve(count);
  std::string line;
  std::getline(in, line);
  std::istringstream line_stream{line};
  std::copy_n(std::istream_iterator< Point >{line_stream}, count, std::back_inserter(result.points));
  if (!line_stream || !eol(line_stream))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  polygon = std::move(result);
  return in;
}
