#include "geometry.hpp"
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace cherkasov
{
  bool Point::operator==(const Point& other) const
  {
    return x == other.x && y == other.y;
  }

  bool Point::operator!=(const Point& other) const
  {
    return !(*this == other);
  }

  bool Polygon::operator==(const Polygon& other) const
  {
    return points == other.points;
  }

  double getArea(const Polygon& poly)
  {
    if (poly.points.size() < 3)
    {
      return 0.0;
    }
      double area = 0.0;
      size_t n = poly.points.size();
      for (size_t i = 0; i < n; ++i)
      {
        const Point& p1 = poly.points[i];
        const Point& p2 = poly.points[(i + 1) % n];
        area += (p1.x * p2.y) - (p2.x * p1.y);
      }
      return std::abs(area) / 2.0;
  }

  int orientation(const Point& a, const Point& b, const Point& c)
  {
    long long val = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
    if (val == 0)
    {
      return 0;
    }
    return (val > 0) ? 1 : 2;
  }

  bool onSegment(const Point& p, const Point& q, const Point& r)
  {
    return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
           q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y);
  }

  bool edgesIntersect(const Point& p1, const Point& p2, const Point& q1, const Point& q2)
  {
    int o1 = orientation(p1, p2, q1);
    int o2 = orientation(p1, p2, q2);
    int o3 = orientation(q1, q2, p1);
    int o4 = orientation(q1, q2, p2);
    if (o1 != o2 && o3 != o4)
    {
      return true;
    }
    if (o1 == 0 && onSegment(p1, q1, p2))
    {
      return true;
    }
    if (o2 == 0 && onSegment(p1, q2, p2))
    {
      return true;
    }
    if (o3 == 0 && onSegment(q1, p1, q2))
    {
      return true;
    }
    if (o4 == 0 && onSegment(q1, p2, q2))
    {
      return true;
    }
    return false;
  }

  bool polygonsIntersect(const Polygon& a, const Polygon& b)
  {
    if (a.points == b.points)
    {
      return true;
    }
    const size_t n = a.points.size();
    const size_t m = b.points.size();
    for (size_t i = 0; i < n; ++i)
    {
      const Point& a1 = a.points[i];
      const Point& a2 = a.points[(i + 1) % n];
      for (size_t j = 0; j < m; ++j)
      {
        const Point& b1 = b.points[j];
        const Point& b2 = b.points[(j + 1) % m];
        if (edgesIntersect(a1, a2, b1, b2))
        {
          return true;
        }
      }
    }
    if (isPointInPolygon(a.points[0], b) || isPointInPolygon(b.points[0], a))
    {
      return true;
    }
    return false;
  }

  bool hasRightAngle(const Polygon& poly)
  {
    size_t n = poly.points.size();
    if (n < 3) return false;

    for (size_t i = 0; i < n; ++i)
    {
      const Point& a = poly.points[i];
      const Point& b = poly.points[(i + 1) % n];
      const Point& c = poly.points[(i + 2) % n];
      int dx1 = b.x - a.x;
      int dy1 = b.y - a.y;
      int dx2 = c.x - b.x;
      int dy2 = c.y - b.y;
      if (dx1 * dx2 + dy1 * dy2 == 0)
      {
        return true;
      }
    }
    return false;
  }
  bool isPointInPolygon(const Point& point, const Polygon& poly)
  {
    if (poly.points.size() < 3)
    {
      return false;
    }
    bool inside = false;
    size_t n = poly.points.size();
    for (size_t i = 0, j = n - 1; i < n; j = i++)
    {
      const Point& pi = poly.points[i];
      const Point& pj = poly.points[j];
      bool intersect = ((pi.y > point.y) != (pj.y > point.y)) &&
        (point.x < (pj.x - pi.x) * (point.y - pi.y) / double(pj.y - pi.y) + pi.x);

      if (intersect)
      {
        inside = !inside;
      }
    }
    return inside;
  }
  std::vector<Polygon> readPolygons(const std::string& filename)
  {
    std::ifstream file(filename);
    if (!file.is_open())
    {
      throw std::runtime_error("Cannot open file: " + filename);
    }

    std::vector<Polygon> polys;
    std::string line;
    while (std::getline(file, line))
    {
      if (line.empty())
      {
        continue;
      }
      try
      {
        Polygon poly = parsePolygon(line);
        if (poly.points.size() >= 3)
        {
          polys.push_back(poly);
        }
      }
      catch (...)
      {
        continue;
      }
    }
    return polys;
  }

  Polygon parsePolygon(const std::string &str)
  {
    std::istringstream iss(str);
    size_t n;
    if (!(iss >> n) || n < 3)
    {
      throw std::invalid_argument("Invalid vertex count");
    }
    Polygon poly;
    for (size_t i = 0; i < n; ++i)
    {
      char ch;
      Point p;
      if (!(iss >> ch >> p.x >> ch >> p.y >> ch))
      {
        throw std::invalid_argument("Invalid point format");
      }
      poly.points.push_back(p);
    }
    if (poly.points.size() != n)
    {
      throw std::invalid_argument("vertex count mismatch");
    }
    for (size_t i = 0; i < poly.points.size(); ++i)
    {
      for (size_t j = i + 1; j < poly.points.size(); ++j)
      {
        if (poly.points[i] == poly.points[j])
        {
          throw std::invalid_argument("duplicate point");
        }
      }
    }
    std::string rest;
    if (iss >> rest)
    {
      throw std::invalid_argument("extra data after polygon");
    }
    return poly;
  }
}
