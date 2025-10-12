#include "functional.hpp"
#include <algorithm>
#include <numeric>

double puzikov::AreaAccumulator::oddSubcommand(const Polygon &poly)
{
  if (poly.points.size() % 2 == 1)
  {
    return calcPolygonArea(poly);
  }
  return 0;
}

double puzikov::AreaAccumulator::evenSubcommand(const Polygon &poly)
{
  if (poly.points.size() % 2 == 0)
  {
    return calcPolygonArea(poly);
  }
  return 0;
}

double puzikov::AreaAccumulator::meanSubcommand(const Polygon &poly)
{
  return calcPolygonArea(poly);
}

puzikov::AreaAccumulator::AreaAccumulator(const std::string &p):
  param(p)
{
  commands = {{"ODD", oddSubcommand}, {"EVEN", evenSubcommand}, {"MEAN", meanSubcommand}};
}

double puzikov::AreaAccumulator::operator()(double acc, const Polygon &poly) const
{
  if (commands.count(param))
  {
    acc += commands.at(param)(poly);
  }
  else
  {
    size_t verticesCount = 0;
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
{
  commands = {{"ODD", oddSubcommand}, {"EVEN", evenSubcommand}};
}

double puzikov::ShapesAccumulator::evenSubcommand(const Polygon &poly)
{
  return (poly.points.size() % 2 == 0);
}

double puzikov::ShapesAccumulator::oddSubcommand(const Polygon &poly)
{
  return (poly.points.size() % 2 == 1);
}

double puzikov::ShapesAccumulator::operator()(double acc, const Polygon &poly) const
{
  if (commands.count(param))
  {
    acc += commands.at(param)(poly);
  }
  else
  {
    size_t verticesCount = 0;
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

puzikov::TranslatePoint::TranslatePoint(int dx, int dy):
  dx_(dx),
  dy_(dy)
{}

puzikov::Point puzikov::TranslatePoint::operator()(const Point &p) const
{
  return Point{p.x + dx_, p.y + dy_};
}

puzikov::AnyOfShift::AnyOfShift(const PointVec &candidate_, const PointVec &reference_):
  candidate(candidate_),
  reference(reference_)
{}

bool puzikov::AnyOfShift::operator()(size_t shift) const
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

  std::vector< size_t > shifts(reference.points.size());
  std::iota(shifts.begin(), shifts.end(), 0);

  if (std::any_of(shifts.begin(), shifts.end(), AnyOfShift(poly.points, reference.points)))
  {
    return true;
  }

  std::vector< Point > reversed(poly.points.rbegin(), poly.points.rend());
  return std::any_of(shifts.begin(), shifts.end(), AnyOfShift(reversed, reference.points));
}

puzikov::constPolygonVecIt puzikov::maxAreaElement(constPolygonVecIt first, constPolygonVecIt last, comparator comp)
{
  return std::max_element(first, last, comp);
}

puzikov::constPolygonVecIt puzikov::minAreaElement(constPolygonVecIt first, constPolygonVecIt last, comparator comp)
{
  return std::min_element(first, last, comp);
}

puzikov::constPolygonVecIt puzikov::maxVertElement(constPolygonVecIt first, constPolygonVecIt last, comparator comp)
{
  return std::max_element(first, last, comp);
}

puzikov::constPolygonVecIt puzikov::minVertElement(constPolygonVecIt first, constPolygonVecIt last, comparator comp)
{
  return std::min_element(first, last, comp);
}
