#include "Polygon.hpp"
#include <IOStreamGuard.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>

namespace gavrilova {

  struct PointDeterminant {
    double operator()(const Point& p1, const Point& p2) const
    {
      return static_cast< double >(p1.x) * p2.y - static_cast< double >(p2.x) * p1.y;
    }
  };

  double getArea(const Polygon& polygon)
  {
    if (polygon.points.size() < 3) {
      return 0.0;
    }
    double area = std::inner_product(
        polygon.points.begin(), polygon.points.end() - 1,
        polygon.points.begin() + 1,
        0.0,
        std::plus< double >(),
        PointDeterminant());
    area += PointDeterminant()(polygon.points.back(), polygon.points.front());
    return std::abs(area) / 2.0;
  }

  bool hasEvenVertices(const Polygon& polygon)
  {
    return polygon.points.size() % 2 == 0;
  }

  bool hasOddVertices(const Polygon& polygon)
  {
    return !hasEvenVertices(polygon);
  }

  std::istream& operator>>(std::istream& is, Polygon& polygon)
  {
    std::istream::sentry sentry(is);
    if (!sentry) {
      return is;
    }

    size_t num_points = 0;
    is >> num_points;

    if (!is || num_points < 3) {
      is.setstate(std::ios::failbit);
      return is;
    }

    std::vector< Point > temp_points;
    temp_points.reserve(num_points);
    for (size_t i = 0; i < num_points; ++i) {
      Point p;
      if (is.peek() == '\n' || is.peek() == EOF) {
        is.setstate(std::ios::failbit);
        return is;
      }
      if (!(is >> p)) {
        is.setstate(std::ios::failbit);
        return is;
      }
      temp_points.push_back(p);
    }

    if (is.peek() != '\n' && is.peek() != EOF) {
      is.setstate(std::ios::failbit);
      return is;
    }

    polygon.points = std::move(temp_points);
    return is;
  }

  std::ostream& operator<<(std::ostream& os, const Polygon& polygon)
  {
    std::ostream::sentry sentry(os);
    if (!sentry) {
      return os;
    }
    IOStreamGuard guard(os);

    os << polygon.points.size() << " ";
    std::copy(
        polygon.points.begin(), polygon.points.end(),
        std::ostream_iterator< Point >(os, " "));
    return os;
  }
}
