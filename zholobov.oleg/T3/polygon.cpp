#include "polygon.hpp"

#include <algorithm>
#include <functional>
#include <istream>
#include <iterator>
#include <numeric>

namespace {

  struct AreaAccumulator {
    const zholobov::Point& p0;
    zholobov::Point cur;

    float operator()(float acc, const zholobov::Point& p)
    {
      acc += (cur.x - p0.x) * (p.y - p0.y) - (cur.y - p0.y) * (p.x - p0.x);
      cur = p;
      return acc;
    }
  };

  long long cross(const zholobov::Point& a, const zholobov::Point& b)
  {
    return a.x * b.y - a.y * b.x;
  }

  zholobov::Point vector(const zholobov::Point& a, const zholobov::Point& b)
  {
    return {b.x - a.x, b.y - a.y};
  }

  bool onSegment(const zholobov::Point& a, const zholobov::Point& b, const zholobov::Point& p)
  {
    return (cross(vector(a, b), vector(a, p)) == 0) &&
           (std::min(a.x, b.x) <= p.x) && (p.x <= std::max(a.x, b.x)) &&
           (std::min(a.y, b.y) <= p.y) && (p.y <= std::max(a.y, b.y));
  }

  bool segmentsIntersect(const zholobov::Point& a1, const zholobov::Point& a2,
      const zholobov::Point& b1, const zholobov::Point& b2)
  {
    auto dir = [](const zholobov::Point& a, const zholobov::Point& b, const zholobov::Point& c) {
      return cross(vector(a, b), vector(a, c));
    };

    int d1 = dir(a1, a2, b1);
    int d2 = dir(a1, a2, b2);
    int d3 = dir(b1, b2, a1);
    int d4 = dir(b1, b2, a2);

    if (((d1 * d2) < 0) && ((d3 * d4) < 0)) {
      return true;
    }
    return onSegment(a1, a2, b1) || onSegment(a1, a2, b2) || onSegment(b1, b2, a1) || onSegment(b1, b2, a2);
  }

  bool pointInPolygon(const zholobov::Point& p, const zholobov::Polygon& poly)
  {
    int n = poly.points.size();
    if (n < 3) {
      return false;
    }
    bool inside = false;
    for (int i = 0; i < n; ++i) {
      zholobov::Point a = poly.points[i];
      zholobov::Point b = poly.points[(i + 1) % n];
      if ((a.y > p.y) != (b.y > p.y)) {
        float x = a.x + static_cast< float >(b.x - a.x) * (p.y - a.y) / (b.y - a.y);
        if (x > p.x)
          inside = !inside;
      }
    }
    return inside;
  }

}

namespace zholobov {

  namespace io_helpers {

    struct CharIO {
      char c;
    };

    std::istream& operator>>(std::istream& input, CharIO&& value)
    {
      std::istream::sentry s(input);
      if (!s) {
        return input;
      }
      char c = 0;
      if ((input >> c) && (c != value.c)) {
        input.setstate(std::ios::failbit);
      }
      return input;
    }

  }

  std::istream& operator>>(std::istream& input, Point& point)
  {
    std::istream::sentry s(input);
    if (!s) {
      return input;
    }
    int x = 0;
    int y = 0;
    if ((input >> io_helpers::CharIO{'('} >> x >> io_helpers::CharIO{';'} >> y >> io_helpers::CharIO{')'})) {
      point.x = x;
      point.y = y;
    }
    return input;
  }

  std::istream& operator>>(std::istream& input, Polygon& polygon)
  {
    std::istream::sentry s(input);
    if (!s) {
      return input;
    }
    size_t count = 0;
    if (input >> count && count >= 3) {
      std::vector< Point > points;
      std::copy_n(std::istream_iterator< Point >(input), count, std::back_inserter(points));
      if (input) {
        polygon.points = std::move(points);
      }
    } else {
      input.setstate(std::ios::failbit);
    }
    return input;
  }
}

float zholobov::calcArea(const Polygon& polygon)
{
  if (polygon.points.size() < 3) {
    return 0.0;
  }
  AreaAccumulator areaAcc{polygon.points[0], polygon.points[1]};
  float sum = std::accumulate(polygon.points.cbegin() + 2, polygon.points.cend(), 0.0f, std::ref(areaAcc));
  return 0.5f * std::abs(sum);
}

bool zholobov::hasIntersection(const zholobov::Polygon& poly1, const zholobov::Polygon& poly2)
{
  int n1 = poly1.points.size();
  int n2 = poly2.points.size();

  for (int i = 0; i < n1; ++i) {
    zholobov::Point a1 = poly1.points[i];
    zholobov::Point a2 = poly1.points[(i + 1) % n1];
    for (int j = 0; j < n2; ++j) {
      zholobov::Point b1 = poly2.points[j];
      zholobov::Point b2 = poly2.points[(j + 1) % n2];
      if (segmentsIntersect(a1, a2, b1, b2))
        return true;
    }
  }
  return (pointInPolygon(poly1.points[0], poly2)) || (pointInPolygon(poly2.points[0], poly1));
}
