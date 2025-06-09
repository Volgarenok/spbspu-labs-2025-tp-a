#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include <string>

struct Point {
  int x = 0;
  int y = 0;

  bool operator==(const Point& other) const;
};

struct Polygon {
  std::vector<Point> points;

  bool operator==(const Polygon& other) const;
};

bool parsePolygon(const std::string& line, Polygon& poly);
double computeArea(const Polygon& poly);

#endif
