#ifndef UTILS_HPP
#define UTILS_HPP
#include <functional>
#include "polygon.hpp"

namespace klimova {
  using namespace std::placeholders;

  double areaEven(double value, const Polygon& polygon);
  double areaOdd(double value, const Polygon& polygon);
  double areaMean(double value, const Polygon& polygon, size_t size);
  double areaNum(double value, const Polygon& polygon, size_t vertexes);
  double areaPolygon(const Polygon& polygon);
  double multiplier(const Point& p1, const Point& p2);
  bool isPolygonEven(const Polygon& polygon);
  size_t getVertexes(const std::string& str);

  double getArea(const Polygon& polygon);
  size_t getVertexesCount(const Polygon& polygon);

  bool isVertexCountEven(const Polygon& polygon);
  bool isVertexCountOdd(const Polygon& polygon);
  bool hasVertexCount(const Polygon& polygon, size_t vertexes);

  bool isValidVertexCount(size_t vertexes);

  struct PointComparator {
    bool operator()(const Point& a, const Point& b) const;
  };

  struct PointEqual {
    bool operator()(const Point& a, const Point& b) const;
  };

  struct PermsTester {
    const Polygon& target;
    bool operator()(const Polygon& p) const;
  };

  struct RectangleChecker {
    bool operator()(const Polygon& p) const;
  private:
    static bool is_right_angle(const Point& a, const Point& b, const Point& c);
  };
}
#endif
