#include "polygon.hpp"

std::istream &filonova::operator>>(std::istream &in, Point &point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  Point temp{0, 0};
  in >> DelimiterValue{'('} >> temp.x >> DelimiterValue{';'} >> temp.y >> DelimiterValue{')'};

  if (in)
  {
    point = temp;
  }
  return in;
}

bool filonova::operator<(const Point &lhs, const Point &rhs)
{
  return (lhs.x < rhs.x) && (lhs.y < rhs.y);
}

bool filonova::operator==(const Point &lhs, const Point &rhs)
{
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

std::istream &filonova::operator>>(std::istream &in, Polygon &polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  size_t count;
  in >> count;
  if (count < MIN_VERTEX_COUNT)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::vector< Point > temp;
  using input_it_t = std::istream_iterator< Point >;
  std::copy_n(input_it_t{in}, count, std::back_inserter(temp));

  if (in)
  {
    polygon.points = temp;
  }

  return in;
}

bool filonova::isOdd(const Polygon &p)
{
  return (p.points.size() % 2) == 1;
}

bool filonova::isEven(const Polygon &p)
{
  return (p.points.size() % 2) == 0;
}

double filonova::getDistance(const Point &p1, const Point &p2)
{
  return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}

filonova::Polygon filonova::buildTriangle(size_t i, const std::vector< Point > &pts)
{
  return Polygon{std::vector< Point >{pts[0], pts[i + 1], pts[i + 2]}};
}

filonova::Polygon filonova::TriangleGenerator::operator()()
{
  return buildTriangle(index++, points);
}

std::vector< filonova::Polygon > filonova::polygonToTriangles(const Polygon &p)
{
  std::vector< Polygon > triangles(p.points.size() - 2);
  size_t index = 0;
  std::generate(triangles.begin(), triangles.end(), TriangleGenerator{index, p.points});

  return triangles;
}

double filonova::getTriangleArea(const Polygon &triangle)
{
  double a = getDistance(triangle.points[0], triangle.points[1]);
  double b = getDistance(triangle.points[1], triangle.points[2]);
  double c = getDistance(triangle.points[0], triangle.points[2]);
  double s = (a + b + c) / 2.0;

  return std::sqrt(s * (s - a) * (s - b) * (s - c));
}

double filonova::getPolygonArea(const Polygon &polygon)
{
  std::vector< Polygon > triangles = polygonToTriangles(polygon);
  std::vector< double > areas;
  std::transform(triangles.begin(), triangles.end(), std::back_inserter(areas), getTriangleArea);

  return std::accumulate(areas.begin(), areas.end(), 0.0);
}

double filonova::getTotalArea(const std::vector< Polygon > &polygons)
{
  std::vector< double > areas(polygons.size());
  std::transform(polygons.begin(), polygons.end(), areas.begin(), getPolygonArea);

  return std::accumulate(areas.begin(), areas.end(), 0.0);
}

bool filonova::compareByArea(const Polygon &a, const Polygon &b)
{
  return getPolygonArea(a) < getPolygonArea(b);
}

bool filonova::compareByVertexes(const Polygon &a, const Polygon &b)
{
  return a.points.size() < b.points.size();
}

bool filonova::comparePointByX(const Point &a, const Point &b)
{
  return (a.x < b.x) || (a.x == b.x && a.y < b.y);
}

bool filonova::comparePointByY(const Point &a, const Point &b)
{
  return (a.y < b.y) || (a.y == b.y && a.x < b.x);
}
