#include "sub-utils.hpp"
#include <numeric>
#include <algorithm>
#include <string>
#include <exception>
#include <iomanip>
#include <stream-guard.hpp>

namespace
{
  bool compareXPoint(const alymova::Point& point1, const alymova::Point& point2)
  {
    return point1.x < point2.x;
  }

  bool compareYPoint(const alymova::Point& point1, const alymova::Point& point2)
  {
    return point1.y < point2.y;
  }

  bool compareMaxXPolygon(const alymova::Polygon& polygon1, const alymova::Polygon& polygon2)
  {
    return alymova::findMaxXPolygon(polygon1) < alymova::findMaxXPolygon(polygon2);
  }

  bool compareMaxYPolygon(const alymova::Polygon& polygon1, const alymova::Polygon& polygon2)
  {
    return alymova::findMaxYPolygon(polygon1) < alymova::findMaxYPolygon(polygon2);
  }

  bool compareMinXPolygon(const alymova::Polygon& polygon1, const alymova::Polygon& polygon2)
  {
    return alymova::findMinXPolygon(polygon1) < alymova::findMinXPolygon(polygon2);
  }

  bool compareMinYPolygon(const alymova::Polygon& polygon1, const alymova::Polygon& polygon2)
  {
    return alymova::findMinYPolygon(polygon1) < alymova::findMinYPolygon(polygon2);
  }
}

double alymova::areaEven(const std::vector< Polygon >& polygons)
{
  return getAreasIf(polygons, isPolygonEven);
}

double alymova::areaOdd(const std::vector< Polygon >& polygons)
{
  return getAreasIf(polygons, isPolygonOdd);
}

double alymova::areaMean(const std::vector< Polygon >& polygons)
{
  if (polygons.size() == 0)
  {
    throw std::overflow_error("");
  }
  double res = getAreasIf(polygons, noConditional);
  return res / polygons.size();
}

double alymova::areaNumber(size_t vertexes, const std::vector< Polygon >& polygons)
{
  auto bindEqualSize = std::bind(isEqualSize, vertexes, _1);
  return getAreasIf(polygons, bindEqualSize);
}

double alymova::areaPolygon(const Polygon& polygon)
{
  Polygon polygon_rotate(polygon);
  std::rotate(polygon_rotate.points.begin(), polygon_rotate.points.begin() + 1, polygon_rotate.points.end());

  auto begin = polygon.points.begin(), end = polygon.points.end();
  double res = std::inner_product(begin, end, polygon_rotate.points.begin(), 0, std::plus< double >{}, multPoints);
  return std::abs(res) / 2.0;
}

double alymova::multPoints(const Point& point1, const Point& point2)
{
  return point1.x * point2.y - point1.y * point2.x;
}

template< class Predicate >
double alymova::getAreasIf(const std::vector< Polygon >& polygons, Predicate pred)
{
  std::vector< Polygon > suitable;
  std::vector< double > areas;
  std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(suitable), pred);
  std::transform(suitable.begin(), suitable.end(), std::back_inserter(areas), areaPolygon);
  return std::accumulate(areas.begin(), areas.end(), 0.0);
}

bool alymova::compareArea(const Polygon& polygon1, const Polygon& polygon2)
{
  return areaPolygon(polygon1) < areaPolygon(polygon2);
}

bool alymova::compareVertexes(const Polygon& polygon1, const Polygon& polygon2)
{
  return polygon1.points.size() < polygon2.points.size();
}

void alymova::printArea(std::ostream& out, const Polygon& polygon)
{
  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1) << areaPolygon(polygon);
}

void alymova::printVertexes(std::ostream& out, const Polygon& polygon)
{
  out << polygon.points.size();
}

int alymova::findMaxXPolygon(const Polygon& polygon)
{
  auto res = std::max_element(polygon.points.begin(), polygon.points.end(), compareXPoint);
  return (*res).x;
}

int alymova::findMaxYPolygon(const Polygon& polygon)
{
  auto res = std::max_element(polygon.points.begin(), polygon.points.end(), compareYPoint);
  return (*res).y;
}

int alymova::findMinXPolygon(const Polygon& polygon)
{
  auto res = std::min_element(polygon.points.begin(), polygon.points.end(), compareXPoint);
  return (*res).x;
}

int alymova::findMinYPolygon(const Polygon& polygon)
{
  auto res = std::min_element(polygon.points.begin(), polygon.points.end(), compareYPoint);
  return (*res).y;
}

int alymova::findMaxXVector(const std::vector< Polygon >& polygons)
{
  auto res = std::max_element(polygons.begin(), polygons.end(), compareMaxXPolygon);
  return findMaxXPolygon(*res);
}

int alymova::findMaxYVector(const std::vector< Polygon >& polygons)
{
  auto res = std::max_element(polygons.begin(), polygons.end(), compareMaxYPolygon);
  return findMaxYPolygon(*res);
}

int alymova::findMinXVector(const std::vector< Polygon >& polygons)
{
  auto res = std::min_element(polygons.begin(), polygons.end(), compareMinXPolygon);
  return findMinXPolygon(*res);
}

int alymova::findMinYVector(const std::vector< Polygon >& polygons)
{
  auto res = std::min_element(polygons.begin(), polygons.end(), compareMinYPolygon);
  return findMinYPolygon(*res);
}

void alymova::inFrameOutput(std::ostream& out, bool res)
{
  if (res)
  {
    out << "<TRUE>";
    return;
  }
  out << "<FALSE>";
}

bool alymova::haveRightAngles(const Polygon& polygon)
{
  std::vector< Point > sides;
  auto begin = polygon.points.begin(), end = polygon.points.end();
  std::transform(begin + 1, end, begin, std::back_inserter(sides), getSide);
  sides.push_back(getSide(polygon.points.front(), polygon.points.back()));
  sides.push_back(sides.front());

  std::vector< bool > isRightShape;
  std::transform(sides.begin() + 1, sides.end(), sides.begin(), std::back_inserter(isRightShape), isRightAngle);
  return std::accumulate(isRightShape.begin(), isRightShape.end(), 0) > 0;
}

bool alymova::isRightAngle(const Point& point1, const Point& point2)
{
  return (point1.x * point2.x + point1.y * point2.y) == 0;
}

alymova::Point alymova::getSide(const Point& point1, const Point& point2)
{
  return {point1.x - point2.x, point1.y - point2.y};
}

bool alymova::noConditional(const Polygon&)
{
  return true;
}

bool alymova::isEqualSize(size_t size, const Polygon& polygon)
{
  return polygon.points.size() == size;
}

bool alymova::isPolygonEven(const Polygon& polygon)
{
  return polygon.points.size() % 2 == 0;
}

bool alymova::isPolygonOdd(const Polygon& polygon)
{
  return !isPolygonEven(polygon);
}

size_t alymova::getVertexes(const std::string& str)
{
  size_t vertexes = std::stoull(str);
  if (vertexes < 3 || vertexes / str.size() == 0)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  return vertexes;
}
