#include "polygon.hpp"
#include "functors.hpp"
#include "input_struct.hpp"

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

std::istream &filonova::operator>>(std::istream &in, Polygon &polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  size_t vertexCount;
  in >> vertexCount;
  if (!in || vertexCount < MIN_VERTEX_COUNT)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::vector< Point > points;
  points.reserve(vertexCount);

  for (size_t i = 0; i < vertexCount; ++i)
  {
    Point point;
    if (!(in >> point))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    points.push_back(point);
  }

  polygon.points = std::move(points);
  return in;
}

double filonova::triangleArea(const Point &a, const Point &b, const Point &c)
{
  return std::abs(
             static_cast< double >(a.x) * (b.y - c.y) +
             static_cast< double >(b.x) * (c.y - a.y) +
             static_cast< double >(c.x) * (a.y - b.y)) / 2.0;
}

double filonova::getArea(const Polygon &polygon)
{
  const std::vector< Point > &pts = polygon.points;
  if (pts.size() < MIN_VERTEX_COUNT)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  std::vector< size_t > indices(pts.size() - 2);
  std::iota(indices.begin(), indices.end(), 1);
  return std::accumulate(
      indices.begin(),
      indices.end(),
      0.0,
      filonova::TriangleAreaAccumulator(pts));
}
