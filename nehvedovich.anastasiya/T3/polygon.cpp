#include "polygon.hpp"
#include <algorithm>
#include <iterator>
#include <numeric>
#include <cmath>
#include <sstream>

namespace nehvedovich
{

  std::istream &operator>>(std::istream &in, Polygon &dest)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }

    std::size_t n = 0;
    if (!(in >> n))
    {
      return in;
    }
    if (n < 3)
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    char c = static_cast< char >(in.peek());
    if (c == '\n')
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    std::string line;
    std::getline(in, line);
    std::istringstream ls(line);

    std::vector< Point > pts;
    pts.reserve(n);
    std::istream_iterator< Point > it(ls);
    std::copy_n(it, n, std::back_inserter(pts));

    if (ls.fail())
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    std::string tail;
    std::getline(ls, tail);
    if (tail.find_first_not_of(" \t\r") != std::string::npos)
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    std::vector< Point > tmp(pts);
    std::sort(tmp.begin(), tmp.end(), PointLess());
    struct PointEq
    {
      bool operator()(const Point &a, const Point &b) const
      {
        return a.x == b.x && a.y == b.y;
      }
    };
    if (std::adjacent_find(tmp.begin(), tmp.end(), PointEq()) != tmp.end())
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    dest.points.swap(pts);
    return in;
  }

  std::ostream &operator<<(std::ostream &out, const Point &src)
  {
    std::ostream::sentry s(out);
    if (!s)
    {
      return out;
    }
    return out << '(' << src.x << ';' << src.y << ')';
  }

  bool operator==(const Point &a, const Point &b)
  {
    return a.x == b.x && a.y == b.y;
  }

  std::istream &operator>>(std::istream &in, Point &pt)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }

    char lp = 0, sc = 0, rp = 0;
    int x = 0, y = 0;

    if (!(in >> lp) || lp != '(')
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    if (!(in >> x))
    {
      return in;
    }
    if (!(in >> sc) || sc != ';')
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    if (!(in >> y))
    {
      return in;
    }
    if (!(in >> rp) || rp != ')')
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    pt.x = x;
    pt.y = y;
    return in;
  }

  std::ostream &operator<<(std::ostream &out, const Polygon &src)
  {
    std::ostream::sentry s(out);
    if (!s)
    {
      return out;
    }

    out << src.points.size() << ' ';
    std::ostream_iterator< Point > oit(out, " ");
    std::copy(src.points.begin(), src.points.end(), oit);
    return out;
  }

  bool operator==(const Polygon &p1, const Polygon &p2)
  {
    return p1.points == p2.points;
  }

  bool operator!=(const Polygon &p1, const Polygon &p2)
  {
    return !(p1 == p2);
  }

  struct PolygonAreaAccumulator
  {
    explicit PolygonAreaAccumulator(const Polygon &p):
      poly(p),
      n(p.points.size())
    {}

    double operator()(double acc, const Point &p1) const
    {
      const std::size_t i = static_cast< std::size_t >(&p1 - &poly.points[0]);
      const Point &p2 = poly.points[(i + 1) % n];
      return acc + (static_cast< double >(p1.x) * p2.y - static_cast< double >(p2.x) * p1.y);
    }

    const Polygon &poly;
    std::size_t n;
  };

  double calcPolygonArea(const Polygon &poly)
  {
    if (poly.points.size() < 3)
    {
      return 0.0;
    }
    const double dblArea = std::accumulate(poly.points.begin(), poly.points.end(), 0.0, PolygonAreaAccumulator(poly));
    return std::abs(dblArea / 2.0);
  }
}
