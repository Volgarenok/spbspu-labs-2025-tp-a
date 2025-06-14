#include "GeometricalTypes.hpp"
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <cmath>
#include <delimiter.hpp>
#include "polygonfunctors.hpp"

using check = shapkov::DelimiterIO;

std::istream& shapkov::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  double x = 0, y = 0;
  in >> check{ '(' };
  in >> x;
  in >> check{ ';' };
  in >> y;
  in >> check{ ')' };
  if (!in)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  point = { x, y };
  return in;
}

std::ostream& shapkov::operator<<(std::ostream& out, const Point& point)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  return (out << " (" << point.x << ';' << point.y << ')');
}

std::istream& shapkov::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  };
  size_t vertexes = 0;
  if (!(in >> vertexes) || vertexes < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > points(vertexes, Point{ 0, 0 });
  using inputIt = std::istream_iterator< Point >;
  std::copy_n(inputIt{ in }, vertexes, points.begin());
  if (in && points.size() == vertexes)
  {
    polygon.points = points;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::ostream& shapkov::operator<<(std::ostream& out, const Polygon& polygon)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  out << polygon.points.size();
  using outputIt = std::ostream_iterator< Point >;
  std::copy(std::begin(polygon.points), std::end(polygon.points), outputIt{ out, "" });
  return out;
}

double shapkov::getDistance(const Point& p1, const Point& p2)
{
  return std::sqrt(std::pow((p1.x - p2.x), 2) + std::pow((p1.y - p2.y), 2));
}

double shapkov::getAreaOfTriangle(const Polygon& p)
{
  double sideA = getDistance(p.points[0], p.points[1]);
  double sideB = getDistance(p.points[2], p.points[1]);
  double sideC = getDistance(p.points[0], p.points[2]);
  double halfPerimeter = (sideA + sideB + sideC) / 2;
  return std::sqrt(halfPerimeter * (halfPerimeter - sideA) * (halfPerimeter - sideB) * (halfPerimeter - sideC));
}

shapkov::Polygon shapkov::makeTriangle(size_t i, const std::vector< Point >& pts)
{
  return Polygon{ std::vector<Point>{ pts[0], pts[i + 1], pts[i + 2] } };
}

shapkov::Polygon shapkov::TriangleGenerator::operator()()
{
  return shapkov::makeTriangle(index++, points);
}

std::vector< shapkov::Polygon > shapkov::polygonToTriangles(const Polygon& p)
{
  size_t size = p.points.size() - 2;
  std::vector< Polygon > triangles(size);
  size_t index = 0;
  std::generate(triangles.begin(), triangles.end(), TriangleGenerator{ index, p.points });
  return triangles;
}

double shapkov::getArea(const Polygon& p)
{
  std::vector< Polygon > triangles = polygonToTriangles(p);
  std::vector< double > areas;
  std::transform(triangles.begin(), triangles.end(), std::back_inserter(areas), getAreaOfTriangle);
  return std::accumulate(areas.begin(), areas.end(), 0.0);
}

bool shapkov::isEven(const Polygon& p)
{
  return p.points.size() != 0 && p.points.size() % 2 == 0;
}

bool shapkov::isOdd(const Polygon& p)
{
  return p.points.size() != 0 && p.points.size() % 2 != 0;
}

bool shapkov::compareByVertexes(const Polygon& p1, const Polygon& p2)
{
  return p1.points.size() < p2.points.size();
}

bool shapkov::isRectangle(const Polygon& p)
{
  if (p.points.size() != 4)
  {
    return false;
  }
  double sideA = getDistance(p.points[0], p.points[1]);
  double sideB = getDistance(p.points[2], p.points[1]);
  double sideC = getDistance(p.points[3], p.points[2]);
  double sideD = getDistance(p.points[3], p.points[0]);
  double diagAC = getDistance(p.points[0], p.points[2]);
  double diagAC1 = std::sqrt(std::pow((sideA), 2) + std::pow((sideB), 2));
  return (compareDouble(diagAC, diagAC1) && compareDouble(sideA, sideC) && compareDouble(sideB, sideD));
}
