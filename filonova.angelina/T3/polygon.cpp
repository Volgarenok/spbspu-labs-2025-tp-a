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

  size_t count = 0;
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

filonova::TriangleGenerator::TriangleGenerator(const std::vector< Point > &points, size_t start): points_(points), current_(start)
{
  if (points.size() < MIN_VERTEX_COUNT)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}

bool filonova::TriangleGenerator::hasNext() const
{
  return current_ < points_.size() - 1;
}

filonova::Triangle filonova::TriangleGenerator::next()
{
  Triangle tr = {points_[0], points_[current_], points_[current_ + 1]};
  current_++;
  return tr;
}

double filonova::TriangleAreaFunctor::operator()(const Triangle &tri) const
{
  return triangleArea(tri.a, tri.b, tri.c);
}

double filonova::triangleArea(const Point &a, const Point &b, const Point &c)
{
  double term1 = static_cast< double >(a.x) * (b.y - c.y);
  double term2 = static_cast< double >(b.x) * (c.y - a.y);
  double term3 = static_cast< double >(c.x) * (a.y - b.y);

  return std::abs(term1 + term2 + term3) / 2.0;
}

double filonova::getArea(const Polygon &polygon)
{
  if (polygon.points.size() < MIN_VERTEX_COUNT)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  TriangleGenerator generator(polygon.points);
  TriangleAreaFunctor areaFunc;
  double area = 0.0;

  while (generator.hasNext())
  {
    area += areaFunc(generator.next());
  }

  return area;
}

double filonova::computeTotalArea(const std::vector< Polygon > &polygons)
{
  std::vector< double > areas(polygons.size());
  std::transform(polygons.begin(), polygons.end(), areas.begin(), getArea);
  return std::accumulate(areas.begin(), areas.end(), 0.0);
}
