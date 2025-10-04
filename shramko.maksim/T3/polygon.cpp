#include "polygon.hpp"
#include <vector>
#include <cmath>

namespace shramko
{
  bool checkUnique(const std::vector< Point >& points, size_t i = 0, size_t j = 1)
  {
    if (i >= points.size() - 1)
    {
      return true;
    }
    if (j >= points.size())
    {
      return checkUnique(points, i + 1, i + 2);
    }
    if (points[i] == points[j])
    {
      return false;
    }
    return checkUnique(points, i, j + 1);
  }

  void readPoints(std::istream& in, std::vector< Point >& points, size_t n)
  {
    if (n == 0)
    {
      return;
    }
    Point p;
    in >> p;
    points.push_back(p);
    readPoints(in, points, n - 1);
  }

  std::istream& operator>>(std::istream& in, Polygon& polygon)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    size_t numPoints = 0;
    in >> numPoints;
    if (numPoints < 3 || in.fail())
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    polygon.points.clear();
    polygon.points.reserve(numPoints);
    readPoints(in, polygon.points, numPoints);
    if (!in)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    if (!checkUnique(polygon.points))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    return in;
  }

  double calculateArea(const std::vector< Point >& points, size_t i = 0, double sum = 0.0)
  {
    if (i >= points.size())
    {
      return std::abs(sum) / 2.0;
    }
    size_t n = points.size();
    size_t next = (i + 1) % n;
    sum += static_cast< double >(points[i].x) * points[next].y - static_cast< double >(points[next].x) * points[i].y;
    return calculateArea(points, i + 1, sum);
  }

  double getPolygonArea(const Polygon& polygon)
  {
    if (polygon.points.size() < 3)
    {
      return 0.0;
    }
    return calculateArea(polygon.points);
  }

  std::istream& operator>>(std::istream& in, Point& point)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char ch;
    in >> ch;
    if (ch != '(')
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    in >> point.x;
    in >> ch;
    if (ch != ';')
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    in >> point.y;
    in >> ch;
    if (ch != ')')
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    return in;
  }

  bool operator<(const Point& a, const Point& b)
  {
    return a.x < b.x || (a.x == b.x && a.y < b.y);
  }

  bool operator==(const Point& a, const Point& b)
  {
    return a.x == b.x && a.y == b.y;
  }

  Point operator-(const Point& a, const Point& b)
  {
    return {a.x - b.x, a.y - b.y};
  }

  long long dot(const Point& a, const Point& b)
  {
    return static_cast< long long >(a.x) * b.x + static_cast< long long >(a.y) * b.y;
  }
}
