#include "sub-utils.hpp"
#include <numeric>
#include <algorithm>
#include <string>
#include <exception>

namespace
{
  int compareMaxXPoint(int value, const alymova::Point& point)
  {
    return std::max(value, point.x);
  }

  int compareMaxYPoint(int value, const alymova::Point& point)
  {
    return std::max(value, point.y);
  }

  int compareMinXPoint(int value, const alymova::Point& point)
  {
    return std::min(value, point.x);
  }

  int compareMinYPoint(int value, const alymova::Point& point)
  {
    return std::min(value, point.y);
  }
}

double alymova::areaEven(double value, const Polygon& polygon, size_t size)
{
  if (size == 0)
  {
    return value;
  }
  if (isPolygonEven(polygon))
  {
    return value + areaPolygon(polygon);
  }
  return value;
}

double alymova::areaOdd(double value, const Polygon& polygon, size_t size)
{
  if (size == 0)
  {
    return value;
  }
  if (!isPolygonEven(polygon))
  {
    return value + areaPolygon(polygon);
  }
  return value;
}

double alymova::areaMean(double value, const Polygon& polygon, size_t size)
{
  if (size == 0)
  {
    throw std::overflow_error("");
  }
  return value + areaPolygon(polygon) / size;
}

double alymova::areaNumber(double value, const Polygon& polygon, size_t vertexes)
{
  if (polygon.points.size() == vertexes)
  {
    return value + areaPolygon(polygon);
  }
  return value;
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

double alymova::maxArea(double value, const Polygon& polygon)
{
  return std::max(value, areaPolygon(polygon));
}

double alymova::minArea(double value, const Polygon& polygon)
{
  return std::min(value, areaPolygon(polygon));
}

size_t alymova::maxVertexes(size_t value, const Polygon& polygon)
{
  return std::max(value, polygon.points.size());
}

size_t alymova::minVertexes(size_t value, const Polygon& polygon)
{
  return std::min(value, polygon.points.size());
}

int alymova::findMaxXPolygon(int start, const Polygon& polygon)
{
  return std::accumulate(polygon.points.begin(), polygon.points.end(), start, compareMaxXPoint);
}

int alymova::findMaxYPolygon(int start, const Polygon& polygon)
{
  return std::accumulate(polygon.points.begin(), polygon.points.end(), start, compareMaxYPoint);
}

int alymova::findMinXPolygon(int start, const Polygon& polygon)
{
  return std::accumulate(polygon.points.begin(), polygon.points.end(), start, compareMinXPoint);
}

int alymova::findMinYPolygon(int start, const Polygon& polygon)
{
  return std::accumulate(polygon.points.begin(), polygon.points.end(), start, compareMinYPoint);
}

int alymova::findMaxXVector(int start, const std::vector< Polygon >& polygons)
{
  return std::accumulate(polygons.begin(), polygons.end(), start, findMaxXPolygon);
}

int alymova::findMaxYVector(int start, const std::vector< Polygon >& polygons)
{
  return std::accumulate(polygons.begin(), polygons.end(), start, findMaxYPolygon);
}

int alymova::findMinXVector(int start, const std::vector< Polygon >& polygons)
{
  return std::accumulate(polygons.begin(), polygons.end(), start, findMinXPolygon);
}

int alymova::findMinYVector(int start, const std::vector< Polygon >& polygons)
{
  return std::accumulate(polygons.begin(), polygons.end(), start, findMinYPolygon);
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

bool alymova::isDigit(char c)
{
  return std::isdigit(c);
}

bool alymova::isEqualSize(size_t size, const Polygon& polygon)
{
  return size == polygon.points.size();
}

bool alymova::isPolygonEven(const Polygon& polygon)
{
  return polygon.points.size() % 2 == 0;
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
