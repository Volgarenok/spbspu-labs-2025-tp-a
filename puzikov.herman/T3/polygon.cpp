#include "polygon.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <format_guard.hpp>
#include <input_wrapper_structs.hpp>

std::istream &puzikov::operator>>(std::istream &in, Point &dest)
{
  std::istream::sentry sentry(in);
  using delim = input::Character;
  if (!sentry)
  {
    return in;
  }

  Point temp{0, 0};
  in >> delim{'('} >> temp.x >> delim{';'} >> temp.y >> delim{')'};
  if (in)
  {
    dest = temp;
  }
  return in;
}

std::ostream &puzikov::operator<<(std::ostream &out, const Point &src)
{
  std::ostream::sentry sentry(out);
  if (sentry)
  {
    FormatGuard guard(out);
    out << '(' << src.x << ';' << src.y << ')';
  }
  return out;
}

std::istream &puzikov::operator>>(std::istream &in, Polygon &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  size_t vertexCount;
  in >> vertexCount;
  if (!in || vertexCount < 3)
  {
    throw std::logic_error("Not enough vertices.");
  }
  std::vector< Point > points(vertexCount, Point{0, 0});
  using inputIt = std::istream_iterator< Point >;
  std::copy_n(inputIt{in}, vertexCount, points.begin());
  if (in && points.size() == vertexCount)
  {
    dest.points = points;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::ostream &puzikov::operator<<(std::ostream &out, const puzikov::Polygon &src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  puzikov::FormatGuard guard(out);
  using output_it_t = std::ostream_iterator< Point >;

  out << src.points.size() << ' ';
  std::copy(src.points.begin(), src.points.end(), output_it_t{out, " "});
  return out;
}

bool puzikov::operator==(const Point &p1, const Point &p2)
{
  return ((p1.x == p2.x) && (p1.y == p2.y));
}

bool puzikov::operator==(const Polygon &p1, const Polygon &p2)
{
  if (p1.points.size() != p2.points.size())
  {
    return false;
  }
  return std::equal(p1.points.begin(), p1.points.end(), p2.points.begin());
}

bool puzikov::operator!=(const Polygon &p1, const Polygon &p2)
{
  return !(p1 == p2);
}

puzikov::PolygonAreaAccumulator::PolygonAreaAccumulator(const Polygon &p):
  poly(p),
  n(p.points.size())
{}

double puzikov::PolygonAreaAccumulator::operator()(double acc, const Point &p1) const
{
  size_t i = &p1 - &poly.points[0];
  const Point &p2 = poly.points[(i + 1) % n];
  return acc + (p1.x * p2.y - p2.x * p1.y);
}

double puzikov::calcPolygonArea(const Polygon &poly)
{
  if (poly.points.size() < 3)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  double area = std::accumulate(poly.points.begin(), poly.points.end(), 0.0, PolygonAreaAccumulator(poly)) / 2.0;

  return std::abs(area);
}

void puzikov::readPolygons(std::istream &in, std::vector< Polygon > &polygons)
{
  using polygonInputIter = std::istream_iterator< Polygon >;
  while (!in.eof())
  {
    try
    {
      std::copy(polygonInputIter{in}, polygonInputIter{}, std::back_inserter(polygons));
      if (in.fail())
      {
        throw std::exception();
      }
    }
    catch (...)
    {
      if (in.fail())
      {
        in.clear(in.rdstate() ^ std::ios::failbit);
      }
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
