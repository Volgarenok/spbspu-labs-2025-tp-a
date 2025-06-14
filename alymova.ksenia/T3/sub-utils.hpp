#ifndef SUB_COMMANDS_HPP
#define SUB_COMMANDS_HPP
#include <functional>
#include "shapes.hpp"

namespace alymova
{
  using namespace std::placeholders;

  double areaEven(double value, const Polygon& polygon, size_t size);
  double areaOdd(double value, const Polygon& polygon, size_t size);
  double areaMean(double value, const Polygon& polygon, size_t size);
  double areaNumber(double value, const Polygon& polygon, size_t vertexes);
  double areaPolygon(const Polygon& polygon);
  double multPoints(const Point& point1, const Point& point2);

  double maxArea(double value, const Polygon& polygon);
  double minArea(double value, const Polygon& polygon);
  size_t maxVertexes(size_t value, const Polygon& polygon);
  size_t minVertexes(size_t value, const Polygon& polygon);

  int findMaxXPolygon(int start, const Polygon& polygon);
  int findMaxYPolygon(int start, const Polygon& polygon);
  int findMinXPolygon(int start, const Polygon& polygon);
  int findMinYPolygon(int start, const Polygon& polygon);

  int findMaxXVector(int start, const std::vector< Polygon >& polygons);
  int findMaxYVector(int start, const std::vector< Polygon >& polygons);
  int findMinXVector(int start, const std::vector< Polygon >& polygons);
  int findMinYVector(int start, const std::vector< Polygon >& polygons);
  void inFrameOutput(std::ostream& out, bool res);

  bool haveRightAngles(const Polygon& polygon);
  bool isRightAngle(const Point& point1, const Point& point2);
  Point getSide(const Point& point1, const Point& point2);

  bool isDigit(char c);
  bool isEqualSize(size_t size, const Polygon& polygon);
  bool isPolygonEven(const Polygon& polygon);
  size_t getVertexes(const std::string& str);
}
#endif
