#include "utils.hpp"
#include <numeric>
#include <string>
#include <exception>
#include <algorithm>

double klimova::areaEven(double value, const Polygon& polygon)
{
  if (isPolygonEven(polygon)) {
    return value + areaPolygon(polygon);
  }
  return value;
}

double klimova::areaOdd(double value, const Polygon& polygon)
{
  if (!isPolygonEven(polygon)) {
    return value + areaPolygon(polygon);
  }
  return value;
}

double klimova::areaMean(double value, const Polygon& polygon, size_t size)
{
  if (size == 0) {
    throw std::overflow_error("");
  }
  return value + areaPolygon(polygon) / size;
}

double klimova::areaNum(double value, const Polygon& polygon, size_t vertexes)
{
  if (polygon.points.size() == vertexes) {
    return value + areaPolygon(polygon);
  }
  return value;
}

double klimova::areaPolygon(const Polygon& polygon)
{
  Polygon rotated = polygon;
  std::rotate(rotated.points.begin(), rotated.points.begin() + 1, rotated.points.end());
  auto beginPoly = polygon.points.begin();
  auto endPoly = polygon.points.end();
  auto beginRotated = rotated.points.begin();
  double sum = std::inner_product(beginPoly, endPoly, beginRotated, 0.0, std::plus< double >{}, multiplier);
  return std::abs(sum) / 2.0;
}

double klimova::multiplier(const Point& p1, const Point& p2)
{
  return p1.x * p2.y - p1.y * p2.x;
}

bool klimova::isPolygonEven(const Polygon& polygon)
{
  return polygon.points.size() % 2 == 0;
}

size_t klimova::getVertexes(const std::string& str)
{
  size_t vertexes = std::stoull(str);
  return vertexes;
}

double klimova::getArea(const Polygon& polygon)
{
  return areaPolygon(polygon);
}

size_t klimova::getVertexesCount(const Polygon& polygon)
{
  return polygon.points.size();
}

bool klimova::isVertexCountEven(const Polygon& polygon)
{
  return polygon.points.size() % 2 == 0;
}

bool klimova::isVertexCountOdd(const Polygon& polygon)
{
  return !isVertexCountEven(polygon);
}

bool klimova::hasVertexCount(const Polygon& polygon, size_t vertexes)
{
  return polygon.points.size() == vertexes;
}

bool klimova::isValidVertexCount(size_t vertexes)
{
  return vertexes >= 3;
}

namespace klimova {
  bool PointComparator::operator()(const Point& a, const Point& b) const
  {
    return std::tie(a.x, a.y) < std::tie(b.x, b.y);
  }

  bool  PointEqual::operator()(const Point& a, const Point& b) const
  {
    return a.x == b.x && a.y == b.y;
  }

  bool PermsTester::operator()(const Polygon& p) const
  {
    if (p.points.size() != target.points.size()) {
      return false;
    }
    std::vector< Point > p_points = p.points;
    std::vector< Point > target_points = target.points;
    std::sort(p_points.begin(), p_points.end(), PointComparator());
    std::sort(target_points.begin(), target_points.end(), PointComparator());
    return std::equal(p_points.begin(), p_points.end(), target_points.begin(), PointEqual());
  }

  bool RectangleChecker::is_right_angle(const Point& a, const Point& b, const Point& c)
  {
    int dx1 = b.x - a.x;
    int dy1 = b.y - a.y;
    int dx2 = b.x - c.x;
    int dy2 = b.y - c.y;
    return (dx1 * dx2 + dy1 * dy2) == 0;
  }

  bool RectangleChecker::operator()(const Polygon& p) const
  {
    if (p.points.size() != 4) {
      return false;
    }
    bool angle1 = is_right_angle(p.points[0], p.points[1], p.points[2]);
    bool angle2 = is_right_angle(p.points[1], p.points[2], p.points[3]);
    bool angle3 = is_right_angle(p.points[2], p.points[3], p.points[0]);
    return angle1 && angle2 && angle3;
  }
}
