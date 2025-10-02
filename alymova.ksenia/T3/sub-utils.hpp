#ifndef SUB_UTILS_HPP
#define SUB_UTILS_HPP
#include <functional>
#include "shapes.hpp"

namespace alymova
{
  using namespace std::placeholders;

  double areaEven(const std::vector< Polygon >& polygons);
  double areaOdd(const std::vector< Polygon >& polygons);
  double areaMean(const std::vector< Polygon >& polygons);
  double areaNumber(size_t vertexes, const std::vector< Polygon >& polygons);
  double areaPolygon(const Polygon& polygon);
  double multPoints(const Point& point1, const Point& point2);
  template< class Predicate >
  double getAreasIf(const std::vector< Polygon >& polygons, Predicate pred);

  bool compareArea(const Polygon& polygon1, const Polygon& polygon2);
  bool compareVertexes(const Polygon& polygon1, const Polygon& polygon2);
  void printArea(std::ostream& out, const Polygon& polygon);
  void printVertexes(std::ostream& out, const Polygon& polygon);

  int findMaxXPolygon(const Polygon& polygon);
  int findMaxYPolygon(const Polygon& polygon);
  int findMinXPolygon(const Polygon& polygon);
  int findMinYPolygon(const Polygon& polygon);

  int findMaxXVector(const std::vector< Polygon >& polygons);
  int findMaxYVector(const std::vector< Polygon >& polygons);
  int findMinXVector(const std::vector< Polygon >& polygons);
  int findMinYVector(const std::vector< Polygon >& polygons);
  void inFrameOutput(std::ostream& out, bool res);

  bool haveRightAngles(const Polygon& polygon);
  bool isRightAngle(const Point& point1, const Point& point2);
  Point getSide(const Point& point1, const Point& point2);

  bool noConditional(const Polygon&);
  bool isEqualSize(size_t size, const Polygon& polygon);
  bool isPolygonEven(const Polygon& polygon);
  bool isPolygonOdd(const Polygon& polygon);
  size_t getVertexes(const std::string& str);
}
#endif
