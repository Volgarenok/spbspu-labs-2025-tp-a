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
    if (input >> count) {
      std::vector< Point > points;
      std::copy_n(std::istream_iterator< Point >(input), count, std::back_inserter(points));
      if (input) {
        polygon.points = std::move(points);
      }
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
