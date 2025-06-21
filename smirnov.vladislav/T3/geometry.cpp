#include "geometry.hpp"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iterator>
#include <limits>
#include <ios>
#include <delimiter.hpp>

namespace geom
{
  double crossProduct(const Point& a, const Point& b)
  {
    return static_cast< double >(a.x) * b.y - static_cast< double >(b.x) * a.y;
  }

  bool operator==(const Point& p1, const Point& p2)
  {
    return p1.x == p2.x && p1.y == p2.y;
  }

  std::istream& operator>>(std::istream& in, Point& p)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    using del = io::DelimiterIO;
    return in >> del{ '(' } >> p.x >> del{ ';' } >> p.y >> del{ ')' };
  }

  std::istream& operator>>(std::istream& in, Polygon& poly)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    size_t count = 0;
    if (!(in >> count) || count < 3)
    {
      in.setstate(std::ios::failbit);
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      return in;
    }

    std::vector< Point > tmp;
    tmp.reserve(count);

    try
    {
      std::generate_n(std::back_inserter(tmp), count, [&] {
        Point p;
        if (!(in >> p))
        {
          throw std::ios_base::failure{ "Error" };
        }
        return p;
        });
    }
    catch (const std::ios_base::failure&)
    {
      in.setstate(std::ios::failbit);
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      return in;
    }

    char next_char = in.peek();
    if (next_char != EOF)
    {
      if (!std::isspace(next_char) && next_char != '\n')
      {
        in.setstate(std::ios::failbit);
        in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        return in;
      }
      else if (next_char == '\n')
      {
        in.get();
      }
      else
      {
        in >> std::ws;
        next_char = in.peek();
        if (next_char != EOF && next_char != '\n')
        {
          in.setstate(std::ios::failbit);
          in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
          return in;
        }
        else if (next_char == '\n')
        {
          in.get();
        }
      }
    }
    poly.points = std::move(tmp);
    return in;
  }

  double getPolygonArea(const Polygon& poly)
  {
    const auto& pts = poly.points;
    if (pts.size() < 3)
    {
      return 0;
    }
    std::vector< Point > shifted(pts.begin() + 1, pts.end());
    shifted.push_back(pts.front());

    double area = std::inner_product(
      pts.begin(), pts.end(),
      shifted.begin(),
      0.0,
      std::plus<>(),
      crossProduct
    );

    return std::abs(area) / 2.0;
  }
}
