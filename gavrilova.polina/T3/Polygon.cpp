#include "Polygon.hpp"
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>

namespace gavrilova {

  double Polygon::area() const
  {
    if (points.empty()) {
      return 0;
    }

    auto det2 = [](const Point& a, const Point& b) {
      return a.x * b.y - b.x * a.y;
    };

    auto area = static_cast< double >(
        std::inner_product(
            points.cbegin(),
            points.cend() - 1,
            points.cbegin() + 1,
            0,
            std::plus<>(),
            det2));

    auto last_el = static_cast< double >(det2(points.back(), points.front()));
    return std::abs(area + last_el) / 2.0;
  }

  bool Polygon::isEven() const
  {
    return points.size() % 2 == 0;
  }

  bool Polygon::isOdd() const
  {
    return !isEven();
  }

  bool Polygon::empty() const
  {
    return points.empty();
  }

  std::ostream& operator<<(std::ostream& os, const Polygon& polygon)
  {
    std::ostream::sentry sentry(os);
    if (!sentry) {
      return os;
    }

    os << polygon.points.size() << " ";
    std::copy(polygon.points.begin(),
        polygon.points.end(),
        std::ostream_iterator< Point >(os, " "));

    return os;
  }

  std::istream& operator>>(std::istream& is, Polygon& polygon)
  {
    std::istream::sentry sentry(is);
    if (!sentry) {
      return is;
    }

    std::string line;
    if (!std::getline(is, line)) {
      is.setstate(std::ios_base::failbit);
      return is;
    }

    std::stringstream curr_line_string_stream(line);
    int num_of_points = 0;

    if (!(curr_line_string_stream >> num_of_points)) {
      is.setstate(std::ios_base::failbit);
      return is;
    }

    if (num_of_points < 0) {
      is.setstate(std::ios_base::failbit);
      return is;
    }

    polygon.points.clear();

    if (num_of_points < 3) {
      return is;
    }

    polygon.points.reserve(num_of_points);

    std::copy_n(std::istream_iterator< Point >(curr_line_string_stream),
        num_of_points,
        std::back_inserter(polygon.points));

    if (!curr_line_string_stream) {
      polygon.points.clear();
      return is;
    }

    std::string trailing;
    curr_line_string_stream >> trailing;
    if (!trailing.empty() || polygon.points.size() != static_cast< size_t >(num_of_points)) {
      polygon.points.clear();
      return is;
    }

    return is;
  }
}
