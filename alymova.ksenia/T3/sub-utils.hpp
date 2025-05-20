#ifndef SUB_COMMANDS_HPP
#define SUB_COMMANDS_HPP
#include "shapes.hpp"
#include <functional>

namespace alymova
{
  using namespace std::placeholders;
  using Predicate = std::function< int(int, const Point& point) >;

  double areaEven(double value, const Polygon& polygon);
  double areaOdd(double value, const Polygon& polygon);
  double areaMean(double value, const Polygon& polygon, size_t size);
  double areaNumber(double value, const Polygon& polygon, size_t vertexes);
  double areaPolygon(const Polygon& polygon);
  double multPoints(const Point& point1, const Point& point2);

  double compareMaxArea(double value, const Polygon& polygon);
  size_t compareMaxVertexes(size_t value, const Polygon& polygon);
  double compareMinArea(double value, const Polygon& polygon);
  size_t compareMinVertexes(size_t value, const Polygon& polygon);

  int compareMaxXPoint(int value, const Point& point);
  int compareMaxYPoint(int value, const Point& point);
  int compareMinXPoint(int value, const Point& point);
  int compareMinYPoint(int value, const Point& point);
  int findMaxMinXYPolygon(int start, const Polygon& polygon, Predicate pred);
  int findMaxMinXYVector(int start, const std::vector< Polygon >& polygons, Predicate pred);

  bool haveRightAngles(const Polygon& polygon);
  bool isRightAngle(const Point& point1, const Point& point2);
  Point getSide(const Point& point1, const Point& point2);

  bool isDigit(char c);
  bool isEqualSize(size_t size, const Polygon& polygon);
  bool isPolygonEven(const Polygon& polygon);
  size_t getVertexes(std::string str);
}
#endif
