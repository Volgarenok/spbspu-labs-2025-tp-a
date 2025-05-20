#include "sub-utils.hpp"
#include <numeric>
#include <algorithm>
#include <string>

double alymova::areaEven(double value, const Polygon& polygon)
{
  if (isPolygonEven(polygon))
  {
    return value + areaPolygon(polygon);
  }
  return value;
}

double alymova::areaOdd(double value, const Polygon& polygon)
{
  if (!isPolygonEven(polygon))
  {
    return value + areaPolygon(polygon);
  }
  return value;
}

double alymova::areaMean(double value, const Polygon& polygon, size_t size)
{
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
  std::rotate(
    polygon_rotate.points.begin(),
    polygon_rotate.points.begin() + 1,
    polygon_rotate.points.end()
  );
  double res = std::inner_product(
    polygon.points.begin(),
    polygon.points.end(),
    polygon_rotate.points.begin(),
    0,
    std::plus< double >{},
    multPoints
  );
  return std::abs(res) / 2.0;
}

double alymova::multPoints(const Point& point1, const Point& point2)
{
  return point1.x * point2.y - point1.y * point2.x;
}

double alymova::compareMaxArea(double value, const Polygon& polygon)
{
  return std::max(value, areaPolygon(polygon));
}

size_t alymova::compareMaxVertexes(size_t value, const Polygon& polygon)
{
  return std::max(value, polygon.points.size());
}

double alymova::compareMinArea(double value, const Polygon& polygon)
{
  return std::min(value, areaPolygon(polygon));
}

size_t alymova::compareMinVertexes(size_t value, const Polygon& polygon)
{
  return std::min(value, polygon.points.size());
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

int alymova::findMaxMinXYPolygon(int start, const Polygon& polygon, Predicate pred)
{
  return std::accumulate(polygon.points.begin(), polygon.points.end(), start, pred);
}

int alymova::findMaxMinXYVector(int start, const std::vector< Polygon >& polygons, Predicate pred)
{
  return std::accumulate(polygons.begin(), polygons.end(), start,
    std::bind(findMaxMinXYPolygon, _1, _2, pred));
}

bool alymova::haveRightAngles(const Polygon& polygon)
{
  std::vector< Point > sides;
  std::transform(
    polygon.points.begin() + 1,
    polygon.points.end(),
    polygon.points.begin(),
    std::back_inserter(sides),
    getSide
  );
  sides.push_back(getSide(polygon.points.front(), polygon.points.back()));
  sides.push_back(sides.front());
  std::vector< bool > isRightShape;
  std::transform(
    sides.begin() + 1,
    sides.end(),
    sides.begin(),
    std::back_inserter(isRightShape),
    isRightAngle
  );
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

size_t alymova::getVertexes(std::string str)
{
  if (!std::all_of(str.begin(), str.end(), isDigit))
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  size_t vertexes = std::stoull(str);
  if (vertexes < 3)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  return vertexes;
}
