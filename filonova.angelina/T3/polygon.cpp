#include "polygon.hpp"
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
  return (lhs.x < rhs.x) || (lhs.x == rhs.x && lhs.y < rhs.y);
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

  std::vector< Point > points(vertexCount);
  for (size_t i = 0; i < vertexCount; i++)
  {
    in >> points[i];
    if (!in)
    {
      return in;
    }
  }

  polygon.points = std::move(points);
  return in;
}

double filonova::getArea(const Polygon &polygon)
{
  const std::vector< Point > &points = polygon.points;

  size_t vertexCount = points.size();
  if (vertexCount < MIN_VERTEX_COUNT)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  double area = 0.0;
  for (size_t i = 0; i < vertexCount; ++i)
  {
    const Point &p1 = points[i];
    const Point &p2 = points[(i + 1) % vertexCount];
    area += static_cast< double >(p1.x) * p2.y - static_cast< double >(p2.x) * p1.y;
  }

  return std::abs(area) / 2.0;
}
