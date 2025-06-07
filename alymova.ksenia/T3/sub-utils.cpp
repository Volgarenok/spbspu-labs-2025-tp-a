#include "sub-utils.hpp"
#include <numeric>
#include <algorithm>
#include <string>
#include <exception>

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

double alymova::compareArea(Comparator< double > cmp, double value, const Polygon& polygon)
{
  return cmp(value, areaPolygon(polygon));
}

size_t alymova::compareVertexes(Comparator< size_t > cmp, size_t value, const Polygon& polygon)
{
  return cmp(value, polygon.points.size());
}

int alymova::compareMaxXPoint(int value, const Point& point)
{
  return std::max(value, point.x);
}

int alymova::compareMaxYPoint(int value, const Point& point)
{
  return std::max(value, point.y);
}

int alymova::compareMinXPoint(int value, const Point& point)
{
  return std::min(value, point.x);
}

int alymova::compareMinYPoint(int value, const Point& point)
{
  return std::min(value, point.y);
}

int alymova::findMaxMinXYPolygon(PredicatePoint pred, int start, const Polygon& polygon)
{
  return std::accumulate(polygon.points.begin(), polygon.points.end(), start, pred);
}

int alymova::findMaxMinXYVector(PredicatePoint pred, int start, const std::vector< Polygon >& polygons)
{
  auto bindFind = std::bind(findMaxMinXYPolygon, pred, _1, _2);
  return std::accumulate(polygons.begin(), polygons.end(), start, bindFind);
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
