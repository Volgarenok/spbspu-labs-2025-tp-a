#include <algorithm>
#include <iterator>
#include <cmath>
#include <numeric>
#include "streamGuard.hpp"
#include "polygon.hpp"

namespace ivanova
{
  bool Point::operator==(const Point& other) const
  {
    return x == other.x && y == other.y;
  }

  bool Polygon::operator==(const Polygon& other) const
  {
    return points == other.points;
  }

  std::istream& operator>>(std::istream& in, Point& point)
  {
    std::istream::sentry sent(in);
    if (!sent)
    {
      return in;
    }

    StreamGuard guard(in);
    char c1 = 0;
    char c2 = 0;
    char c3 = 0;
    if (!(in >> c1 >> point.x >> c2 >> point.y >> c3) || c1 != '(' || c2 != ';' || c3 != ')')
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const Point& point)
  {
    out << '(' << point.x << ';' << point.y << ')';
    return out;
  }

  class PointInserter
  {
  public:
    PointInserter(std::vector< Point >& p, size_t c):
      points(p),
      count(c),
      current(0)
    {}

    void operator()(const Point& point)
    {
      if (current < count)
      {
        points.push_back(point);
        current++;
      }
    }

    size_t getCurrent() const
    {
      return current;
    }

    bool isComplete() const
    {
      return current >= count;
    }

  private:
    std::vector< Point >& points;
    size_t count;
    size_t current;
  };

  std::istream& operator>>(std::istream& in, Polygon& poly)
  {
    std::istream::sentry sent(in);
    if (!sent)
    {
      return in;
    }

    size_t count = 0;
    if (!(in >> count) || count < 3)
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    std::vector< Point > temp;
    temp.reserve(count);

    PointInserter inserter(temp, count);
    std::istream_iterator< Point > it(in);
    std::istream_iterator< Point > end;

    while (it != end && !inserter.isComplete())
    {
      inserter(*it);
      ++it;
    }

    if (in && temp.size() == count)
    {
      poly.points = std::move(temp);
    }
    else
    {
      in.setstate(std::ios::failbit);
    }

    return in;
  }

  class PointPrinter
  {
  public:
    PointPrinter(std::ostream& o):
      out(o)
    {}

    void operator()(const Point& point)
    {
      out << point << " ";
    }

  private:
    std::ostream& out;
  };

  std::ostream& operator<<(std::ostream& out, const Polygon& poly)
  {
    out << poly.points.size() << ' ';
    PointPrinter printer(out);
    std::for_each(poly.points.begin(), poly.points.end(), printer);
    return out;
  }

  class AreaAccumulator
  {
  public:
    AreaAccumulator(const Polygon& p):
      poly(p),
      n(p.points.size())
    {}

    double operator()(size_t i) const
    {
      size_t j = (i + 1) % n;
      return (poly.points[i].x * poly.points[j].y) - (poly.points[j].x * poly.points[i].y);
    }

  private:
    const Polygon& poly;
    size_t n;
  };

  class IndexGenerator
  {
  public:
    IndexGenerator(size_t m):
      current(0),
      max(m)
    {}

    size_t operator()()
    {
      if (current < max)
      {
        return current++;
      }
      return max;
    }

  private:
    size_t current;
    size_t max;
  };

  double calculateArea(const Polygon& poly)
  {
    if (poly.points.empty())
    {
      return 0.0;
    }

    size_t n = poly.points.size();
    std::vector< size_t > indices(n);
    IndexGenerator generator(n);
    std::generate(indices.begin(), indices.end(), generator);

    AreaAccumulator accumulator(poly);
    std::vector< double > partials(n);
    std::transform(indices.begin(), indices.end(), partials.begin(), accumulator);

    double sum = std::accumulate(partials.begin(), partials.end(), 0.0);
    return std::abs(sum) / 2.0;
  }
}
