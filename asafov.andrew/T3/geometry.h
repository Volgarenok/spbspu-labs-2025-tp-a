#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <iostream>

namespace asafov
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

  double computeArea(const Polygon& poly);
  bool arePolygonsSame(const Polygon& a, const Polygon& b);
  bool doPolygonsIntersect(const Polygon& a, const Polygon& b);
  bool isPointInPolygon(const Point& point, const Polygon& poly);
  bool isPolygonConvex(const Polygon& poly);
  bool isPolygonRectangle(const Polygon& poly);
  bool hasRightAngle(const Polygon& poly);
  Polygon getBoundingBox(const std::vector<Polygon>& polygons);
}

#endif
