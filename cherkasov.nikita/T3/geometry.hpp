#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <vector>
#include <string>

namespace cherkasov
{
  struct Point
  {
    int x;
    int y;

    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
  };

  struct Polygon
  {
    std::vector<Point> points;
    bool operator==(const Polygon& other) const;
  };

  double getArea(const Polygon& poly);
  bool polygonsIntersect(const Polygon& a, const Polygon& b);
  bool hasRightAngle(const Polygon& poly);
  bool isPointInPolygon(const Point& p, const Polygon& poly);
  std::vector<Polygon> readPolygons(const std::string& filename);
  Polygon parsePolygon(const std::string& string);
}

#endif
