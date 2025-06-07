#ifndef SUB_COMMANDS_HPP
#define SUB_COMMANDS_HPP
#include <functional>
#include "shapes.hpp"

namespace alymova
{
  using namespace std::placeholders;
  using PredicatePoint = std::function< int(int, const Point& point) >;
  template< class T >
  using Comparator = std::function< T(T, T) >;

  double areaEven(double value, const Polygon& polygon, size_t size);
  double areaOdd(double value, const Polygon& polygon, size_t size);
  double areaMean(double value, const Polygon& polygon, size_t size);
  double areaNumber(double value, const Polygon& polygon, size_t vertexes);
  double areaPolygon(const Polygon& polygon);
  double multPoints(const Point& point1, const Point& point2);

  double compareArea(Comparator< double > cmp, double value, const Polygon& polygon);
  size_t compareVertexes(Comparator< size_t > cmp, size_t value, const Polygon& polygon);

  int compareMaxXPoint(int value, const Point& point);
  int compareMaxYPoint(int value, const Point& point);
  int compareMinXPoint(int value, const Point& point);
  int compareMinYPoint(int value, const Point& point);
  int findMaxMinXYPolygon(PredicatePoint pred, int start, const Polygon& polygon);
  int findMaxMinXYVector(PredicatePoint pred, int start, const std::vector< Polygon >& polygons);
  void inFrameOutput(std::ostream& out, bool res);

  bool haveRightAngles(const Polygon& polygon);
  bool isRightAngle(const Point& point1, const Point& point2);
  Point getSide(const Point& point1, const Point& point2);

  bool isDigit(char c);
  bool isEqualSize(size_t size, const Polygon& polygon);
  bool isPolygonEven(const Polygon& polygon);
  size_t getVertexes(const std::string& str);

  template< class T >
  T maxWrapper(const T& lhs, const T& rhs)
  {
    return std::max(lhs, rhs);
  }

  template< class T >
  T minWrapper(const T& lhs, const T& rhs)
  {
    return std::min(lhs, rhs);
  }
}
#endif
