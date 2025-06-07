#include "functional.hpp"
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <sstream>

puzikov::AreaAccumulator::AreaAccumulator(const std::string &p):
  param(p)
{}

double puzikov::AreaAccumulator::operator()(double acc, const puzikov::Polygon &poly) const
{
  if (param == "ODD")
  {
    if (poly.points.size() % 2 == 1)
    {
      acc += calcPolygonArea(poly);
    }
  }
  else if (param == "EVEN")
  {
    if (poly.points.size() % 2 == 0)
    {
      acc += calcPolygonArea(poly);
    }
  }
  else if (param == "MEAN")
  {
    acc += calcPolygonArea(poly);
  }
  else
  {
    std::size_t verticesCount = 0;
    verticesCount = std::stoul(param);

    if (poly.points.size() == verticesCount)
    {
      acc += calcPolygonArea(poly);
    }
  }
  return acc;
}

bool puzikov::VerticesComparator(const Polygon &p1, const Polygon &p2)
{
  return p1.points.size() < p2.points.size();
}

bool puzikov::AreaComparator(const Polygon &p1, const Polygon &p2)
{
  return calcPolygonArea(p1) < calcPolygonArea(p2);
}

puzikov::ShapesAccumulator::ShapesAccumulator(const std::string &p):
  param(p)
{}

double puzikov::ShapesAccumulator::operator()(double acc, const puzikov::Polygon &poly) const
{
  if (param == "ODD")
  {
    if (poly.points.size() % 2 == 1)
    {
      acc++;
    }
  }
  else if (param == "EVEN")
  {
    if (poly.points.size() % 2 == 0)
    {
      acc++;
    }
  }
  else
  {
    std::size_t verticesCount = 0;
    verticesCount = std::stoul(param);

    if (poly.points.size() == verticesCount)
    {
      acc++;
    }
  }
  return acc;
}

puzikov::RmEchoPredicate::RmEchoPredicate(const Polygon &poly):
  ref(poly)
{}

bool puzikov::RmEchoPredicate::operator()(const Polygon &p1, const Polygon &p2)
{
  return (p1 == p2) && (ref == p1);
}

puzikov::TranslatePoint::TranslatePoint(int dx_, int dy_):
  dx(dx_),
  dy(dy_)
{}

puzikov::Point puzikov::TranslatePoint::operator()(const Point &p) const
{
  return Point {p.x + dx, p.y + dy};
}

puzikov::AnyOfShift::AnyOfShift(const PointVec &candidate_, const PointVec &reference_):
  candidate(candidate_),
  reference(reference_)
{}

bool puzikov::AnyOfShift::operator()(std::size_t shift) const
{
  PointVec rotated(candidate.size());
  std::rotate_copy(candidate.begin(), candidate.begin() + shift, candidate.end(), rotated.begin());

  int dx = reference[0].x - rotated[0].x;
  int dy = reference[0].y - rotated[0].y;

  PointVec translated(rotated.size());
  std::transform(rotated.begin(), rotated.end(), translated.begin(), TranslatePoint(dx, dy));

  return std::equal(translated.begin(), translated.end(), reference.begin());
}

puzikov::IsTranslationCongruent::IsTranslationCongruent(const Polygon &reference_):
  reference(reference_)
{}

bool puzikov::IsTranslationCongruent::operator()(const Polygon &poly) const
{
  if (poly.points.size() != reference.points.size())
  {
    return false;
  }

  std::vector< std::size_t > shifts(reference.points.size());
  std::iota(shifts.begin(), shifts.end(), 0);

  if (std::any_of(shifts.begin(), shifts.end(), AnyOfShift(poly.points, reference.points)))
  {
    return true;
  }

  std::vector< Point > reversed(poly.points.rbegin(), poly.points.rend());
  if (std::any_of(shifts.begin(), shifts.end(), AnyOfShift(reversed, reference.points)))
  {
    return true;
  }

  return false;
}

puzikov::Point puzikov::PointGenerator(std::istream &in)
{
  char c = in.peek();
  if (c == '\n' || c == EOF)
  {
    throw std::runtime_error("BUUUH");
  }
  Point p;
  in >> p;
  return p;
}

puzikov::AreaIt puzikov::maxAreaElement(AreaIt first, AreaIt last, AreaComp comp)
{
  return std::max_element(first, last, comp);
}

puzikov::AreaIt puzikov::minAreaElement(AreaIt first, AreaIt last, AreaComp comp)
{
  return std::min_element(first, last, comp);
}

puzikov::VertIt puzikov::maxVertElement(VertIt first, VertIt last, VertComp comp)
{
  return std::max_element(first, last, comp);
}

puzikov::VertIt puzikov::minVertElement(VertIt first, VertIt last, VertComp comp)
{
  return std::min_element(first, last, comp);
}
