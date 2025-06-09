#include "Subcommands.h"

double voronina::getAreaOfPolygon(const Polygon &shape)
{
  std::vector<double> areaOfTriangles(shape.points.size() - 2);

  auto shapePBegin = shape.points.cbegin();
  auto shapePEnd = shape.points.cend();
  auto bindForAriaTriangle = std::bind(getAreaOfTriangle, std::cref(shape.points[0]), _1, _2);

  std::transform(shapePBegin + 2, shapePEnd, shapePBegin + 1, areaOfTriangles.begin(), bindForAriaTriangle);

  return std::accumulate(areaOfTriangles.cbegin(), areaOfTriangles.cend(), 0.0);
}
double voronina::getAreaOfTriangle(const Point &p1, const Point &p2, const Point &p3)
{
  return 0.5 * std::abs((p1.x - p3.x) * (p2.y - p1.y) - (p1.x - p2.x) * (p3.y - p1.y));
}
double voronina::getAreaForEvenPolygons(double areaSum, const Polygon &shape)
{
  if (shape.points.size() % 2 == 0)
  {
    return areaSum + getAreaOfPolygon(shape);
  }
  return areaSum;
}
double voronina::getAreaForOddPolygons(double areaSum, const Polygon &shape)
{
  if (shape.points.size() % 2 != 0)
  {
    return areaSum + getAreaOfPolygon(shape);
  }
  return areaSum;
}
double voronina::getSumArea(double areaSum, const Polygon &shape)
{
  return areaSum + getAreaOfPolygon(shape);
}
bool voronina::isDigitBool(char c)
{
  return static_cast<bool>(isdigit(c));
}
double voronina::getAreaOfGivenAmountOfVertexes(double areaSum, const Polygon &shape, int vertexes)
{
  if (shape.points.size() == static_cast<size_t>(vertexes))
  {
    return areaSum + getAreaOfPolygon(shape);
  }
  return areaSum;
}
int voronina::isEqual(const Polygon &left, const Polygon &right)
{
  return left == right;
}
int voronina::getMaxSeq(int left, int right)
{
  if (left == 0 || right == 0)
  {
    return left;
  }
  return left + right;
}
bool voronina::areaComporator(const Polygon &left, const Polygon &right)
{
  return getAreaOfPolygon(left) < getAreaOfPolygon(right);
}
bool voronina::vertexesComporator(const Polygon &left, const Polygon &right)
{
  return left.points.size() < right.points.size();
}
bool voronina::isEven(const Polygon &shape)
{
  return shape.points.size() % 2 == 0;
}
bool voronina::isOdd(const Polygon &shape)
{
  return shape.points.size() % 2 != 0;
}
bool voronina::hasGivenAmountOfVertexes(const Polygon &shape, int vertexes)
{
  return shape.points.size() == static_cast<size_t>(vertexes);
}
bool voronina::isRightAngle(const Point &side1, const Point &side2)
{
  return (side1.x * side2.x + side1.y * side2.y) == 0;
}
bool voronina::isThereRightAngleInPolygon(const Polygon &shape)
{
  std::vector<Point> sides(shape.points.size());
  auto shapePBegin = shape.points.cbegin();
  std::transform(shapePBegin + 1, shape.points.cend(), shapePBegin, sides.begin(), getSide);
  sides[shape.points.size() - 1] = getSide(shape.points[0], shape.points[shape.points.size() - 1]);

  std::vector<bool> hasRightAngleVector(shape.points.size());
  std::transform(sides.cbegin() + 1, sides.cend(), sides.cbegin(), hasRightAngleVector.begin(), isRightAngle);

  hasRightAngleVector[sides.size() - 1] = isRightAngle(sides[0], sides[sides.size() - 1]);
  return std::any_of(hasRightAngleVector.cbegin(), hasRightAngleVector.cend(), isTrue);
}
voronina::Point voronina::getSide(const Point &p1, const Point &p2)
{
  Point newSide;
  newSide.x = p1.x - p2.x;
  newSide.y = p1.y - p2.y;
  return newSide;
}
bool voronina::isTrue(bool element)
{
  return element != 0;
}
