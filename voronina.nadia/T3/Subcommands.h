#ifndef SUBCOMMANDS_H
#define SUBCOMMANDS_H

#include <cmath>
#include <algorithm>
#include <functional>
#include <numeric>

#include "Shapes.h"

namespace voronina
{
  using namespace std::placeholders;
  double getAreaOfPolygon(const Polygon& shape);
  double getAreaOfTriangle(const Point& p1, const Point& p2, const Point& p3);
  double getAreaForEvenPolygons(double areaSum, const Polygon& shape);
  double getAreaForOddPolygons(double areaSum, const Polygon& shape);
  double getSumArea(double areaSum, const Polygon& shape);
  bool isDigitBool(char c);
  double getAreaOfGivenAmountOfVertexes(double areaSum, const Polygon& shape, int vertexes);
  int isEqual(const Polygon& left, const Polygon& right);
  int getMaxSeq(int left, int right);
  bool areaComporator(const Polygon& left, const Polygon& right);
  bool vertexesComporator(const Polygon& left, const Polygon& right);
  bool isEven(const Polygon& shape);
  bool isOdd(const Polygon& shape);
  bool hasGivenAmountOfVertexes(const Polygon& shape, int vertexes);
  bool isRightAngle(const Point& side1, const Point& side2);
  bool isThereRightAngleInPolygon(const Polygon& shape);
  Point getSide(const Point& p1, const Point& p2);
  bool isTrue(bool element);
}
#endif
